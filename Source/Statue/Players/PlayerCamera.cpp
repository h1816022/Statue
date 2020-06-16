#include "PlayerCamera.h"
#include "Components/ArrowComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter.h"

APlayerCamera::APlayerCamera()
	:Player(Player)
{
	PrimaryActorTick.bCanEverTick = true;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SetRootComponent(Arrow);
	CameraArm->SetupAttachment(Arrow);
	Camera->SetupAttachment(CameraArm);
}

void APlayerCamera::BeginPlay()
{
	Super::BeginPlay();
	
	const auto& Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	Controller->SetViewTargetWithBlend(this);
}

void APlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTransform();
}

void APlayerCamera::Init(APlayerCharacter* inPlayer)
{
	Player = inPlayer;
}

void APlayerCamera::UpdateTransform()
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
	SetActorRotation(Player->GetControlRotation());
}

