#include "Camera.h"
#include "Components/ArrowComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Players/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PostProcessComponent.h"

ACamera::ACamera()
{
	PrimaryActorTick.bCanEverTick = true;
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));

	SetRootComponent(Arrow);
	CameraArm->SetupAttachment(Arrow);
	Camera->SetupAttachment(CameraArm);
	PostProcess->SetupAttachment(RootComponent);
}

void ACamera::BeginPlay()
{
	Super::BeginPlay();
	
	const auto& Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	Controller->SetViewTargetWithBlend(this);

	// 全範囲に適用
	PostProcess->bUnbound = true;

	// 早い飛行の時のブラー効果準備
	FlightBlurMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, FlightBlurMatInterface);

	const FWeightedBlendable WeightedBlendable = { 1, FlightBlurMat};
	PostProcess->Settings.WeightedBlendables.Array.Add(WeightedBlendable);
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

UCameraComponent* ACamera::GetCameraComponent()const
{
	return Camera;
}

void ACamera::ChangeBlar(bool bIsOn)
{
	FName Name = "IsApply";
	FlightBlurMat->SetScalarParameterValue(Name, (bIsOn ? 1.0f : 0.0f));
}

void ACamera::UpdateTransform_Implementation()
{
	FVector MyLocation = GetActorLocation();
	FVector PlayerLocation = Player->GetActorLocation();

	SetActorLocation(PlayerLocation);

	return;

	float Distance = FVector::Distance(MyLocation, PlayerLocation);

	FVector NewLocation;

	// カメラが離れていい距離を超えているか
	//if (Distance > LagMaxDistance)
	//{
	//	// LagMaxDistanceの距離までに補正した位置にする
	//	FVector ForwardVec = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(PlayerLocation, MyLocation));
	//	NewLocation = PlayerLocation + ForwardVec * LagMaxDistance;
	//}
	//else
	{
		// ラグりながら近づく
		NewLocation = FMath::Lerp(MyLocation, PlayerLocation, LagSpeed);
	}

	SetActorLocation(NewLocation);

	// 回転
	SetActorRotation(Player->GetControlRotation());
}


