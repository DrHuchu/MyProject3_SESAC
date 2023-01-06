// Fill out your copyright notice in the Description page of Project Settings.


#include "MyShootingGameModeBase.h"
#include "MainWidget.h"
#include "MenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "EnemySpawningPool.h"
#include "BossActor.h"
#include "EngineUtils.h"
#include "Enemy.h"


void AMyShootingGameModeBase::BeginPlay()
{
	//위젯 블루프린트를 생성한다.
	main_UI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);

	if (main_UI != nullptr)

		//생성된 위젯을 뷰포트에 그린다.
	{
		main_UI->AddToViewport();

		//최고점수를 파일에서 읽어온다.
		FString scoreText;
		bool isLoaded = FFileHelper::LoadFileToString(scoreText, *filePath);

		
/*		UE_LOG(LogTemp, Warning, TEXT("%s"), isLoaded ? TEXT("Success") : TEXT("Failure"));*/
		

		//읽어온 값을 bestScore 변수에 넣는다.
		bestScore = FCString::Atoi(*scoreText);

		//현재 점수를 위젯의 curScore 텍스트 블록에 반영한다.
		main_UI->PrintCurrentScore();
	}

	/*FString testPath = FPaths::GetProjectFilePath();*/
	FString testPath = FPaths::ProjectContentDir() + TEXT("SaveScore/SaveScore.txt");
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *testPath);
}


void AMyShootingGameModeBase::AddScore(int32 count)
{
	currentScore += count;

	//만일 누적된 점수가 최고 점수보다 크다면 
	if (currentScore > bestScore)
	{
		//최고 점수에 현재 점수를 넣는다.
		bestScore = currentScore;

		//최고 점수를 파일로 저장한다.
		bool isSaved = FFileHelper::SaveStringToFile(FString::FromInt(bestScore), *filePath);

		/*UE_LOG(LogTemp, Warning, TEXT("%s"), isSaved ? TEXT("True") : TEXT("False"));*/
	}
	//만일, 현재 점수가 30점 이상이면
	if (currentScore >= 30 && !bIsAppearBoss)
	{
		bIsAppearBoss = true;
		UE_LOG(LogTemp, Warning, TEXT("boss on"));
		//4초 뒤에 보스를 생성한다.
	
		FTimerHandle bossTimer;
		GetWorld()->GetTimerManager().SetTimer(bossTimer, this, &AMyShootingGameModeBase::BossTimer, 2, false);

		StopAllSpawn();
	}

	//현재 점수를 위젯의 curScore 텍스트 블록에 반영한다.
	if(main_UI != nullptr)
	{
		main_UI->PrintCurrentScore();
	}
}


void AMyShootingGameModeBase::ShowMenu()
{
	menu_UI = CreateWidget<UMenuWidget>(GetWorld(), menuWidget);

	if (menu_UI != nullptr)
	{
		menu_UI->AddToViewport();

	}
	//게임을 일시 정지 상태로 만든다.
	//1. 직접 게임의 시간 흐름을 0으로 만든다.
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.01);

	//2. 일시정지 함수를 호출한다.
	//UGameplayStatics::SetGamePaused(GetWorld(), true);

	//마우스 커서를 보이게 한다.
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void AMyShootingGameModeBase::BossTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("boss appearrrrr"));
	GetWorld()->SpawnActor<ABossActor>(bossFactory, FVector(0,0,780), FRotator(0,0,0));
}

void AMyShootingGameModeBase::StopAllSpawn()
{
	for (TActorIterator<AEnemySpawningPool> pool(GetWorld()); pool; ++pool)
	{
		pool->SetActorTickEnabled(false);
	}
}

