// Fill out your copyright notice in the Description page of Project Settings.

#include "HttpLayer.h"


// Sets default values
AHttpLayer::AHttpLayer() : Super()
{
 	// Unet this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AHttpLayer::Http = &FHttpModule::Get();
}

// Called when the game starts or when spawned
void AHttpLayer::BeginPlay()
{
	Super::BeginPlay();

	if (AHttpLayer::Ping())
	{
		UE_LOG(LogTemp, Warning, TEXT("AHttpLayer Ping started"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AHttpLayer Ping error"));
	}
}

bool AHttpLayer::Ping()
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpLayer::onResponseRecived);

	Request->SetURL(ServerURL);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	return Request->ProcessRequest();
}

void AHttpLayer::onResponseRecived(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSusessful)
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

