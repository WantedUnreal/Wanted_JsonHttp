// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShapeActor.generated.h"

UCLASS()
class JSONHTTP_API AShapeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShapeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMesh;

	// 여러 모양
	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> allShape;

	// 몇 번째 모양
	int32 type;

public:
	void Init(int32 index);
};
