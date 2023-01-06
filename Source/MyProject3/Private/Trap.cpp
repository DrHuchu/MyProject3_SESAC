// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerFlight.h"

// Sets default values
ATrap::ATrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Trap Collision"));
	SetRootComponent(sphereComp);
	sphereComp->SetSphereRadius(200);
	
	sphereComp->SetCollisionProfileName("TrapPreset");

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trap Mesh"));
	meshComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();
	
	//Overlap ��������Ʈ�� InTrap �Լ��� ���ε�
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATrap::InTrap);
	sphereComp->OnComponentEndOverlap.AddDynamic(this, &ATrap::OutTrap);
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentTime += DeltaTime;
	//���� ���� �ð�(currentTime)�� ������ �ð�(teleportTime)�� �����ߴٸ�
	if(currentTime >= teleportTime)
	{ 
		//������ ��ġ(����)�� �����Ѵ�.
		float drawVectorY = FMath::RandRange(-810.0f, 810.0f);
		float drawVectorZ = FMath::RandRange(-370.0f, 370.0f);
		FVector teleportPos = FVector(0, drawVectorY, drawVectorZ);

		//�� ��ġ�� �̵��Ѵ�.
		SetActorLocation(teleportPos);

		//currentTime�� �ٽ� �ʱ�ȭ�Ѵ�.
		currentTime = 0;
	}

}

//InTrap �Լ� ����
void ATrap::InTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�������� ���Ͱ� �÷��̾��� ���
	player = Cast<APlayerFlight>(OtherActor);
	if (player != nullptr)
	{
		//�÷��̾� Ŭ������ �ִ� isTrapOn�� ������ ��ȯ
		player->isTrapOn = true;
	}
}

void ATrap::OutTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	player = Cast<APlayerFlight>(OtherActor);
	if (player != nullptr)
	{
		//�÷��̾� Ŭ������ �ִ� isTrapOn�� �������� ��ȯ
		player->isTrapOn = false;

	}
}
