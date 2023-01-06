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
	//���� �������Ʈ�� �����Ѵ�.
	main_UI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);

	if (main_UI != nullptr)

		//������ ������ ����Ʈ�� �׸���.
	{
		main_UI->AddToViewport();

		//�ְ������� ���Ͽ��� �о�´�.
		FString scoreText;
		bool isLoaded = FFileHelper::LoadFileToString(scoreText, *filePath);

		
/*		UE_LOG(LogTemp, Warning, TEXT("%s"), isLoaded ? TEXT("Success") : TEXT("Failure"));*/
		

		//�о�� ���� bestScore ������ �ִ´�.
		bestScore = FCString::Atoi(*scoreText);

		//���� ������ ������ curScore �ؽ�Ʈ ��Ͽ� �ݿ��Ѵ�.
		main_UI->PrintCurrentScore();
	}

	/*FString testPath = FPaths::GetProjectFilePath();*/
	FString testPath = FPaths::ProjectContentDir() + TEXT("SaveScore/SaveScore.txt");
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *testPath);
}


void AMyShootingGameModeBase::AddScore(int32 count)
{
	currentScore += count;

	//���� ������ ������ �ְ� �������� ũ�ٸ� 
	if (currentScore > bestScore)
	{
		//�ְ� ������ ���� ������ �ִ´�.
		bestScore = currentScore;

		//�ְ� ������ ���Ϸ� �����Ѵ�.
		bool isSaved = FFileHelper::SaveStringToFile(FString::FromInt(bestScore), *filePath);

		/*UE_LOG(LogTemp, Warning, TEXT("%s"), isSaved ? TEXT("True") : TEXT("False"));*/
	}
	//����, ���� ������ 30�� �̻��̸�
	if (currentScore >= 30 && !bIsAppearBoss)
	{
		bIsAppearBoss = true;
		UE_LOG(LogTemp, Warning, TEXT("boss on"));
		//4�� �ڿ� ������ �����Ѵ�.
	
		FTimerHandle bossTimer;
		GetWorld()->GetTimerManager().SetTimer(bossTimer, this, &AMyShootingGameModeBase::BossTimer, 2, false);

		StopAllSpawn();
	}

	//���� ������ ������ curScore �ؽ�Ʈ ��Ͽ� �ݿ��Ѵ�.
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
	//������ �Ͻ� ���� ���·� �����.
	//1. ���� ������ �ð� �帧�� 0���� �����.
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.01);

	//2. �Ͻ����� �Լ��� ȣ���Ѵ�.
	//UGameplayStatics::SetGamePaused(GetWorld(), true);

	//���콺 Ŀ���� ���̰� �Ѵ�.
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

