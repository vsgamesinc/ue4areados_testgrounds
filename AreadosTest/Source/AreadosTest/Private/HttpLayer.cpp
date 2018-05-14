// Fill out your copyright notice in the Description page of Project Settings.

#include "HttpLayer.h"


// Sets default values
AHttpLayer::AHttpLayer() : Super()
{
 	// Unet this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AHttpLayer::BeginPlay()
{
	Super::BeginPlay();
	AHttpLayer::Http = &FHttpModule::Get();

	/*
	if (AHttpLayer::Ping())
	{
		UE_LOG(LogTemp, Warning, TEXT("AHttpLayer Ping started"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AHttpLayer Ping error"));
	}
	*/

	// You can uncomment this out for testing.
	FRequestData req;
	req.s = TEXT("123456");
	req.a = TEXT("get_items_random");
	req.p.Add(TEXT("1"));

	if (AHttpLayer::Proceed(req))
	{
		UE_LOG(LogTemp, Warning, TEXT("AHttpLayer Proceed started"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AHttpLayer Proceed error"));
	}
}

TSharedRef<IHttpRequest> AHttpLayer::RequestWithRoute(FString Subroute)
{
	UE_LOG(LogTemp, Error, TEXT("AHttpLayer::RequestWithRoute() at %s"), *(ApiBaseUrl + Subroute));
	TSharedRef<IHttpRequest> Request = AHttpLayer::Http->CreateRequest();
	Request->SetURL(ApiBaseUrl + Subroute);
	SetRequestHeaders(Request);
	return Request;
}

void AHttpLayer::SetRequestHeaders(TSharedRef<IHttpRequest>& Request)
{
	UE_LOG(LogTemp, Error, TEXT("AHttpLayer::GetRequest()"));
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
}

TSharedRef<IHttpRequest> AHttpLayer::GetRequest(FString Subroute)
{
	UE_LOG(LogTemp, Error, TEXT("AHttpLayer::GetRequest()"));
	TSharedRef<IHttpRequest> Request = RequestWithRoute(Subroute);
	Request->SetVerb("GET");
	return Request;
}

TSharedRef<IHttpRequest> AHttpLayer::PostRequest(FString Subroute, FString ContentJsonString)
{
	UE_LOG(LogTemp, Error, TEXT("AHttpLayer::PostRequest()"));
	TSharedRef<IHttpRequest> Request = RequestWithRoute(Subroute);
	Request->SetVerb("POST");
	Request->SetContentAsString(ContentJsonString);
	return Request;
}

bool AHttpLayer::Send(TSharedRef<IHttpRequest>& Request)
{
	UE_LOG(LogTemp, Error, TEXT("AHttpLayer::Send()"));
	return Request->ProcessRequest();
}

bool AHttpLayer::ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid()) return false;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) return true;
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code: %d"), Response->GetResponseCode());
		return false;
	}
}

void AHttpLayer::SetAuthorizationHash(FString Hash, TSharedRef<IHttpRequest>& Request)
{
	Request->SetHeader(AuthorizationHeader, Hash);
}

/**************************************************************************************/

template <typename StructType>
void AHttpLayer::GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput)
{
	FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(), &FilledStruct, StringOutput, 0, 0);
}

template <typename StructType>
void AHttpLayer::GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput)
{
	StructType StructData;
	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutput, 0, 0);
}

/***************************************************************************************/

bool AHttpLayer::Proceed(FRequestData requestData)
{
	//FString ContentJsonString;
	//GetJsonStringFromStruct<FRequestData>(requestData, ContentJsonString);
	//UE_LOG(LogTemp, Error, TEXT("AHttpLayer::Proceed ContentJsonString = %s"), *ContentJsonString);

	FString urlRequestString = TEXT("?s=123456&a=get_items_random&p=[2]");

	TSharedRef<IHttpRequest> Request = GetRequest(urlRequestString);
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpLayer::onResponseProceedRecieved);

	return Send(Request);
}

void AHttpLayer::onResponseProceedRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Error, TEXT("AHttpLayer::onResponseProceedRecieved started"));
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FString result = Response->GetContentAsString();

	UE_LOG(LogTemp, Warning, TEXT("AHttpLayer Proceed got data: %s"), *result);

	/*
	FResponse_Login LoginResponse;
	GetStructFromJsonString<FResponse_Login>(Response, LoginResponse);

	UE_LOG(LogTemp, Warning, TEXT("Id is: %d"), LoginResponse.id);
	UE_LOG(LogTemp, Warning, TEXT("Name is: %s"), *LoginResponse.name);
	*/
	TSharedPtr<FJsonObject> JSON_data;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(result);

	if (FJsonSerializer::Deserialize(Reader, JSON_data))
	{
		TSharedPtr<FJsonObject> nfo = JSON_data->GetObjectField("nfo");
		TArray <TSharedPtr<FJsonValue>> itype = nfo->GetArrayField("itype");
		TSharedPtr<FJsonObject> itype0 = itype[0]->AsObject();	// could crash...
		FString type0 = itype0->GetStringField("TACCESSORIZE");

		// https://answers.unrealengine.com/questions/462607/array-json-parsing.html

		UE_LOG(LogTemp, Warning, TEXT("AHttpLayer got data: %s"), *type0);
		//int32 recivedData = JSON_data->GetIntegerField("ping");
		//UE_LOG(LogTemp, Warning, TEXT("AHttpLayer Proceed get data = %i"), recivedPing);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AHttpLayer Proceed has NO data"));
	}
}


bool AHttpLayer::Ping()
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpLayer::onResponsePingRecived);

	Request->SetURL(ServerPingURL);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	return Request->ProcessRequest();
}

void AHttpLayer::onResponsePingRecived(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSusessful)
{
	TSharedPtr<FJsonObject> JSON_data;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JSON_data))
	{
		int32 recivedPing = JSON_data->GetIntegerField("ping");
		UE_LOG(LogTemp, Warning, TEXT("AHttpLayer Ping get data = %i"), recivedPing);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AHttpLayer Ping has NO data"));
	}
}

