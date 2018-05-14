// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/VectorField/VectorField.h"
#include "Runtime/Engine/Classes/VectorField/VectorFieldStatic.h"
#include "Runtime/Engine/Classes/VectorField/VectorFieldVolume.h"
#include "VectorFieldReciever.generated.h"


UCLASS( ClassGroup=(VectorFields), meta=(BlueprintSpawnableComponent) )
class UVectorFieldReciever : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVectorFieldReciever();

	/** Vector Field Actor to get da Wind... */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Wind_Setup)
	AVectorFieldVolume* vsDustWindActor;

	UFUNCTION(BlueprintCallable, Category = "Wind_Setup", meta = (WorldContext = "WorldContextObject"))
	void InitVectorField();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	/** Vector Field to Recieve Wind vectors */
	UPROPERTY(EditAnywhere, Category = Wind_Setup)
	UVectorField* vsDustWind;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
