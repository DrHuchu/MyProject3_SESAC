// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "MyShootingGameModeBase.h"

APlayerBullet::APlayerBullet()
{
	boxComp->SetCollisionProfileName(TEXT("BulletPreset"));
}

void APlayerBullet::BeginPlay()
{
	Super::BeginPlay();
	SetDirection(GetActorForwardVector());
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerBullet::OnOverlap);
}

void APlayerBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 만일 부딪힌 대상이 에너미라면
	// 아더액터를 에너미에 캐스팅이 성공한다면-> 포인터변수가 널포인터를 가리키지 않는다면
	AEnemy* enemy = Cast<AEnemy>(OtherActor);
	if (enemy != nullptr)


	{	//적이 있던 위치에 폭발 이펙트를 생성한다.
		FVector enemyLoc = enemy->GetActorLocation();
		FRotator enemyRot = enemy->GetActorRotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosion, enemyLoc, enemyRot, true);

		//적을 제거하고
		enemy->Destroy();

		//게임 모드에 점수를 1점 추가한다.
		AGameModeBase* gm = UGameplayStatics::GetGameMode(this);
		/*AGameModeBase* gm = GetWorld()->GetAuthGameMode();*/
		//위 두 문장은 같은 기능

		AMyShootingGameModeBase* myGM = Cast<AMyShootingGameModeBase>(gm);
		myGM->AddScore(1);

		//UE_LOG(LogTemp, Warning, TEXT("Point : %d"), myGM->GetCurrentScore());


	//나 자신도 제거한다.
		Destroy();
	}
}