#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

class USphereComponent;

UENUM(BlueprintType)
enum class EBallMoveType : uint8
{
	Grab,
	Physics,
	Fly
};

UCLASS()
class STATUE_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	ABall();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Bound(const FVector& NormalImpulse);

	UPROPERTY(BlueprintReadWrite)
	FVector Dir;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float Speed;

	UPROPERTY(BlueprintReadWrite)
	EBallMoveType NowType;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float InvincibleDelayTime;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* Col;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Sphere;

private:
	void PhysicsUpdate();
};
