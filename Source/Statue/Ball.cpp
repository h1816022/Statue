#include "Ball.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"

using Math = UKismetMathLibrary;

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);

	Col = CreateDefaultSubobject<USphereComponent>(TEXT("Col"));
	Col->SetupAttachment(Sphere);

	Dir = Math::RandomUnitVector();

	NowType = EBallMoveType::Physics;
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
}

void ABall::Bound(const FVector& NormalImpulse)
{
	FVector Ni = NormalImpulse.GetSafeNormal();
	Dir -= (Math::Dot_VectorVector(Dir, Ni) * 2.0f * Ni);
}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (NowType)
	{
	case EBallMoveType::Grab:
		break;
	case EBallMoveType::Physics:
		PhysicsUpdate();
		break;

	case EBallMoveType::Fly:
		break;
	default:
		break;
	}
}

void ABall::PhysicsUpdate()
{
	Sphere->SetPhysicsLinearVelocity(Dir * Speed);
}

