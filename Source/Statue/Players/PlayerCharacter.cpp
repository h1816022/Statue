#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "../Cameras/Camera.h"
#include "Kismet/GameplayStatics.h"
#include "../Cameras/MyCameraShake.h"
#include "GameFramework/PhysicsVolume.h"
#include "Components/TimelineComponent.h"
#include "UObject/ConstructorHelpers.h"

// 流体摩擦力(空中制動に関わる)
constexpr float DEF_FLUID_FRICTION = 2.0f;			// 基本値
constexpr float CANT_MOVE_FLUID_FRICTION = 0.0f;	// 自由に動けないとき

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bCanMove = true;

	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;

	// コントローラーの回転入力についていって回転しないように
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	NowModeType = EPlayerModeType::Human;
	DesiredGait = EGait::Running;
	bCanChangeMode = true;


	// モードチェンジ用タイムライン
	ModeChangeTl = new FTimeline();
	ModeChangeTl->SetTimelineLength(1.0f);

	const ConstructorHelpers::FObjectFinder<UCurveFloat> StepCurve(TEXT("/Game/_Users/Players/Data/Curve_ChangeMode_Float"));

	// タイムライン更新時に呼ばれる関数
	FOnTimelineFloat ModeChangeStep;;
	ModeChangeStep.BindUFunction(this, "UpdateBodyMaterial");
	ModeChangeTl->AddInterpFloat(StepCurve.Object, ModeChangeStep);

	// タイムライン終了時に呼ばれる関数
	FOnTimelineEvent ModeChangeFinished;
	ModeChangeFinished.BindUFunction(this, "Finished");
	ModeChangeTl->SetTimelineFinishedFunc(ModeChangeFinished);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FTransform SpawnTransform(GetActorRotation(), GetActorLocation());


	// カメラ生成
	const FString Path = "/Game/_Users/Cameras/Blueprints/BP_Camera.BP_Camera_C";
	const TSubclassOf<class ACamera> Sc = TSoftClassPtr<ACamera>(FSoftObjectPath(*Path)).LoadSynchronous();

	if (Sc != nullptr)
	{
		// 生成処理
		Camera = Cast<ACamera>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, Sc, SpawnTransform));

		// コンストラクタの前に変数を書き換えるInit処理
		Camera->Init(this);
		UGameplayStatics::FinishSpawningActor(Camera, SpawnTransform);
	}

	// メッシュのマテリアル取得
	auto MyMesh = GetMesh();
	BodyMaterial = MyMesh->CreateDynamicMaterialInstance(0, MyMesh->GetMaterial(0));

	auto Cm = GetCharacterMovement();
	Cm->GroundFriction = 100.0f;
	Cm->GetPhysicsVolume()->FluidFriction = DEF_FLUID_FRICTION;
}

void APlayerCharacter::SetCanChangeMode(const bool NewFlag)
{
	if (NewFlag)
	{
		DontChangeCount = FMath::Max(DontChangeCount - 1, 0);

		if (DontChangeCount == 0)
		{
			bCanChangeMode = true;
		}
	}
	else
	{
		DontChangeCount++;
		bCanChangeMode = false;
	}
}

void APlayerCharacter::StopMove(const float ReturnDelay)
{
	bCanMove = false;

	auto Cm = GetCharacterMovement();
	Cm->GetPhysicsVolume()->FluidFriction = CANT_MOVE_FLUID_FRICTION;

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &APlayerCharacter::StartMove, ReturnDelay, false);
}

void APlayerCharacter::StartMove()
{
	bCanMove = true;

	auto Cm = GetCharacterMovement();
	Cm->GetPhysicsVolume()->FluidFriction = DEF_FLUID_FRICTION;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ModeChangeTl != nullptr && ModeChangeTl->IsPlaying())
	{
		ModeChangeTl->TickTimeline(DeltaTime);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAction(TEXT("Walk"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ChangeWalkMode);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &APlayerCharacter::StartSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &APlayerCharacter::EndSprint);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::JumpStart);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &APlayerCharacter::JumpEnd);
	PlayerInputComponent->BindAction(TEXT("Stance"), EInputEvent::IE_Pressed, this, &APlayerCharacter::StartStance);
	PlayerInputComponent->BindAction(TEXT("Stance"), EInputEvent::IE_Released, this, &APlayerCharacter::EndStance);
}

void APlayerCharacter::PlayCameraShake(const ECameraShakeType& Type)
{
	auto controller = UGameplayStatics::GetPlayerController(this, 0);
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(ShakeData[Type], 1.0f);
}

void APlayerCharacter::ChangePlayerMode(const EPlayerModeType& NewMode, const bool bIsCertainlyChange)
{
	if (NewMode == NowModeType)
	{
		return;
	}

	if (bCanChangeMode)
	{
		SetCanChangeMode(false);

		switch (NowModeType)
		{
		case EPlayerModeType::Human:
			NowModeType = EPlayerModeType::Statue;
			ModeChangeTl->PlayFromStart(); 
			break;

		case EPlayerModeType::Statue:
			NowModeType = EPlayerModeType::Human;
			ModeChangeTl->ReverseFromEnd();
			break;

		default:
			break;
		}

		ChangeMode.Broadcast(NowModeType);
	}
}

void APlayerCharacter::MoveForward(const float Value)
{
	if (!bCanMove)
	{
		return;
	}

	PlayerMovementInput(true);
}

void APlayerCharacter::MoveRight(const float Value)
{
	if (!bCanMove)
	{
		return;
	}

	PlayerMovementInput(false);
}

void APlayerCharacter::ChangeWalkMode()
{
	if (!bCanMove)
	{
		return;
	}

	switch (DesiredGait)
	{
	case EGait::Walking:
		DesiredGait = EGait::Running;
	case EGait::Running:
		DesiredGait = EGait::Walking;
	case EGait::Sprinting:
		;
	default:
		;
	}
}

void APlayerCharacter::StartSprint()
{
	if (!bCanMove)
	{
		return;
	}

	DesiredGait = EGait::Sprinting;
}

void APlayerCharacter::EndSprint()
{
	DesiredGait = EGait::Running;
}

void APlayerCharacter::UpdateBodyMaterial(const float Value)
{
	const FName ParamName = "DissolveRate";
	BodyMaterial->SetScalarParameterValue(ParamName, Value);
}

void APlayerCharacter::Finished()
{
	SetCanChangeMode(true);
}
