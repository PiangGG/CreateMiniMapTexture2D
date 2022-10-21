// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "CreateMiniMapTexture2DStyle.h"

class FCreateMiniMapTexture2DCommands : public TCommands<FCreateMiniMapTexture2DCommands>
{
public:

	FCreateMiniMapTexture2DCommands()
		: TCommands<FCreateMiniMapTexture2DCommands>(TEXT("CreateMiniMapTexture2D"), NSLOCTEXT("Contexts", "CreateMiniMapTexture2D", "CreateMiniMapTexture2D Plugin"), NAME_None, FCreateMiniMapTexture2DStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
