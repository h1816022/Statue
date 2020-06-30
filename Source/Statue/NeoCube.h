#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeoCube.generated.h"

class USphereComponent;

UCLASS()
class STATUE_API ANeoCube : public AActor
{
	GENERATED_BODY()
	
public:	
	ANeoCube();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	float MaxDistance = 600.0f;

	UPROPERTY(EditDefaultsOnly)
	float WaveThickness = 100.0f;

	UPROPERTY(EditDefaultsOnly)
	float MaxOffset = 100.0f;

	// �����l��ۑ�

	UPROPERTY(BlueprintReadOnly)
	FVector DefLocation;

	UPROPERTY(BlueprintReadOnly)
	FRotator DefRotation;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	const bool GetIsMoving()const;

	UFUNCTION(BlueprintCallable)
	void SetIsMoving(bool nowFlag);

	UFUNCTION(BlueprintCallable)
	void Moving(/*const FVector& Center, const float Size*/float Rate);

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* Mesh;

private:
	bool bIsMoving;

	// ���肠���鎞�̉�]�p�x
	FRotator RandomRot;
};
