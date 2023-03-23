// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectCreator.generated.h"

USTRUCT()
struct FObjectInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	int32 type;

	UPROPERTY(EditAnywhere)
	class ADefaultObject* obj;

};
UCLASS()
class JASONSTUDY_API AObjectCreator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectCreator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CreateObject();
	void CreateObject(int32 type, FVector pos, float scale);
	void SaveObject();
	void LoadObject();
	void DeleteAll();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ADefaultObject> objectFactory;

	UPROPERTY(EditAnywhere)
	TArray<class UStaticMesh*> meshs;

	UPROPERTY(EditAnywhere)
	TArray<FObjectInfo> arrayObject; 
};
