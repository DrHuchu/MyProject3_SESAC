// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "PlayerFlight.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FULTbomb);

//1. 벡터를 인자로 넘겨받는 델리게이트 선언(FDirectionModifier)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDirectionModifier, FVector, newDir);



UCLASS()
class MYPROJECT3_API APlayerFlight : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerFlight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	float moveSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	TSubclassOf<class ABullet> bulletFactory;

	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	class UInputAction* IA_Horizontal;

	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	class UInputAction* IA_Vertical;

	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	class UInputAction* IA_Fire;

	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	class UInputAction* IA_Boost;

	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	class UInputAction* IA_BoostOff;

	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	class UInputAction* IA_Ult;

	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	class UInputMappingContext* IMC_MyMapping;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	int32 bulletCount = 3;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	float bulletSpacing = 150;


	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	class USoundBase* fireSound;

	void ReservationHitColor(float time);
	void changeOriginColor();

	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	float bulletAngle = 30.0f;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	bool isTrapOn = false;

	FULTbomb playerBomb;

	FDirectionModifier dirMod;

	class AMyShootingGameModeBase* gm;

private:
// 	UFUNCTION(BlueprintCallable)
// 	void Horizontal(float value);

	UFUNCTION(BlueprintCallable)
	void Horizontal(const FInputActionValue& value);

// 	UFUNCTION(BlueprintCallable)
// 	void Vertical(float value);

	UFUNCTION(BlueprintCallable)
	void Vertical(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable)
	void FireBullet();

	UFUNCTION(BlueprintCallable)
	void Boost();

	UFUNCTION(BlueprintCallable)
	void BoostOff();

	void Boosting();

	bool isBoosting = false;

	float moveSpeedOrigin = 500;

	//액션인풋의 경우 매개변수를 비워둬야 함
// 	UFUNCTION(BlueprintCallable)
// 	void FireBullet();

	float h;
	float v;
	FVector direction;

	FLinearColor initColor;
	FTimerHandle colorTimer;
	class UMaterialInstanceDynamic* dynamicMaterial;

	UFUNCTION()
	void ExplosionAll();
};
