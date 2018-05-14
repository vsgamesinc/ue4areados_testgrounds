// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "HttpLayer.generated.h"

USTRUCT()
struct FRequestData
{
	GENERATED_BODY()
	UPROPERTY() FString s;			// session ID
	UPROPERTY() FString a;			// action request
	UPROPERTY() TArray<FString> p;	// params

	FRequestData() {}
};

UCLASS()
class AREADOSTEST_API AHttpLayer : public AActor
{
	GENERATED_BODY()
	
private:	
	FHttpModule* Http;

	// UE WIKI code ^__^
	FString AuthorizationHeader = TEXT("Authorization");
	void SetAuthorizationHash(FString Hash, TSharedRef<IHttpRequest>& Request);

	TSharedRef<IHttpRequest> RequestWithRoute(FString Subroute);
	void SetRequestHeaders(TSharedRef<IHttpRequest>& Request);

	TSharedRef<IHttpRequest> GetRequest(FString Subroute);
	TSharedRef<IHttpRequest> PostRequest(FString Subroute, FString ContentJsonString);
	bool Send(TSharedRef<IHttpRequest>& Request);

	bool ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);

	template <typename StructType>
	void GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput);
	template <typename StructType>
	void GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput);

public:
	UPROPERTY(EditAnywhere, Category = "HTTP_Layer")
	FString ApiBaseUrl = TEXT("http://areados.com/datalayer/iquery.php");	// API "http://areados.com/datalayer/iquery.php" http://areados.com/datalayer/iquery.php

	UPROPERTY(EditAnywhere, Category = "HTTP_Layer")
	FString ServerPingURL = TEXT("http://areados.com/datalayer/init.php");	// Default Ping URL

	UFUNCTION()
	bool Ping();

	void onResponsePingRecived(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSusessful);

	// Sets default values for this actor's properties
	AHttpLayer();

	bool Proceed(FRequestData LoginCredentials);
	void onResponseProceedRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
