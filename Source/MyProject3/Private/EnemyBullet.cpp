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
	//�ε��� ����� �÷��̾���
	APlayerFlight* PlayerFlight = Cast<APlayerFlight>(OtherActor);
	if (PlayerFlight != nullptr)

		//���� �����ϰ�
	{
		//PlayerFlight->ReservationHitColor(0.2f);
		PlayerFlight->Destroy();

		//�޴� ������ �����ϴ� �Լ��� �����Ѵ�.
		AMyShootingGameModeBase* gm = Cast<AMyShootingGameModeBase>(GetWorld()->GetAuthGameMode());

		if (gm != nullptr)
		{
			gm->ShowMenu();
		}

		//�� �ڽŵ� �����Ѵ�.
		Destroy();

	}
}
