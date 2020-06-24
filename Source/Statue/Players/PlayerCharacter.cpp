#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "../Cameras/Camera.h"
#include "Kismet/GameplayStatics.h"
#include "../Cameras/MyCameraShake.h"
#include "GameFramework/PhysicsVolume.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

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
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FTransform SpawnTransform(GetActorRotation(), GetActorLocation());


	// カメラ生成
	FString Path = "/Game/_Users/Cameras/Blueprints/BP_Camera.BP_Camera_C";
	TSubclassOf<class ACamera> Sc = TSoftClassPtr<ACamera>(FSoftObjectPath(*Path)).LoadSynchronous();

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
	Cm->GetPhysicsVolume()->FluidFriction = 0.0f;
}

void APlayerCharacter::SetCanChangeMode(bool NewFlag)
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

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

void APlayerCharacter::PlayCameraShake(ECameraShakeType Type)
{
	auto controller = UGameplayStatics::GetPlayerController(this, 0);
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(ShakeData[Type], 1.0F);
}

void APlayerCharacter::MoveForward(float Value)
{
	PlayerMovementInput(true);
}

void APlayerCharacter::MoveRight(float Value)
{
	PlayerMovementInput(false);
}

void APlayerCharacter::ChangeWalkMode()
{
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
	DesiredGait = EGait::Sprinting;
}

void APlayerCharacter::EndSprint()
{
	DesiredGait = EGait::Running;
}