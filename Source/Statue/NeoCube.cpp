#include "NeoCube.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

using Math = UKismetMathLibrary;

ANeoCube::ANeoCube()
{
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
}

const bool ANeoCube::GetIsMoving()const
{
	return bIsMoving;
}

void ANeoCube::SetIsMoving(bool newFlag)
{
	bIsMoving = newFlag;
}

void ANeoCube::Moving(/*const FVector& Center, const float Size*/float Rate)
{
	if (bIsMoving)
	{
		// åªç›ÇÃà⁄ìÆó 
		float NowOffset = MaxOffset * Rate;
		
		SetActorLocation(FMath::Lerp(GetActorLocation(), (DefLocation + FVector(0.0f, 0.0f, NowOffset)), 0.2f));
		SetActorRotation(FMath::Lerp(DefRotation, RandomRot,  Rate));
	}
	else
	{
		SetActorLocation(FMath::Lerp(GetActorLocation(), DefLocation, 0.2f));
		SetActorRotation(FMath::Lerp(GetActorRotation(), DefRotation, 0.2f));
	}
}