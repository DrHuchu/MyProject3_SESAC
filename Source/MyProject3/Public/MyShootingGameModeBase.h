// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyShootingGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT3_API AMyShootingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public :

	UPROPERTY(EditAnywhere, Category= "MyDefaultSetting")
	TSubclassOf<class UMainWidget> mainWidget;

	UPROPERTY(EditAnywhere, Category = "MyDefaultSetting")
	TSubclassOf<class UMenuWidget> menuWidget;

	TArray<class AEnemy*> enemies;

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetCurrentScore() { return currentScore;}

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetBestScore() { return bestScore;}

	virtual void BeginPlay() override;

	void AddScore(int32 count);

	void ShowMenu();
	
	UFUNCTION()
	void BossTimer();

	UFUNCTION()
	void StopAllSpawn();

	class AEnemySpawningPool* spawner;

	int32 currentScore = 0;
	int32 bestScore = 0;

	UPROPERTY(EditAnywhere, Category = "Boss Settings")
	TSubclassOf<class ABossActor> bossFactory;

	UPROPERTY(EditAnywhere, Category = "Boss Settings")
	TSubclassOf<class AEnemy> enemyFactory;

private:
	
	bool bIsAppearBoss = false;


	class UMainWidget* main_UI;
	class UMenuWidget* menu_UI;
	FString filePath = FString("D:/unreal project/MyProject3/Content/SaveScore/BestScore.txt");
};


