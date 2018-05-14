// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorFieldReciever.h"
#include "Runtime/Engine/Classes/Components/VectorFieldComponent.h"


// Sets default values for this component's properties
UVectorFieldReciever::UVectorFieldReciever()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVectorFieldReciever::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitVectorField();
}


void UVectorFieldReciever::InitVectorField()
{
	if (UVectorFieldReciever::vsDustWindActor)
	{
		auto vectorFieldComponent = UVectorFieldReciever::vsDustWindActor->GetVectorFieldComponent();
		UVectorFieldReciever::vsDustWind = vectorFieldComponent->VectorField;
		
		//*UVectorFieldReciever::vsDustWind
		UE_LOG(LogTemp, Warning, TEXT("Got VectorFieldActor %s & VectorField %s"), *UVectorFieldReciever::vsDustWindActor->GetName(), TEXT("test"));
		//UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s from %s"), *ourMechaName, *HitLocation.ToString(), *turretLocation.ToString());
	}
	return;
}

// Called every frame
void UVectorFieldReciever::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

