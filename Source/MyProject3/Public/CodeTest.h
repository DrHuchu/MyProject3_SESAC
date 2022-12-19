// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CodeTest.generated.h"

UCLASS()
class MYPROJECT3_API ACodeTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACodeTest();

//접근 한정자, 접근 제어자 (access modifier)
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//정수형 변수 number를 선언한다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CodeVariable)
	int32 number;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 number2;

	//실수형 변수 fnumber를 선언
	float fnumber1 = 3.14f;
	float fnumber2 = 10;

	//문자열 변수 name을 선언
	FString name;
	// FString name = "";
	FString name2 = FString(TEXT("백준현"));

	//논리형 변수 isStudent를 선언한다.
	bool isStudent;
	//bool isStudent = false; 선언하지 않으면 기본값으로 false가 들어가 있음.
	//bool isStudent = true;
	//선언과 동시에 '초기화' 한다.



	int32 Add(int32 number1, int32 number2);
	int32 Subtract(int32 number1, int32 number2);
	int32 Multiply(int32 number1, int32 number2);
	float Divide(int32 number1, int32 number2);

	UPROPERTY(EditAnywhere, Category=CodeVariable)
	int32 age = 42;
	UPROPERTY(EditAnywhere, Category = CodeVariable)
	int32 height = 170;
	UPROPERTY(EditAnywhere, Category = CodeVariable)
	FString nation = TEXT("한국");


	//문자열 두 개를 받아서 문자열 한 개로 합쳐서 반환하는 함수를 만들고 싶다.
	UFUNCTION(BlueprintPure)
	FString StringAppender(FString last, FString first);
	UPROPERTY(EditAnywhere, Category = CodeVariable)
	FString last = FString(TEXT("백"));
	UPROPERTY(EditAnywhere, Category = CodeVariable)
	FString first = FString(TEXT("준현"));

	//만일 checker에 들어있는 값이 짝수라면, "Even Number"라고 출력한다. 그렇지 않고 
};
