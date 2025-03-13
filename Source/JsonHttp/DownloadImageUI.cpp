// Fill out your copyright notice in the Description page of Project Settings.


#include "DownloadImageUI.h"

#include "Blueprint/AsyncTaskDownloadImage.h"
#include "Components/Image.h"
#include "Engine/Texture2DDynamic.h"
#include "http.h"
#include "ImageUtils.h"

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

void UDownloadImageUI::StartDownload2(FString url)
{
	// 서버에게 요청하는 객체 만들자.
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	// 요청 URL - 서버가 알려줌
	httpRequest->SetURL(url);
	// 요청 방식
	httpRequest->SetVerb(TEXT("GET"));
	// 헤더를 설정
	//httpRequest->SetHeader(TEXT("Content-type"), TEXT("image/png"));
	// 서버에게 요청을 한 후 응답이오면 호출되는 함수 등록
	
	httpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
	{
		// 응답이 오면 실행
		// 성공
		if (bProcessedSuccessfully)
		{
			TArray<uint8> texBites = Response->GetContent();
			UTexture2D* texture = FImageUtils::ImportBufferAsTexture2D(texBites);
			downloadImage->SetBrushFromTexture(texture);
			float ratio = 800.0f / texture->GetSurfaceWidth();   // ex)  가로 1600, 세로 500 ratio = 0.5
			downloadImage->SetDesiredSizeOverride(FVector2D(texture->GetSurfaceWidth(), texture->GetSurfaceHeight()) * ratio);
		}
		// 실패
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("통신 실패 : %d"), Response->GetResponseCode());
		}
	});

	// 요청을 보내자.
	httpRequest->ProcessRequest();
}
