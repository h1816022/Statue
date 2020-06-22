#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera.generated.h"

class UArrowComponent;
class USpringArmComponent;
class UCameraComponent;
class APlayerCharacter;

UCLASS()
class STATUE_API ACamera : public AActor
{
	GENERATED_BODY()
	
public:	
	ACamera();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	APlayerCharacter* Player;

public:	
	virtual void Tick(float DeltaTime) override;

	void Init(APlayerCharacter* inPlayer);

	UFUNCTION(BlueprintPure)
	UCameraComponent* GetCameraComponent()const;

private:
	void UpdateTransform();

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* Arrow;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	float LagSpeed = 0.1f;

	UPROPERTY(EditAnywhere)
	float LagMaxDistance = 400.0f;
};
