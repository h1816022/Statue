#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera.generated.h"

class UArrowComponent;
class USpringArmComponent;
class UCameraComponent;
class APlayerCharacter;
class UPostProcessComponent;

UCLASS()
class STATUE_API ACamera : public AActor
{
	GENERATED_BODY()
	
public:	
	ACamera();

protected:
	virtual void BeginPlay() override;

	// 位置と回転の更新
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateTransform();
	void UpdateTransform_Implementation();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite)
	APlayerCharacter* Player;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector FlyingOffset;

	UPROPERTY(BlueprintReadWrite)
	FVector CameraLocationOffset = { 0.0f, 0.0f, 0.0f };

public:	
	virtual void Tick(float DeltaTime) override;

	void Init(APlayerCharacter* inPlayer);

	UFUNCTION(BlueprintCallable)
	void ChangeBlar(bool bIsOn);

private:
	UPROPERTY(VisibleAnywhere)
	UArrowComponent* Arrow;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraArm;

	// カメラが追いかける速さ
	UPROPERTY(EditAnywhere)
	float LagSpeed = 0.1f;

	// 離れていい距離
	UPROPERTY(EditAnywhere)
	float LagMaxDistance = 400.0f;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* FlightBlurMatInterface;

	UMaterialInstanceDynamic* FlightBlurMat;

	UPROPERTY(VisibleAnywhere)
	UPostProcessComponent* PostProcess;
};
