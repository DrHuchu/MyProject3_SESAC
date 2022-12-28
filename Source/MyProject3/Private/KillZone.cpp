// Fill out your copyright notice in the Description page of Project Settings.


#include "KillZone.h"
#include "Components/BoxComponent.h"
#include "PlayerFlight.h"

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


	//������ �̺�Ʈ�� Ȱ��ȭ�Ѵ�.
	boxComp->SetGenerateOverlapEvents(true);
	//������ �̺�Ʈ ��������Ʈ�� ������ �Լ��� �����Ѵ�.
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AKillZone::OnOverlap);
}

// Called every frame
void AKillZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKillZone::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�÷��̾ ������ �ε��� ��� ����� �����Ѵ�.
	//�÷��̾� �ö���Ʈ�� ĳ���� �غ����� �÷��̾� �ö���Ʈ�� �ƴϸ� �������� ���� ��ȯ��
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);

	//�÷��̾� ���� �������Ͱ� �ƴ϶�� -> �÷��̾�� �������� ���Ÿ� ���� ����.
	if (player != nullptr)
	{

	}
	//�÷��̾� ���� �������Ͱ� �ƴ��� �ʴٸ� = �������Ͷ��, �÷��̾ �ƴϹǷ� ������.
	else {
		OtherActor->Destroy();
	}
}
