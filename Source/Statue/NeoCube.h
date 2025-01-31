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

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	const bool GetIsMoving()const;

	UFUNCTION(BlueprintCallable)
	void SetIsMoving(bool nowFlag);

	UFUNCTION(BlueprintCallable)
	void StartMove(USphereComponent* newForce);

	UFUNCTION(BlueprintCallable)
	void Moving();

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* Mesh;

	// 初期値を保存
	FVector DefLocation;
	FRotator DefRotation;

	// 捲りあがる時の回転角度
	FRotator RandomRot;

private:
	USphereComponent* Force = nullptr;

	bool bIsMoving;
};
