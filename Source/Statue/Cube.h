#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cube.generated.h"

UCLASS()
class STATUE_API ACube : public AActor
{
	GENERATED_BODY()
	
public:	
	ACube();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Front;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	const FVector& GetFrontLocation()const;

	UFUNCTION(BlueprintCallable)
	const FVector& GetLeftLocation()const;

	UFUNCTION(BlueprintCallable)
	void UpdateTransform();

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* Mesh;

private:
	FVector FrontLocation;
	FVector LeftLocation;
};