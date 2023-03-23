// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectCreator.h"
#include "DefaultObject.h"

// Sets default values
AObjectCreator::AObjectCreator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObjectCreator::BeginPlay()
{
	Super::BeginPlay();

	//Actor 에서 Input 등록
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	EnableInput(playerController);

	InputComponent->BindAction(TEXT("1"), IE_Pressed, this, &AObjectCreator::CreateObject);
	InputComponent->BindAction(TEXT("2"), IE_Pressed, this, &AObjectCreator::SaveObject);
	InputComponent->BindAction(TEXT("3"), IE_Pressed, this, &AObjectCreator::LoadObject);
}

// Called every frame
void AObjectCreator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObjectCreator::CreateObject()
{
	float pos_x = FMath::RandRange(-1000.0f, 1000.0f);
	float pos_y = FMath::RandRange(-1000.0f, 1000.0f);
	float pos_z = FMath::RandRange(0.0f, 1000.0f);
	int32 type = FMath::RandRange(0, meshs.Num() - 1);
	float scale = FMath::RandRange(0.1f, 2.0f);

	CreateObject(type, FVector(pos_x, pos_y, pos_z), scale);
}

void AObjectCreator::CreateObject(int32 type, FVector pos, float scale)
{
	ADefaultObject* obj = GetWorld()->SpawnActor<ADefaultObject>(objectFactory, pos, FRotator::ZeroRotator);

	obj->SetActorScale3D(FVector(scale));
	obj->SetInit(meshs[type]);

	FObjectInfo info;
	info.type = type;
	info.obj = obj;

	arrayObject.Add(info);
}

void AObjectCreator::SaveObject()
{
	//변수값(구조체)을 Json Object 로 변경
	TArray<TSharedPtr<FJsonValue>> jsonArray;
	for (int32 i = 0; i < arrayObject.Num(); i++)
	{
		TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());
		jsonObject->SetNumberField("type", arrayObject[i].type);
		jsonObject->SetNumberField("pos_x", arrayObject[i].obj->GetActorLocation().X);
		jsonObject->SetNumberField("pos_y", arrayObject[i].obj->GetActorLocation().Y);
		jsonObject->SetNumberField("pos_z", arrayObject[i].obj->GetActorLocation().Z);
		jsonObject->SetNumberField("scale", arrayObject[i].obj->GetActorScale3D().X);

		jsonArray.Add(MakeShareable(new FJsonValueObject(jsonObject)));
	}

	//Json Object 를 FString 으로 변경
	FString jsonString;
	TSharedRef<TJsonWriter<>> jsonWirter = TJsonWriterFactory<>::Create(&jsonString);
	if (FJsonSerializer::Serialize(jsonArray, jsonWirter))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *jsonString);

		//Json 형태로 변경된 문자열(jsonString)울 파일로 저장한다
		FString filePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("obj_info.txt"));
		FFileHelper::SaveStringToFile(jsonString, *filePath);
	}

	////키값을 datas 로 묶고싶을때
  //TSharedPtr<FJsonObject> j = MakeShareable(new FJsonObject());
  //j->SetArrayField("datas", jsonArray);
  //FJsonSerializer::Serialize(j->ToSharedRef(), jsonWirter);
}

void AObjectCreator::LoadObject()
{
	DeleteAll();

	FString jsonString;

	//파일을 읽어오자
	FString filePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("obj_info.txt"));
	FFileHelper::LoadFileToString(jsonString, *filePath);

	UE_LOG(LogTemp, Warning, TEXT("Load : %s"), *jsonString);

	//FString 을 Json Object 로 변환
	TArray<TSharedPtr<FJsonValue>> jsonArray;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonString);
	if (FJsonSerializer::Deserialize(jsonReader, jsonArray))
	{
		//오브젝트 생성
		for (int32 i = 0; i < jsonArray.Num(); i++)
		{
			TSharedPtr<FJsonObject> jsonObject = jsonArray[i]->AsObject();
			int32 type = jsonObject->GetNumberField("type");
			float pos_x = jsonObject->GetNumberField("pos_x");
			float pos_y = jsonObject->GetNumberField("pos_y");
			float pos_z = jsonObject->GetNumberField("pos_z");
			float scale = jsonObject->GetNumberField("scale");

			CreateObject(type, FVector(pos_x, pos_y, pos_z), scale);
		}
	}
}

void AObjectCreator::DeleteAll()
{
	for (int32 i = 0; i < arrayObject.Num(); i++)
	{
		arrayObject[i].obj->Destroy();
	}

	arrayObject.Empty();
}

