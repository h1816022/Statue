#include "PlayerCamera.h"
#include "Components/ArrowComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter.h"

APlayerCamera::APlayerCamera()
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

	//UpdateTransform();
}

//void APlayerCamera::Init(APlayerCharacter* inPlayer)
//{
//	Player = inPlayer;
//}

//void APlayerCamera::UpdateTransform()
//{
//	FVector MyLocation = GetActorLocation();
//	FVector PlayerLocation = Player->GetActorLocation();
//	float Distance = FVector::Distance(MyLocation, PlayerLocation);
//
//	FVector NewLocation;
//
//	// カメラが離れていい距離を超えているか
//	if (Distance > LagMaxDistance)
//	{
//		// LagMaxDistanceの距離までに補正した位置にする
//		FVector ForwardVec = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(PlayerLocation, MyLocation));
//		NewLocation = PlayerLocation + ForwardVec * LagMaxDistance;
//	}
//	else
//	{
//		// ラグりながら近づく
//		NewLocation = FMath::Lerp(MyLocation, PlayerLocation, LagSpeed);
//	}
//
//	SetActorLocation(NewLocation);
//	SetActorRotation(Player->GetControlRotation());
//}
//
