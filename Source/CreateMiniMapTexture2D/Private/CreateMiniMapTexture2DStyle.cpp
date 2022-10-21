// Copyright Epic Games, Inc. All Rights Reserved.

#include "CreateMiniMapTexture2DStyle.h"
#include "CreateMiniMapTexture2D.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FCreateMiniMapTexture2DStyle::StyleInstance = nullptr;

void FCreateMiniMapTexture2DStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FCreateMiniMapTexture2DStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FCreateMiniMapTexture2DStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("CreateMiniMapTexture2DStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FCreateMiniMapTexture2DStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("CreateMiniMapTexture2DStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("CreateMiniMapTexture2D")->GetBaseDir() / TEXT("Resources"));

	Style->Set("CreateMiniMapTexture2D.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FCreateMiniMapTexture2DStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FCreateMiniMapTexture2DStyle::Get()
{
	return *StyleInstance;
}
