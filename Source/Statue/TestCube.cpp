// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TestCube.h"

namespace
{
	constexpr float MAX_DISTANCE = 600.0f;

	constexpr float WAVE_THICKNESS = 100.0f;

	constexpr float MAX_OFFSET = 100.0f;

	// èâä˙ílÇï€ë∂
	FVector DefLocation;
	FRotator DefRotation;

	// åûÇËÇ†Ç™ÇÈéûÇÃâÒì]äpìx
	FRotator RandomRot;
}

using Math = UKismetMathLibrary;

// Sets default values
ATestCube::ATestCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	R = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(R);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(R);

	Front = CreateDefaultSubobject<USceneComponent>(TEXT("Front"));
	Front->SetupAttachment(R);
	Front->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));

	Left = CreateDefaultSubobject<USceneComponent>(TEXT("Left"));
	Left->SetupAttachment(R);
	Left->SetRelativeLocation(FVector(0.0f, -100.0f, 0.0f));

	FrontLocation = FVector();
	LeftLocation = FVector();

	DefLocation = FVector();
	DefRotation = FRotator();
	RandomRot = Math::RandomRotator(true);
}

// Called when the game starts or when spawned
void ATestCube::BeginPlay()
{
	Super::BeginPlay();
	
	FrontLocation = Front->GetComponentLocation();

	LeftLocation = Left->GetComponentLocation();

	DefLocation = GetActorLocation();
	DefRotation = GetActorRotation();
}

const FVector& ATestCube::GetFrontLocation() const
{
	return FrontLocation;
}

const FVector& ATestCube::GetLeftLocation() const
{
	return LeftLocation;
}

// Called every frame
void ATestCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float Distance = Math::Vector_Distance(DefLocation, UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation());
	const float Rate = FMath::Clamp(Math::Abs(MAX_DISTANCE - Distance) / WAVE_THICKNESS, 0.0f, 1.0f);

	const float NowOffset = MAX_OFFSET * Rate * -1.0f;

	SetActorLocation(FMath::Lerp(GetActorLocation(), (DefLocation + FVector(0.0f, 0.0f, NowOffset)), 0.05f));

	SetActorRotation(FMath::Lerp(DefRotation, RandomRot, 1.0f - Rate));

}

