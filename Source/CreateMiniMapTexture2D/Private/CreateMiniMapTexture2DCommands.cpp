// Copyright Epic Games, Inc. All Rights Reserved.

#include "CreateMiniMapTexture2DCommands.h"

#define LOCTEXT_NAMESPACE "FCreateMiniMapTexture2DModule"

void FCreateMiniMapTexture2DCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "CreateMiniMapTexture2D", "Execute CreateMiniMapTexture2D action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
