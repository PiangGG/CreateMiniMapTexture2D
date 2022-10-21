// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FCreateMiniMapTexture2DModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();


private:
	TSharedPtr<class FUICommandList> PluginCommands;
	
	UWorld* World;
	
	UWorldPartition* WorldPartition;

	FBox2D WorldMiniMapBounds;
public:
	void Start();

	void UpdateWorldMiniMapDetails();

public:
	FSlateBrush	WorldMiniMapBrush;
	
	UTexture2D* MyMiniMapTexture;
	
	
};
