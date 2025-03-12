// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"

#include "DownloadImageUI.h"
#include "Components/ScrollBox.h"

void UMainUI::AddDownloadImage(FString url)
{
	UDownloadImageUI* image = CreateWidget<UDownloadImageUI>(GetWorld(), donwnloadImageUIFactory);
	image->StartDownload(url);
	imageList->AddChild(image);
}
