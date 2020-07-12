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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeMode, EPlayerModeType, NewType);

class ACamera;
struct FTimeline;

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

	// �_�b�V��
	UFUNCTION(BlueprintImplementableEvent)
	void StartSprintHold();
	UFUNCTION(BlueprintImplementableEvent)
	void EndSprintHold();

	// ���Ⴊ��
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

	// �`�F���W���Ă������̋L�^���X�V
	UFUNCTION(BlueprintCallable)
	void SetCanChangeMode(bool NewFlag);

	// �w�莞�ԓ����Ȃ��悤�ɂ���
	UFUNCTION(BlueprintCallable)
	void StopMove(float ReturnDelay);

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

	UPROPERTY(BlueprintReadWrite)
	UMaterialInstanceDynamic* BodyMaterial;

	UPROPERTY(BlueprintReadOnly)
	bool bCanMove;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �^�C�v���Ƃ̃J�����U�����Đ�
	UFUNCTION(BlueprintCallable)
	void PlayCameraShake(const ECameraShakeType& Type);

	// �v���C���[�̃��[�h��ύX
	// CertainlyChange�ŁA�ύX���ł��ύX��ҋ@�����̂��ɐ��s������
	UFUNCTION(BlueprintCallable)
	void ChangePlayerMode(const EPlayerModeType& NewMode, bool bIsCertainlyChange);

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

	// ����/����̐؂�ւ�
	void ChangeWalkMode();

	// �_�b�V���J�n
	void StartSprint();

	// �_�b�V�����I����
	void EndSprint();

	UFUNCTION()
	void UpdateBodyMaterial(float Value);

	UFUNCTION()
	void Finished();

	UFUNCTION()
	void StartMove();

	// ���[�h�`�F���W���p�̃^�C�����C��
	FTimeline* ModeChangeTl;

	// ���[�h�`�F���W�𔛂��Ă��鐔
	// 0�Ȃ�`�F���W��
	uint8 DontChangeCount = 0;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FChangeMode ChangeMode;
};
