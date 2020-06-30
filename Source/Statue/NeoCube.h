// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeoCube.generated.h"

UCLASS()
class STATUE_API ANeoCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANeoCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	float MaxDistance = 600.0f;

	UPROPERTY(EditDefaultsOnly)
	float WaveThickness = 100.0f;

	UPROPERTY(EditDefaultsOnly)
	float MaxOffset = 100.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	const bool GetIsMoving()const;

	UFUNCTION(BlueprintCallable)
	void SetIsMoving(bool nowFlag);

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* Mesh;

	// èâä˙ílÇï€ë∂
	FVector DefLocation;
	FRotator DefRotation;

	// åûÇËÇ†Ç™ÇÈéûÇÃâÒì]äpìx
	FRotator RandomRot;

private:
	bool bIsMoving;
};
