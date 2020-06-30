#include "Cube.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

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

ACube::ACube()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Front = CreateDefaultSubobject<USceneComponent>(TEXT("Fr"));
	Front->SetupAttachment(Mesh);
	Front->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));

	Left = CreateDefaultSubobject<USceneComponent>(TEXT("Le"));
	Left->SetupAttachment(Mesh);
	Left->SetRelativeLocation(FVector(0.0f, -100.0f, 0.0f));

	FrontLocation = FVector();
	LeftLocation = FVector();

	DefLocation = FVector();
	DefRotation = FRotator();
	RandomRot = Math::RandomRotator(true);
}

void ACube::BeginPlay()
{
	Super::BeginPlay();

	//FrontLocation = Front->GetComponentLocation();

	//LeftLocation = Left->GetComponentLocation();

	//DefLocation = GetActorLocation();
	//DefRotation = GetActorRotation();
}

void ACube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FVector& ACube::GetFrontLocation() const
{
	return FrontLocation;
}

const FVector& ACube::GetLeftLocation() const
{
	return LeftLocation;
}

void ACube::UpdateTransform()
{
	const float Distance = Math::Vector_Distance(DefLocation, UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation());
	const float Rate = FMath::Clamp(Math::Abs(MAX_DISTANCE - Distance) / WAVE_THICKNESS, 0.0f, 1.0f);

	const float NowOffset = MAX_OFFSET * Rate * -1.0f;

	SetActorLocation(FMath::Lerp(GetActorLocation(), (DefLocation + FVector(0.0f, 0.0f, NowOffset)), 0.05f));

	SetActorRotation(FMath::Lerp(DefRotation, RandomRot, 1.0f - Rate));
}
