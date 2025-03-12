// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DownloadImageUI.generated.h"

/**
 * 
 */
UCLASS()
class JSONHTTP_API UDownloadImageUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UImage* downloadImage;

	// 다운로드 시작
	void StartDownload(FString url);
	// 다운로드 완료시 호출되는 함수
	UFUNCTION()
	void OnDownloadComplete(class UTexture2DDynamic* texture);
};
