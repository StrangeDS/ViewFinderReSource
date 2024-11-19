// Copyright Epic Games, Inc. All Rights Reserved.

#include "ViewFinderReGameMode.h"
#include "ViewFinderReCharacter.h"
#include "UObject/ConstructorHelpers.h"

AViewFinderReGameMode::AViewFinderReGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
