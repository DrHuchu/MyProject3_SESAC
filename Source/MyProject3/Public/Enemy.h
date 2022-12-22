// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class MYPROJECT3_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemySettings")
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemySettings")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BulletSettings)
	float moveSpeed = 300.0f;

private:
	FVector direction;

};