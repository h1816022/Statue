// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NeoCube.h"

using Math = UKismetMathLibrary;

ANeoCube::ANeoCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	DefLocation = FVector();
	DefRotation = FRotator();
	RandomRot = Math::RandomRotator(true);
}

void ANeoCube::BeginPlay()
{
	Super::BeginPlay();
	
	DefLocation = GetActorLocation();
	DefRotation = GetActorRotation();
}

void ANeoCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Œ»Ý‚ÌˆÚ“®—Ê
	float NowOffset;

	// •Ï‰»—¦
	float Rate;

	if (bIsMoving)
	{
		float Distance = Math::Vector_Distance(DefLocation, UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation());
		Rate = FMath::Clamp(Math::Abs(MaxDistance - Distance) / WaveThickness, 0.0f, 1.0f);
		NowOffset = MaxOffset * Rate * -1.0f;
	}
	else
	{
		NowOffset = 0.0f;
		Rate = 1.0f;
	}

	SetActorLocation(FMath::Lerp(GetActorLocation(), (DefLocation + FVector(0.0f, 0.0f, NowOffset)), 0.2f));

	SetActorRotation(FMath::Lerp(DefRotation, RandomRot, 1.0f - Rate));

}

const bool ANeoCube::GetIsMoving()const
{
	return bIsMoving;
}

void ANeoCube::SetIsMoving(bool newFlag)
{
	bIsMoving = newFlag;
}
