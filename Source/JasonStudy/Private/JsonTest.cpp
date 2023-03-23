// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonTest.h"

// Sets default values
AJsonTest::AJsonTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJsonTest::BeginPlay()
{
	Super::BeginPlay();
	
	//SaveData();
	LoadData();
}

// Called every frame
void AJsonTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJsonTest::SaveData()
{
// 	FString userName = TEXT("이명제");
// 	int32 userAge = 20;
	TArray<FString> interests = { TEXT("축구"), TEXT("돈"), TEXT("건강") };
//	bool isMan = true;

	FUserInfo info;
	info.name = TEXT("이명제");
	info.age = 35;
	for (int32 i = 0; i < 3; i++)
	{
		info.interests.Add(interests[i]);
	}
	info.isMan = true;

	//변수에 담겨있는 데이터를 Json Object 로 변경한다
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject()); //TSharedPtr<FJsonObject> JsonObject 랑 FJsonObject* jonObject; 랑 같은거 앞에 쓴것이 메모리 관리를 자동으로 해줘서 안정성이 더 좋음
	jsonObject->SetStringField("userName", info.name);
	jsonObject->SetNumberField("userAge", info.age);
	jsonObject->SetBoolField("isMan", info.isMan);

	//FString 을 FJsonValue 로 변환해서 jsonArray 세팅
	TArray<TSharedPtr<FJsonValue>> arrayInterests;
	for (int32 i = 0; i < info.interests.Num(); i++)
	{
		arrayInterests.Add(MakeShareable(new FJsonValueString(info.interests[i])));
	}
	jsonObject->SetArrayField("interests", arrayInterests);


	//Json Object 를 문자열(string)로 변경
	FString jsonString;
	TSharedRef<TJsonWriter<>> jsonWrite = TJsonWriterFactory<>::Create(&jsonString);//TJsonWriter<> 꺽쇠안에 아무것도 써주지않으면 자동으로 char 값이 들어감
	if (FJsonSerializer::Serialize(jsonObject.ToSharedRef(), jsonWrite))
	{
		UE_LOG(LogTemp, Warning, TEXT("Save : %s"), *jsonString);

			//Json 형태로 변경된 문자열(jsonString)울 파일로 저장한다
			FString filePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("jsonTest.txt"));
		FFileHelper::SaveStringToFile(jsonString, *filePath);
	}
}

void AJsonTest::LoadData()
{
	FString jsonString;

	//파일을 읽어오자
	FString filePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("jsonTest.txt"));
	FFileHelper::LoadFileToString(jsonString, *filePath);

	UE_LOG(LogTemp, Warning, TEXT("Load : %s"), *jsonString);

	//string 을 jsonObject 로 변환
	TSharedPtr<FJsonObject> jsonObect;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonString);
	if (FJsonSerializer::Deserialize(jsonReader, jsonObect))
	{
		FString userName = jsonObect->GetStringField("userName");
		int32 userAge = jsonObect->GetNumberField("userAge");
		TArray<FString> interests;
		TArray<TSharedPtr<FJsonValue>> arrayInterests = jsonObect->GetArrayField("interests");
		for (int32 i = 0; i < arrayInterests.Num(); i++)
		{
			interests.Add(arrayInterests[i]->AsString());
		}
		bool isMan = jsonObect->GetBoolField("isMan");

		UE_LOG(LogTemp, Warning, TEXT("userName : %s"), *userName);
		UE_LOG(LogTemp, Warning, TEXT("userAge : %d"), userAge);
		UE_LOG(LogTemp, Warning, TEXT("isMan : %d"), isMan);
		for (int32 i = 0; i < interests.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("interests : %s"), *interests[i]);
		}
	}


}

