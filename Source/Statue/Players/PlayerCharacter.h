// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerModeType.h"
#include "../ALS/ALSEnums.h"
#include "PlayerCharacter.generated.h"

class ACamera;

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

	// ダッシュボタンを押し始めた時の処理
	UFUNCTION(BlueprintImplementableEvent)
	void StartSprintHold();

	// 押していたダッシュボタンを離したときの処理
	UFUNCTION(BlueprintImplementableEvent)
	void EndSprintHold();

	// しゃがみ/立ちの切り替え
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeStance();

	// ジャンプボタンを押したとき
	UFUNCTION(BlueprintImplementableEvent)
	void JumpStart();

	// ジャンプボタンを離したとき
	UFUNCTION(BlueprintImplementableEvent)
	void JumpEnd();

	UFUNCTION(BlueprintCallable)
	void SetCanChangeMode(bool NewFlag);

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

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyHit(
		UPrimitiveComponent* MyComp, 
		AActor* Other, 
		UPrimitiveComponent* OtherComp, 
		bool bSelfMoved, 
		FVector HitLocation, 
		FVector HitNormal, 
		FVector NormalImpulse, 
		const FHitResult& Hit)override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BaseLookUpRate;

private:
	// 前後移動
	void MoveForward(float Value);

	// 左右移動
	void MoveRight(float Value);

	// カメラ横移動
	void TurnAtRate(float Rate);

	// カメラ縦移動
	void LookUpAtRate(float Rate);

	// 歩き/走りの切り替え
	void ChangeWalkMode();

	// ダッシュ開始
	void StartSprint();

	// ダッシュを終える
	void EndSprint();

	// 飛ぶ
	void MoveTop(float Value);

	UPROPERTY()
	ACamera* Camera;

	uint8 DontChangeCount = 0;
};
