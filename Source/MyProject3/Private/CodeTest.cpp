// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeTest.h"

//생성자 함수, 변환자료형이 없음
// Sets default values
ACodeTest::ACodeTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACodeTest::BeginPlay()
{
	Super::BeginPlay();
// 	int32 result1 = Add(8, 4);
// 	UE_LOG(LogTemp, Warning, TEXT("result = % d"), result1);
// 
// 	int32 result2 = Subtract(3, 5);
// 	UE_LOG(LogTemp, Warning, TEXT("subtract result is = %d"), result2);
// 
// 	int32 result3 = Multiply(3, 5);
// 	UE_LOG(LogTemp, Warning, TEXT("multiply result = %d"), result3);
// 	
// 	float result4 = Divide(3, 4);
// 	UE_LOG(LogTemp, Warning, TEXT("divide result = %.2f"), result4);

	FString result = StringAppender(last, first);
	UE_LOG(LogTemp, Warning, TEXT("당신의 이름은 %s"), *result);

	int32 checker = 123;
	//만일 checker 변수의 값이 짝수라면
	//"even"이라고 출력한다.
	//그렇지 않고, 변수의 값이 홀수라면
	//"odd"라고 출력한다.

	if (checker % 2 == 0 )
	{
		UE_LOG(LogTemp, Warning, TEXT("Even"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Odd"));
	}





	//만일 나이가 30살 이상이면 "할배다"라고 출력하고 싶다.
	if (age >= 30)
	{
		UE_LOG(LogTemp, Warning, TEXT("할배다"));
	}
	//그렇지 않고, 만일 나이가 20살 이상이면 "꼰대군요"라고 출력한다.
	else if(age >= 20)
	{
		UE_LOG(LogTemp, Warning, TEXT("꼰대군요"));
	}
	//그렇지 않다면, "꼬맹이"라고 출력하고 싶다.
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("꼬맹이네"));
	}

	//만일 나이가 20살 이상이면서, 키가 160 이상인 경우에는 '환영'이라고 출력한다.
	if (age >= 20 || height >= 160)
	{
		UE_LOG(LogTemp, Warning, TEXT("환영"));
	}
	//그렇지 않으면 '외면'이라고 출력한다.
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("외면"));
	}

	//만일 국적이 "한국"이면서 나이가 20살 이상 40살 미만인 경우 "청년"이라고 출력한다. 그렇지 않으면 "청년 아님"이라고 출력한다.

	if (nation == TEXT("한국") && age >= 20 && age < 40)
	{
		UE_LOG(LogTemp, Warning, TEXT("청년"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("청년아님"));
	}

	// 반복문
	// "안녕하세요"라는 문자열을 10번 출력한다.
// 	for (int32 i = 0; i < 10; i++)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("%d"), i);
// 	}
// 
// 	int32 num = 3;
// 	int32 temp = 2;
// 	int32 sum = temp + num++;


	//구구단 7단 출력
// 	for (int32 i = 1; i < 10; i++)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("7 X %d = %d"), i, i*7);
// 	}

	//구구단 전체 출력
// 	for (int32 i = 2; i < 10; i++)
// 	{
// 		for (int32 j = 1; j < 10; i++)
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("%d X %d = %d"), i, j, i*j);
// 		}
// 	}

	// number 변수에 3을 넣는다.
	//number = 3;

	//number2 = number;
	//number2 = -7;


	fnumber1 = 3.14f;


	//number의 값을 output 콘솔 창에 출력한다.
// 	UE_LOG(LogTemp, Warning, TEXT("number is %d, number2 is %d "), number, number2);
//  UE_LOG(LogTemp, Warning, TEXT("float number is %.2f"), fnumber1);

// 	if (isStudent == true)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("그는 학생이다."));
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("그는 학생이 아니다."));
// 	}
/* 	UE_LOG(LogTemp, Warning, TEXT("Hello World! %s"), *name2);*/
// 
// 	UE_LOG(LogTemp, Error, TEXT("끝이지롱"))
 }

// Called every frame
void ACodeTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ACodeTest::Add(int32 num1, int32 num2)
{
	//to do
	return num1 + num2;
}

int32 ACodeTest::Subtract(int32 num3, int32 num4)
{
	return num3 - num4;
}

int32 ACodeTest::Multiply(int32 num5, int32 num6)
{
	return num5 * num6;
}

float ACodeTest::Divide(int32 num7, int32 num8)
{
	return (float)num7 / (float)num8;
}

FString ACodeTest::StringAppender(FString last2, FString first2)
{
	return last + first;
}