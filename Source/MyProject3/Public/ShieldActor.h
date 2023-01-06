// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShieldActor.generated.h"

UCLASS()
class MYPROJECT3_API AShieldActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShieldActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BulletSettings)
	class UParticleSystem* explosion;

	bool isShieldOn = false;

	UPROPERTY(EditAnywhere, Category = ShieldSettings)
	float rotSpeed = 500.0f;

	UFUNCTION()
	void ShieldOverlapped (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ResetAttach();
};
