#include "Camera.h"
#include "Components/ArrowComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

ACamera::ACamera()
{
	PrimaryActorTick.bCanEverTick = true;
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SetRootComponent(Arrow);
	CameraArm->SetupAttachment(Arrow);
	Camera->SetupAttachment(CameraArm);
}

void ACamera::BeginPlay()
{
	Super::BeginPlay();
	
	const auto& Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	Controller->SetViewTargetWithBlend(this);
}

void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTransform();
}

void ACamera::Init(APlayerCharacter* inPlayer)
{
	Player = inPlayer;
}

void ACamera::UpdateTransform()
{
	FVector MyLocation = GetActorLocation();
	FVector PlayerLocation = Player->GetActorLocation();
	float Distance = FVector::Distance(MyLocation, PlayerLocation);

	FVector NewLocation;

	// ƒJƒƒ‰‚ª—£‚ê‚Ä‚¢‚¢‹——£‚ð’´‚¦‚Ä‚¢‚é‚©
	if (Distance > LagMaxDistance)
	{
		// LagMaxDistance‚Ì‹——£‚Ü‚Å‚É•â³‚µ‚½ˆÊ’u‚É‚·‚é
		FVector ForwardVec = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(PlayerLocation, MyLocation));
		NewLocation = PlayerLocation + ForwardVec * LagMaxDistance;
	}
	else
	{
		// ƒ‰ƒO‚è‚È‚ª‚ç‹ß‚Ã‚­
		NewLocation = FMath::Lerp(MyLocation, PlayerLocation, LagSpeed);
	}

	SetActorLocation(NewLocation);

	// ‰ñ“]
	if (Player->GetCharacterMovement()->MovementMode == MOVE_Flying)
	{
		SetActorRotation(Player->GetActorRotation());
	}
	else
	{
		SetActorRotation(Player->GetControlRotation());
	}
}


