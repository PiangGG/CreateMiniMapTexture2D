// Copyright Epic Games, Inc. All Rights Reserved.

#include "CreateMiniMapTexture2D.h"
#include "CreateMiniMapTexture2DStyle.h"
#include "CreateMiniMapTexture2DCommands.h"
#include "ToolMenus.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/GameplayStatics.h"
#include "WorldPartition/WorldPartitionMiniMap.h"
#include "WorldPartition/WorldPartitionMiniMapHelper.h"

static const FName CreateMiniMapTexture2DTabName("CreateMiniMapTexture2D");

#define LOCTEXT_NAMESPACE "FCreateMiniMapTexture2DModule"

void FCreateMiniMapTexture2DModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FCreateMiniMapTexture2DStyle::Initialize();
	FCreateMiniMapTexture2DStyle::ReloadTextures();

	FCreateMiniMapTexture2DCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FCreateMiniMapTexture2DCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FCreateMiniMapTexture2DModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCreateMiniMapTexture2DModule::RegisterMenus));
}

void FCreateMiniMapTexture2DModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FCreateMiniMapTexture2DStyle::Shutdown();

	FCreateMiniMapTexture2DCommands::Unregister();
}

void FCreateMiniMapTexture2DModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	/*FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FCreateMiniMapTexture2DModule::PluginButtonClicked()")),
							FText::FromString(TEXT("CreateMiniMapTexture2D.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);*/
	
	Start();
}

void FCreateMiniMapTexture2DModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FCreateMiniMapTexture2DCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCreateMiniMapTexture2DCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

void FCreateMiniMapTexture2DModule::Start()
{
	World = GWorld;

	//???UTexture2D????????????
	AWorldPartitionMiniMap* WorldMiniMap = FWorldPartitionMiniMapHelper::GetWorldPartitionMiniMap(World);
	if (WorldMiniMap)
	{
		WorldMiniMapBounds = FBox2D(FVector2D(WorldMiniMap->MiniMapWorldBounds.Min), FVector2D(WorldMiniMap->MiniMapWorldBounds.Max));
		if (UTexture2D* MiniMapTexture = WorldMiniMap->MiniMapTexture)
		{
			//??????Package?????????
			FString PackageName = TEXT("ProceduralTextures");
			FString TextureName;
			if (World)
			{
				TextureName = FString(UGameplayStatics::GetCurrentLevelName(GWorld));
			}
			else
			{
				TextureName = FString("TextureName");
			}
			
			FString AssetPath = TEXT("/Game/")+ PackageName+ TEXT("/");
			AssetPath += TextureName;
			UPackage* Package = CreatePackage( *AssetPath);
			Package->FullyLoad();

			//????????????
			int32 TextureWidth = MiniMapTexture->GetSizeX();
			int32 TextureHeight = MiniMapTexture->GetSizeY();
			
			//??????UTexture2D?????????
			UTexture2D* NewTexture = NewObject<UTexture2D>(Package, *TextureName, RF_Public | RF_Standalone | RF_MarkAsRootSet);
			NewTexture->AddToRoot();            
			NewTexture->SetPlatformData(new FTexturePlatformData());  
			NewTexture->GetPlatformData()->SizeX = TextureWidth;
			NewTexture->GetPlatformData()->SizeY = TextureHeight;
			NewTexture->GetPlatformData()->SetNumSlices(1);
			//??????????????????
			NewTexture->GetPlatformData()->PixelFormat = EPixelFormat::PF_B8G8R8A8;
			
			//????????????
			//????????????uint8????????????????????????
			//?????????????????????????????????????????????
		

			TextureCompressionSettings OldCompressionSettings = MiniMapTexture->CompressionSettings; TextureMipGenSettings OldMipGenSettings = MiniMapTexture->MipGenSettings; bool OldSRGB = MiniMapTexture->SRGB;

			MiniMapTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
			MiniMapTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
			MiniMapTexture->SRGB = false;
			MiniMapTexture->UpdateResource();

			const FColor* FormatedImageData = static_cast<const FColor*>(MiniMapTexture->GetPlatformData()->Mips[0].BulkData.LockReadOnly());

			//FColor FormatedImageData = FColor(255,255,255,128);
			uint8* Pixels = new uint8[TextureWidth * TextureHeight * 4];
			
			for(int32 x = 0; x < MiniMapTexture->GetSizeX(); x++)
			{
				for (int32 y = 0; y < MiniMapTexture->GetSizeY(); y++)
				{
					int32 curPixelIndex = ((y * MiniMapTexture->GetSizeX()) + x);
					
					FColor PixelColor = FormatedImageData[y * MiniMapTexture->GetSizeX() + x];
					//???????????????

					Pixels[4 * curPixelIndex] =			PixelColor.R;
					Pixels[4 * curPixelIndex + 1] =		PixelColor.G;
					Pixels[4 * curPixelIndex + 2] =		PixelColor.B;
					Pixels[4 * curPixelIndex + 3] =		PixelColor.A;
				}
			}

			MiniMapTexture->GetPlatformData()->Mips[0].BulkData.Unlock();

			NewTexture->CompressionSettings = OldCompressionSettings;
			NewTexture->MipGenSettings = OldMipGenSettings;
			NewTexture->SRGB = OldSRGB;
			NewTexture->VirtualTextureStreaming = false;
			NewTexture->UpdateResource();
			
			//???????????????MipMap
			FTexture2DMipMap* Mip = new FTexture2DMipMap();
			NewTexture->GetPlatformData()->Mips.Add(Mip);
			Mip->SizeX = TextureWidth;
			Mip->SizeY = TextureHeight;

			//??????Texture?????????????????????
			Mip->BulkData.Lock(LOCK_READ_WRITE);
			uint8* TextureData = (uint8*)Mip->BulkData.Realloc(TextureWidth * TextureHeight * 4);
			FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * TextureHeight * TextureWidth * 4);
			Mip->BulkData.Unlock();

			//??????????????????????????????????????????????????????
			//????????????????????????????????????????????????asset????????????????????????????????????????????????Asset?????????UTexture2D????????????????????????
			NewTexture->Source.Init(TextureWidth, TextureHeight, 1, 1, ETextureSourceFormat::TSF_BGRA8, Pixels);
			NewTexture->UpdateResource();

			Package->MarkPackageDirty();
			FAssetRegistryModule::AssetCreated(NewTexture);
			//??????asset???????????????????????????
			FString PackageFileName = FPackageName::LongPackageNameToFilename(AssetPath, FPackageName::GetAssetPackageExtension());
			//????????????
			bool bSaved = UPackage::SavePackage(Package, NewTexture, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);

			delete[] Pixels;
		}
	}
	
}

void FCreateMiniMapTexture2DModule::UpdateWorldMiniMapDetails()
{
	AWorldPartitionMiniMap* WorldMiniMap = FWorldPartitionMiniMapHelper::GetWorldPartitionMiniMap(World);
	if (WorldMiniMap)
	{
		WorldMiniMapBounds = FBox2D(FVector2D(WorldMiniMap->MiniMapWorldBounds.Min), FVector2D(WorldMiniMap->MiniMapWorldBounds.Max));
		if (UTexture2D* MiniMapTexture = WorldMiniMap->MiniMapTexture)
		{
			WorldMiniMapBrush.SetUVRegion(WorldMiniMap->UVOffset);
			WorldMiniMapBrush.SetImageSize(MiniMapTexture->GetImportedSize());
			WorldMiniMapBrush.SetResourceObject(MiniMapTexture);
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCreateMiniMapTexture2DModule, CreateMiniMapTexture2D)