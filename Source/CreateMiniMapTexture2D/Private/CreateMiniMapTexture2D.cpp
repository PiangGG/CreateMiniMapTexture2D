// Copyright Epic Games, Inc. All Rights Reserved.

#include "CreateMiniMapTexture2D.h"
#include "CreateMiniMapTexture2DStyle.h"
#include "CreateMiniMapTexture2DCommands.h"
#include "Misc/MessageDialog.h"
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
	
	/*WorldPartition = World ? World->GetWorldPartition() : nullptr;
	if (WorldPartition)
	{
		UpdateWorldMiniMapDetails();
	}*/

	/*FString PackageName = TEXT("/Game/ProceduralTextures/");
	FString TextureName = FString("TextureName");
	PackageName += TextureName;
	UPackage* Package = CreatePackage(NULL, *PackageName);
	Package->FullyLoad();

	UTexture2D* NewTexture = NewObject<UTexture2D>(Package, *TextureName, RF_Public | RF_Standalone | RF_MarkAsRootSet);
	
	NewTexture->AddToRoot();				// This line prevents garbage collection of the texture
	NewTexture->PlatformData = new FTexturePlatformData();	// Then we initialize the PlatformData
	
	int32 TextureWidth = 2048.0f;
	int32 TextureHeight = 2048.0f;
	
	NewTexture->PlatformData->SizeX = TextureWidth;
	NewTexture->PlatformData->SizeY = TextureHeight;
	NewTexture->PlatformData->SetNumSlices(1);
	NewTexture->PlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;
	
	uint8* Pixels = new uint8[TextureWidth * TextureHeight * 4];
	for (int32 y = 0; y < TextureHeight; y++)
	{
		for (int32 x = 0; x < TextureWidth; x++)
		{
			int32 curPixelIndex = ((y * TextureWidth) + x);
			Pixels[4 * curPixelIndex] = 0.0f;
			Pixels[4 * curPixelIndex + 1] = 1.0f;
			Pixels[4 * curPixelIndex + 2] = 0.1f;
			Pixels[4 * curPixelIndex + 3] = 0.5f;
		}
	}
	// Allocate first mipmap.
	FTexture2DMipMap* Mip = new(NewTexture->PlatformData->Mips) FTexture2DMipMap();
	Mip->SizeX = TextureWidth;
	Mip->SizeY = TextureHeight;

	// Lock the texture so it can be modified
	Mip->BulkData.Lock(LOCK_READ_WRITE);
	uint8* TextureData = (uint8*) Mip->BulkData.Realloc(TextureWidth * TextureHeight * 4);
	FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * TextureHeight * TextureWidth * 4);
	Mip->BulkData.Unlock();

	NewTexture->Source.Init(TextureWidth, TextureHeight, 1, 1, ETextureSourceFormat::TSF_BGRA8, Pixels);

	NewTexture->UpdateResource();
	Package->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(NewTexture);

	FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	bool bSaved = UPackage::SavePackage(Package, NewTexture, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);

	delete[] Pixels;*/

	/*AWorldPartitionMiniMap* WorldMiniMap = FWorldPartitionMiniMapHelper::GetWorldPartitionMiniMap(World);
	if (WorldMiniMap)
	{
		WorldMiniMapBounds = FBox2D(FVector2D(WorldMiniMap->MiniMapWorldBounds.Min), FVector2D(WorldMiniMap->MiniMapWorldBounds.Max));
		if (WorldMiniMap->MiniMapTexture)
		{
			//WorldMiniMap->MiniMapTexture->Source.GetBytesPerPixel();
			FString PackageName = TEXT("/Game/ProceduralTextures/");
			FString TextureName = FString("TextureName");
			PackageName += TextureName;
			UPackage* Package = CreatePackage(NULL, *PackageName);
			Package->FullyLoad();

			UTexture2D* NewTexture = NewObject<UTexture2D>(Package, *TextureName, RF_Public | RF_Standalone | RF_MarkAsRootSet);
			//NewTexture = WorldMiniMap->MiniMapTexture;

			NewTexture->AddToRoot();				// This line prevents garbage collection of the texture
			NewTexture->PlatformData = new FTexturePlatformData();	// Then we initialize the PlatformData
	
			int32 TextureWidth = 2048.0f;
			int32 TextureHeight = 2048.0f;
	
			NewTexture->PlatformData->SizeX = TextureWidth;
			NewTexture->PlatformData->SizeY = TextureHeight;
			NewTexture->PlatformData->SetNumSlices(1);
			NewTexture->PlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;
			
			FTexture2DMipMap* Mip = new(NewTexture->PlatformData->Mips) FTexture2DMipMap();
			
			Mip->SizeX = TextureWidth;
			Mip->SizeY = TextureHeight;

			// Lock the texture so it can be modified
			Mip->BulkData.Lock(LOCK_READ_WRITE);
			Mip->BulkData.Unlock();

			uint8* Pixels = new uint8[TextureWidth * TextureHeight * 4];

			/*for (int32 y = 0; y < TextureHeight; y++)
			{
				for (int32 x = 0; x < TextureWidth; x++)
				{
					int32 curPixelIndex = ((y * TextureWidth) + x);
					Pixels[4 * curPixelIndex] = B;
					Pixels[4 * curPixelIndex + 1] = G;
					Pixels[4 * curPixelIndex + 2] = R;
					Pixels[4 * curPixelIndex + 3] = A;
				}
			}#1#
			
			NewTexture->Source.Init(TextureWidth, TextureHeight, 1, 1, ETextureSourceFormat::TSF_BGRA8,Pixels);
			
			NewTexture->UpdateResource();
			Package->MarkPackageDirty();
			FAssetRegistryModule::AssetCreated(NewTexture);

			FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
			bool bSaved = UPackage::SavePackage(Package, NewTexture, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);

			delete Pixels;
		}
	}*/

	//从UTexture2D读取数据
	AWorldPartitionMiniMap* WorldMiniMap = FWorldPartitionMiniMapHelper::GetWorldPartitionMiniMap(World);
	if (WorldMiniMap)
	{
		WorldMiniMapBounds = FBox2D(FVector2D(WorldMiniMap->MiniMapWorldBounds.Min), FVector2D(WorldMiniMap->MiniMapWorldBounds.Max));
		if (UTexture2D* MiniMapTexture = WorldMiniMap->MiniMapTexture)
		{
			//取得Package的指针
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
			UPackage* Package = CreatePackage(NULL, *AssetPath);
			Package->FullyLoad();

			//图片大小
			int32 TextureWidth = MiniMapTexture->GetSizeX();
			int32 TextureHeight = MiniMapTexture->GetSizeY();
			
			//取得UTexture2D的指针
			UTexture2D* NewTexture = NewObject<UTexture2D>(Package, *TextureName, RF_Public | RF_Standalone | RF_MarkAsRootSet);
			NewTexture->AddToRoot();            
			NewTexture->PlatformData = new FTexturePlatformData();  
			NewTexture->PlatformData->SizeX = TextureWidth;
			NewTexture->PlatformData->SizeY = TextureHeight;
			NewTexture->PlatformData->SetNumSlices(1);
			//设置像素格式
			NewTexture->PlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;
			
			//写入数据
			//创建一个uint8的数组并取得指针
			//这里需要考虑之前设置的像素格式
		

			TextureCompressionSettings OldCompressionSettings = MiniMapTexture->CompressionSettings; TextureMipGenSettings OldMipGenSettings = MiniMapTexture->MipGenSettings; bool OldSRGB = MiniMapTexture->SRGB;

			MiniMapTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
			MiniMapTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
			MiniMapTexture->SRGB = false;
			MiniMapTexture->UpdateResource();

			const FColor* FormatedImageData = static_cast<const FColor*>(MiniMapTexture->PlatformData->Mips[0].BulkData.LockReadOnly());

			//FColor FormatedImageData = FColor(255,255,255,128);
			uint8* Pixels = new uint8[TextureWidth * TextureHeight * 4];
			
			for(int32 x = 0; x < MiniMapTexture->GetSizeX(); x++)
			{
				for (int32 y = 0; y < MiniMapTexture->GetSizeY(); y++)
				{
					int32 curPixelIndex = ((y * MiniMapTexture->GetSizeX()) + x);
					
					FColor PixelColor = FormatedImageData[y * MiniMapTexture->GetSizeX() + x];
					//做若干操作

					Pixels[4 * curPixelIndex] =			PixelColor.R;
					Pixels[4 * curPixelIndex + 1] =		PixelColor.G;
					Pixels[4 * curPixelIndex + 2] =		PixelColor.B;
					Pixels[4 * curPixelIndex + 3] =		PixelColor.A;
				}
			}

			MiniMapTexture->PlatformData->Mips[0].BulkData.Unlock();

			NewTexture->CompressionSettings = OldCompressionSettings;
			NewTexture->MipGenSettings = OldMipGenSettings;
			NewTexture->SRGB = OldSRGB;
			NewTexture->VirtualTextureStreaming = false;
			NewTexture->UpdateResource();
			
			//创建第一个MipMap
			FTexture2DMipMap* Mip = new FTexture2DMipMap();
			NewTexture->PlatformData->Mips.Add(Mip);
			Mip->SizeX = TextureWidth;
			Mip->SizeY = TextureHeight;

			//锁定Texture让它可以被修改
			Mip->BulkData.Lock(LOCK_READ_WRITE);
			uint8* TextureData = (uint8*)Mip->BulkData.Realloc(TextureWidth * TextureHeight * 4);
			FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * TextureHeight * TextureWidth * 4);
			Mip->BulkData.Unlock();

			//通过以上步骤，我们完成数据的临时写入
			//执行完以下这两个步骤，编辑器中的asset会显示可以保存的状态（如果是指定Asset来获取UTexture2D的指针的情况下）
			NewTexture->Source.Init(TextureWidth, TextureHeight, 1, 1, ETextureSourceFormat::TSF_BGRA8, Pixels);
			NewTexture->UpdateResource();

			Package->MarkPackageDirty();
			FAssetRegistryModule::AssetCreated(NewTexture);
			//通过asset路径获取包中文件名
			FString PackageFileName = FPackageName::LongPackageNameToFilename(AssetPath, FPackageName::GetAssetPackageExtension());
			//进行保存
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