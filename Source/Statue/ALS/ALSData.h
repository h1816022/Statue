// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ALSData.generated.h"

class UCurveVector;
class UCurveFloat;

/**
 * 
 */
USTRUCT(BlueprintType)
struct STATUE_API FMovementSettings
{
	GENERATED_BODY()
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WalkSpeed;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RunSpeed;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SprintSpeed;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HoverSpeed;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FlyingSpeed;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCurveVector* MovementCurve;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCurveFloat* RotationRateCurve;
};
