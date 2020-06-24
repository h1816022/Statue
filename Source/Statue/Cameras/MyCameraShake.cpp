// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCameraShake.h"

UMyCameraShake::UMyCameraShake()
{
	OscillationDuration = 1.0f;

	RotOscillation.Pitch.Amplitude = 1.0f;
	RotOscillation.Pitch.Frequency = 100.0f;
}