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

	// 移動処理
	UFUNCTION(BlueprintImplementableEvent)
	void PlayerMovementInput(bool bIsForwardAxis);

	// ダッシュ
	UFUNCTION(BlueprintImplementableEvent)
	void StartSprintHold();
	UFUNCTION(BlueprintImplementableEvent)
	void EndSprintHold();

	// しゃがみ
	UFUNCTION(BlueprintImplementableEvent)
	void StartStance();
	UFUNCTION(BlueprintImplementableEvent)
	void EndStance();

	// ジャンプボタンを押したとき
	UFUNCTION(BlueprintImplementableEvent)
	void JumpStart();

	// ジャンプボタンを離したとき
	UFUNCTION(BlueprintImplementableEvent)
	void JumpEnd();

	// チェンジしていいかの記録を更新
	UFUNCTION(BlueprintCallable)
	void SetCanChangeMode(bool NewFlag);

	// 指定時間動けないようにする
	UFUNCTION(BlueprintCallable)
	void StopMove(float ReturnDelay);

	// 現在のプレイヤーのモード
	UPROPERTY(BlueprintReadWrite)
	EPlayerModeType NowModeType;

	// 移動パターン
	UPROPERTY(BlueprintReadWrite)
	EGait DesiredGait;
	
	// 走っているか
	UPROPERTY(BlueprintReadWrite)
	bool bSprintHeld;

	// モードチェンジ可能か
	UPROPERTY(BlueprintReadOnly)
	bool bCanChangeMode;

	UPROPERTY(BlueprintReadWrite)
	UMaterialInstanceDynamic* BodyMaterial;

	UPROPERTY(BlueprintReadOnly)
	bool bCanMove;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// タイプごとのカメラ振動を再生
	UFUNCTION(BlueprintCallable)
	void PlayCameraShake(const ECameraShakeType& Type);

	// プレイヤーのモードを変更
	// CertainlyChangeで、変更中でも変更を待機させのちに遂行させる
	UFUNCTION(BlueprintCallable)
	void ChangePlayerMode(const EPlayerModeType& NewMode, bool bIsCertainlyChange);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BaseLookUpRate;

	UPROPERTY(BlueprintReadOnly)
	ACamera* Camera;

	// タイプごとのカメラシェイク用パラメータ
	UPROPERTY(EditAnywhere)
	TMap<ECameraShakeType, TSubclassOf<UCameraShake>> ShakeData;

private:
	// 前後移動
	void MoveForward(float Value);

	// 左右移動
	void MoveRight(float Value);

	// 歩き/走りの切り替え
	void ChangeWalkMode();

	// ダッシュ開始
	void StartSprint();

	// ダッシュを終える
	void EndSprint();

	UFUNCTION()
	void UpdateBodyMaterial(float Value);

	UFUNCTION()
	void Finished();

	UFUNCTION()
	void StartMove();

	// モードチェンジ時用のタイムライン
	FTimeline* ModeChangeTl;

	// モードチェンジを縛っている数
	// 0ならチェンジ可
	uint8 DontChangeCount = 0;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FChangeMode ChangeMode;
};
