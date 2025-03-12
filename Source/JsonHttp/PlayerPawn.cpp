// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "http.h"
#include "JsonObjectConverter.h"
#include "ShapeActor.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	// 모든 유저의 정보
	TArray<FUserInfo> allUser;

	for (int32 i = 0; i < 10; i++)
	{
		// 나의 정보 입력
		FUserInfo info;
		info.name = FString::Printf(TEXT("%s_%d"), TEXT("김현진"), i);
		info.age = 27 + i;
		info.gender = (bool)(i % 2);
		info.height = 185.5f + i * 5;
		info.favoriteFood = { TEXT("치킨"), TEXT("김치찌개"), TEXT("오이무침") };

		allUser.Add(info);
	}

	// Json 을 만들기 위한 변수
	FUserInfoArray userInfoArray;
	userInfoArray.data = allUser;
	
	// info 데이터를 Json 형태로 변환
	FString jsonString;
	FJsonObjectConverter::UStructToJsonObjectString(userInfoArray, jsonString);
	UE_LOG(LogTemp, Warning, TEXT("나의 정보 : %s"), *jsonString);

	// jsonString 데이터를 FUserInfo 로 변환
	FJsonObjectConverter::JsonObjectStringToUStruct(jsonString, &friendInfos);
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One))
	{
		CreateShape();
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Two))
	{
		SaveData();
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Three))
	{
		LoadData();
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Four))
	{
		HttpGet();
	}
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerPawn::CreateShape()
{
	FShapeInfo info;
	// 랜덤 위치
	info.posistion = GetRandLocation();
	// 랜덤 회전
	info.rotation = GetRandRotation();
	// 랜덤 크기
	info.scale = FVector(FMath::RandRange(0.5f, 3.0f));
	// 랜덤 모양
	info.type = FMath::RandRange(0, 2);
	// 위 정보를 이용해서 모양을 만들자.
	CreateShape(info);
}

void APlayerPawn::CreateShape(FShapeInfo info)
{
	// ShapeActor 만들자.
	AShapeActor* shape = GetWorld()->SpawnActor<AShapeActor>(shapeFactory);
	
	// info 정보를 이용해서 ShpaeActor 에게 설정
	shape->SetActorLocation(info.posistion);
	shape->SetActorRotation(info.rotation);
	shape->SetActorScale3D(info.scale);
	shape->Init(info.type);

	allShapeActor.Add(shape);
}

FVector APlayerPawn::GetRandLocation()
{
	FVector pos;
	pos.X = FMath::RandRange(-500, 500);
	pos.Y = FMath::RandRange(-500, 500);
	pos.Z = FMath::RandRange(100, 500);
	
	// FBox range(FVector(-500, -500, 100), FVector(500));
	// pos = FMath::RandPointInBox(range);
	
	return pos;
}

FRotator APlayerPawn::GetRandRotation()
{
	FRotator rot;

	rot.Pitch = FMath::RandRange(0, 360);
	rot.Yaw = FMath::RandRange(0, 360);
	rot.Roll = FMath::RandRange(0, 360);

	return rot;
}

void APlayerPawn::SaveData()
{
	// 모든 ShapeInfo 가지는 변수
	TArray<FShapeInfo> allInfo;

	for (int32 i = 0; i < allShapeActor.Num(); i++)
	{
		FShapeInfo info;
		info.posistion = allShapeActor[i]->GetActorLocation();
		info.rotation = allShapeActor[i]->GetActorRotation();
		info.scale = allShapeActor[i]->GetActorScale3D();
		info.type = allShapeActor[i]->type;

		allInfo.Add(info);
	}

	FShapeInfoArray shapeInfoArray;
	shapeInfoArray.data = allInfo;

	// FShapeInfo 를 jsonValue 으로 변환
	FString jsonValue;
	FJsonObjectConverter::UStructToJsonObjectString(shapeInfoArray, jsonValue);

	// jsonValue 값을 file 로 저장
	FString path = FString::Printf(TEXT("%s%s"), *FPaths::ProjectDir(), TEXT("test.txt"));
	FFileHelper::SaveStringToFile(jsonValue, *path);
}

void APlayerPawn::LoadData()
{
	// test.txt 파일을 읽어오자
	FString path = FString::Printf(TEXT("%s%s"), *FPaths::ProjectDir(), TEXT("test.txt"));
	FString jsonString;
	FFileHelper::LoadFileToString(jsonString, *path);
	// 읽어온 JsonString 을 FShapeInfoArray 형태로 변환
	FShapeInfoArray shapeInfoArray;
	FJsonObjectConverter::JsonObjectStringToUStruct(jsonString, &shapeInfoArray);

	// shapeInfoArray.data 갯수 만큼 모양을 만들자.
	for (int32 i = 0; i < shapeInfoArray.data.Num(); i++)
	{
		CreateShape(shapeInfoArray.data[i]);
	}
}

void APlayerPawn::HttpGet()
{
	// 서버에게 요청하는 객체 만들자.
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	// 요청 URL - 서버가 알려줌
	httpRequest->SetURL(TEXT("https://jsonplaceholder.typicode.com/comments"));
	// 요청 방식
	httpRequest->SetVerb(TEXT("GET"));
	// 헤더를 설정
	httpRequest->SetHeader(TEXT("Content-type"), TEXT("application/json"));
	// 서버에게 요청을 한 후 응답이오면 호출되는 함수 등록
	
	httpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
	{
		//GetResponseCode : 200 - 성공, 400번대, 500번대 - 오류
		
		// 응답이 오면 실행
		// 성공
		if (bProcessedSuccessfully)
		{
			FString jsonString = FString::Printf(TEXT("{ \"data\" : %s}"), *Response->GetContentAsString());
			UE_LOG(LogTemp, Warning, TEXT("%s"), *jsonString);
			
			// 결과를 가지고 무언가 처리를 하자
			// jsonString --> FCommentInfoArray 로 변환
			FJsonObjectConverter::JsonObjectStringToUStruct(jsonString, &allCommnet);
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

void APlayerPawn::HttpPost()
{
	// 서버에게 요청하는 객체 만들자.
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	// 요청 URL - 서버가 알려줌
	httpRequest->SetURL(TEXT("https://jsonplaceholder.typicode.com/posts"));
	// 요청 방식
	httpRequest->SetVerb(TEXT("POST"));
	// 헤더를 설정
	httpRequest->SetHeader(TEXT("Content-type"), TEXT("application/json"));

	// 서버에게 보내고 싶은 데이터 값 (Json)
	FPostInfo info;
	info.title = TEXT("집에가고 싶다");
	info.body = TEXT("놀고 싶다");
	info.userId = 1234;
	FString jsonString;
	FJsonObjectConverter::UStructToJsonObjectString(&info, jsonString);
	
	// 서버에게 요청을 한 후 응답이오면 호출되는 함수 등록
	
	httpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
	{
		//GetResponseCode : 200 - 성공, 400번대, 500번대 - 오류
		
		// 응답이 오면 실행
		// 성공
		if (bProcessedSuccessfully)
		{
			FString jsonString = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("%s"), *jsonString);
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


// {
// 	"data" : [
// 		{
// 			"이름" : "김현진",
// 			"성별" : false,
// 			"나이" : 27,
// 			"키" : 185.5,
// 			"좋아하는 음식" : [ "치킨", "김치찌개", "오이무침"]
// 		},
// 		{
// 			"이름" : "김현진",
// 			"성별" : false,
// 			"나이" : 27,
// 			"키" : 185.5,
// 			"좋아하는 음식" : [ "치킨", "김치찌개", "오이무침"]
// 		},
// 		{
// 			"이름" : "김현진",
// 			"성별" : false,
// 			"나이" : 27,
// 			"키" : 185.5,
// 			"좋아하는 음식" : [ "치킨", "김치찌개", "오이무침"]
// 		}	
// 	]
// }
