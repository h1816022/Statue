// Copyright Epic Games, Inc. All Rights Reserved.

#include "StatueGameMode.h"
#include "Players/PlayerCharacter.h"

AStatueGameMode::AStatueGameMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
}
