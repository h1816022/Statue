#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCamera.generated.h"

class UArrowComponent;
class USpringArmComponent;
class UCameraComponent;
class APlayerCharacter;

UCLASS()
class STATUE_API APlayerCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayerCamera();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Init(APlayerCharacter* inPlayer);

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

	// ÉJÉÅÉâÇ™ó£ÇÍÇƒÇ‡Ç¢Ç¢ãóó£
	UPROPERTY(EditAnywhere)
	float LagMaxDistance = 400.0f;

	UPROPERTY()
	APlayerCharacter* Player;
};
