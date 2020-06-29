// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestCube.generated.h"

UCLASS()
class STATUE_API ATestCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Front;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* R;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	const FVector& GetFrontLocation()const;

	UFUNCTION(BlueprintCallable)
	const FVector& GetLeftLocation()const;


	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* Mesh;

private:
	FVector FrontLocation;
	FVector LeftLocation;

};
