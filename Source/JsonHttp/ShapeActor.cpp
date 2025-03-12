// Fill out your copyright notice in the Description page of Project Settings.


#include "ShapeActor.h"

// Sets default values
AShapeActor::AShapeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	SetRootComponent(compMesh);
}

// Called when the game starts or when spawned
void AShapeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShapeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShapeActor::Init(int32 index)
{
	// type 설정
	type = index;
	// type 에 해당되는 StaticeMesh 를 compMesh 에 설정
	compMesh->SetStaticMesh(allShape[type]);
}
