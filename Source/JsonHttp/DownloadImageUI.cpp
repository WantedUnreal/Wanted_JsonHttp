// Fill out your copyright notice in the Description page of Project Settings.


#include "DownloadImageUI.h"

#include "Blueprint/AsyncTaskDownloadImage.h"
#include "Components/Image.h"
#include "Engine/Texture2DDynamic.h"

void UDownloadImageUI::StartDownload(FString url)
{
	UAsyncTaskDownloadImage* async = UAsyncTaskDownloadImage::DownloadImage(url);
	async->OnSuccess.AddDynamic(this, &UDownloadImageUI::OnDownloadComplete);
	async->OnFail.AddDynamic(this, &UDownloadImageUI::OnDownloadComplete);
}

void UDownloadImageUI::OnDownloadComplete(class UTexture2DDynamic* texture)
{
	if (texture)
	{
		downloadImage->SetBrushFromTextureDynamic(texture);
		float ratio = 800.0f / texture->GetSurfaceWidth();   // ex)  가로 1600, 세로 500 ratio = 0.5
		downloadImage->SetDesiredSizeOverride(FVector2D(texture->GetSurfaceWidth(), texture->GetSurfaceHeight()) * ratio);
		//downloadImage->SetBrushSize(FVector2D(texture->GetSurfaceWidth(), texture->GetSurfaceHeight()) * ratio);
	}
	else
	{
		
	}
}
