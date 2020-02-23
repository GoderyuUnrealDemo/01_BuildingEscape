// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/TriggerVolume.h"
#include "Engine.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("Pointer Owner is nullptr"));
		return;
	}
	OriginRotator = Owner->GetTransform().GetRotation().Rotator();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	// If the ActorThatOpens is in the volume
	// if(DoOnce)
	// {
		if(GetTotalMassOfActorsOnPlate() > TriggerMass)
		{
			OpenDoor();
			// DoOnce = !DoOnce;
			LastDoorOpenTime = GetWorld()->GetTimeSeconds();
		}

		if ((GetWorld()->GetTimeSeconds() - LastDoorOpenTime) > DoorCloseDelay)
		{
			CloseDoor();
		}
	// }
}

void UOpenDoor::OpenDoor()
{
	// if (!Owner)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Pointer Owner is nullptr"));
	// 	return;
	// }
	// Owner->SetActorRotation(FRotator(0.f, OriginRotator.Yaw + OpenAngle, 0.f));
	OnOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	// if (!Owner)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Pointer Owner is nullptr"));
	// 	return;
	// }
	// Owner->SetActorRotation(FRotator(0.f, OriginRotator.Yaw, 0.f));
	OnCloseRequest.Broadcast();
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	// TODO calc total mass
	float TotalMass = 0.f;
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("Pointer PressurePlate is nullptr"));
		return TotalMass;
	}
	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	// Iterate through them adding their masses
	for(const auto& Actor : OverlappingActors)
	{
		if (!Actor)
		{
			UE_LOG(LogTemp, Error, TEXT("TArray<AACtor*> OverlappingActors has nullptr"));
			return TotalMass;
		}
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
	}

	return TotalMass;
}