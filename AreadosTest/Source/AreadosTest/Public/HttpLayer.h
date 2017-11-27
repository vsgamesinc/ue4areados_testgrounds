// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "HttpLayer.generated.h"

UCLASS()
class AREADOSTEST_API AHttpLayer : public AActor
{
	GENERATED_BODY()
	
public:	
	FHttpModule* Http;

	UPROPERTY(EditAnywhere, Category = "HTTP_Layer")
	FString ServerURL = "http://areados.com/datalayer/init.php";

	UFUNCTION()
	bool Ping();

	void onResponseRecived(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSusessful);

	// Sets default values for this actor's properties
	AHttpLayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
};
