// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "PlayerPawn.generated.h"

// 유저 정보 구조체
USTRUCT()
struct FUserInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	FString name;
	UPROPERTY(VisibleAnywhere)
	int32 age;
	UPROPERTY(VisibleAnywhere)
	float height;
	UPROPERTY(VisibleAnywhere)
	bool gender; // true : 여성, false : 남성
	UPROPERTY(VisibleAnywhere)
	TArray<FString> favoriteFood;
};

USTRUCT()
struct FUserInfoArray
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	TArray<FUserInfo> data;
};

// 모양 정보
USTRUCT()
struct FShapeInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FVector posistion;
	UPROPERTY()
	FRotator rotation;
	UPROPERTY()
	FVector scale;
	UPROPERTY()
	int32 type;
};

USTRUCT()
struct FShapeInfoArray
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FShapeInfo> data;
};


UCLASS()
class JSONHTTP_API APlayerPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere)
	FUserInfo myInfo;

	UPROPERTY(VisibleAnywhere)
	FUserInfoArray friendInfos;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AShapeActor> shapeFactory;

	UPROPERTY()
	TArray<class AShapeActor*> allShapeActor;
	
public:
	void CreateShape();
	FVector GetRandLocation();
	FRotator GetRandRotation();

	void SaveData();
	void LoadData();

};
