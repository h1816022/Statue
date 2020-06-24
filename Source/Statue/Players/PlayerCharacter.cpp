#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "../Cameras/Camera.h"
#include "Kismet/GameplayStatics.h"

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
	
	FTransform SpawnTransform(FRotator(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f));

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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL"));
	}
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
	//PlayerInputComponent->BindAxis("MoveTop", this, &APlayerCharacter::MoveTop);

	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction(TEXT("Walk"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ChangeWalkMode);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &APlayerCharacter::StartSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &APlayerCharacter::EndSprint);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::JumpStart);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &APlayerCharacter::JumpEnd);
	PlayerInputComponent->BindAction(TEXT("Stance"), EInputEvent::IE_Pressed, this, &APlayerCharacter::StartStance);
	PlayerInputComponent->BindAction(TEXT("Stance"), EInputEvent::IE_Released, this, &APlayerCharacter::EndStance);
}

//void APlayerCharacter::NotifyHit(
//	UPrimitiveComponent* MyComp, 
//	AActor* Other, 
//	UPrimitiveComponent* OtherComp, 
//	bool bSelfMoved, 
//	FVector HitLocation, 
//	FVector HitNormal, 
//	FVector NormalImpulse, 
//	const FHitResult& Hit)
//{
//	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
//
//	// 歩行可能な角度
//	auto WalkableAngle = FMath::DegreesToRadians(GetCharacterMovement()->GetWalkableFloorAngle());
//
//	// 飛行モードで、かつ接触した物体が歩行可能な法線を返した場合に歩行モードにする
//	if (GetCharacterMovement()->IsFlying()
//	&& acosf(FVector::DotProduct(HitNormal, FVector::UpVector)) < WalkableAngle)
//	{
//		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
//	}
//}

void APlayerCharacter::MoveForward(float Value)
{
	PlayerMovementInput(true);
}

void APlayerCharacter::MoveRight(float Value)
{
	PlayerMovementInput(false);
}

//void APlayerCharacter::MoveTop(float Value)
//{
//	if ((Controller != nullptr) && (Value != 0.0f))
//	{
//		if (Value > 0 && !GetCharacterMovement()->IsFlying())
//		{
//			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
//		}
//
//		const FRotator Rotation = Controller->GetControlRotation();
//		const FRotator YawRotation(0, Rotation.Yaw, 0);
//
//		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
//		AddMovementInput(Direction, Value);
//	}
//}

//void APlayerCharacter::TurnAtRate(float Rate)
//{
//	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
//}
//
//void APlayerCharacter::LookUpAtRate(float Rate)
//{
//	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
//}

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