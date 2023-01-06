// Fill out your copyright notice in the Description page of Project Settings.


#include "KillZone.h"
#include "Components/BoxComponent.h"
#include "PlayerFlight.h"
#include "BossActor.h"

// Sets default values
AKillZone::AKillZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f, 2000.0f, 50.0f));
	boxComp->SetCollisionProfileName(TEXT("KillZonePreset"));
}

// Called when the game starts or when spawned
void AKillZone::BeginPlay()
{
	Super::BeginPlay();


	//오버랩 이벤트를 활성화한다.
	boxComp->SetGenerateOverlapEvents(true);
	//오버랩 이벤트 델리게이트의 오버랩 함수를 연결한다.
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AKillZone::OnOverlap);
}

// Called every frame
void AKillZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKillZone::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//플레이어를 제외한 부딪힌 모든 대상을 제거한다.
	//플레이어 플라이트로 캐스팅 해봤을때 플레이어 플라이트가 아니면 널포인터 값을 반환함
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);
	ABossActor* boss = Cast<ABossActor>(OtherActor);

	//플레이어 값이 널포인터가 아니라면 -> 플레이어란 뜻임으로 제거를 하지 않음.
	if (player != nullptr)
	{

	}
	else if(boss != nullptr)
	{
		
	}
	//플레이어 값이 널포인터가 아니지 않다면 = 널포인터라면, 플레이어가 아니므로 제거함.
	else {
		OtherActor->Destroy();
	}
}

