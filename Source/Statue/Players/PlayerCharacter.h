// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerStatus.h"
#include "../ALS/ALSEnums.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class ECameraShakeType : uint8
{
	Landhing
};

class ACamera;

UCLASS()
class STATUE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

	// �ړ�����
	UFUNCTION(BlueprintImplementableEvent)
	void PlayerMovementInput(bool bIsForwardAxis);

	// �_�b�V���{�^���������n�߂����̏���
	UFUNCTION(BlueprintImplementableEvent)
	void StartSprintHold();

	// �����Ă����_�b�V���{�^���𗣂����Ƃ��̏���
	UFUNCTION(BlueprintImplementableEvent)
	void EndSprintHold();

	// ���Ⴊ��/�����̐؂�ւ�
	UFUNCTION(BlueprintImplementableEvent)
	void StartStance();

	UFUNCTION(BlueprintImplementableEvent)
	void EndStance();

	// �W�����v�{�^�����������Ƃ�
	UFUNCTION(BlueprintImplementableEvent)
	void JumpStart();

	// �W�����v�{�^���𗣂����Ƃ�
	UFUNCTION(BlueprintImplementableEvent)
	void JumpEnd();

	UFUNCTION(BlueprintCallable)
	void SetCanChangeMode(bool NewFlag);

	// ���݂̃v���C���[�̃��[�h
	UPROPERTY(BlueprintReadWrite)
	EPlayerModeType NowModeType;

	// �ړ��p�^�[��
	UPROPERTY(BlueprintReadWrite)
	EGait DesiredGait;

	// �����Ă��邩
	UPROPERTY(BlueprintReadWrite)
	bool bSprintHeld;

	// ���[�h�`�F���W�\��
	UPROPERTY(BlueprintReadOnly)
	bool bCanChangeMode;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//virtual void NotifyHit(
	//	UPrimitiveComponent* MyComp, 
	//	AActor* Other, 
	//	UPrimitiveComponent* OtherComp, 
	//	bool bSelfMoved, 
	//	FVector HitLocation, 
	//	FVector HitNormal, 
	//	FVector NormalImpulse, 
	//	const FHitResult& Hit)override;

	UFUNCTION(BlueprintCallable)
	void PlayCameraShake(ECameraShakeType Type);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BaseLookUpRate;

	UPROPERTY(BlueprintReadOnly)
	ACamera* Camera;

	// �^�C�v���Ƃ̃J�����V�F�C�N�p�p�����[�^
	UPROPERTY(EditAnywhere)
	TMap<ECameraShakeType, TSubclassOf<UCameraShake>> ShakeData;

private:
	// �O��ړ�
	void MoveForward(float Value);

	// ���E�ړ�
	void MoveRight(float Value);

	//// �J�������ړ�
	//void TurnAtRate(float Rate);

	//// �J�����c�ړ�
	//void LookUpAtRate(float Rate);

	// ����/����̐؂�ւ�
	void ChangeWalkMode();

	// �_�b�V���J�n
	void StartSprint();

	// �_�b�V�����I����
	void EndSprint();

	// ���
	//void MoveTop(float Value);

	uint8 DontChangeCount = 0;
};
