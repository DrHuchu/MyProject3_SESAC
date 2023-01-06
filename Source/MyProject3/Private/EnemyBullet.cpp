// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerFlight.h"
#include "Kismet/GameplayStatics.h"
#include "MyShootingGameModeBase.h"

AEnemyBullet::AEnemyBullet()
{
	boxComp->SetCollisionProfileName("EnemyPreset");
}

void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OnOverlap);
}

void AEnemyBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//부딪힌 대상이 플레이어라면
	APlayerFlight* PlayerFlight = Cast<APlayerFlight>(OtherActor);
	if (PlayerFlight != nullptr)

		//적을 제거하고
	{
		//PlayerFlight->ReservationHitColor(0.2f);
		PlayerFlight->Destroy();

		//메뉴 위젯을 생성하는 함수를 실행한다.
		AMyShootingGameModeBase* gm = Cast<AMyShootingGameModeBase>(GetWorld()->GetAuthGameMode());

		if (gm != nullptr)
		{
			gm->ShowMenu();
		}

		//나 자신도 제거한다.
		Destroy();

	}
}
