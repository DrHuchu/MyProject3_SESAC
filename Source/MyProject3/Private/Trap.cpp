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
	
	//Overlap 델리게이트에 InTrap 함수를 바인딩
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATrap::InTrap);
	sphereComp->OnComponentEndOverlap.AddDynamic(this, &ATrap::OutTrap);
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentTime += DeltaTime;
	//현재 누적 시간(currentTime)이 지정된 시간(teleportTime)에 도달했다면
	if(currentTime >= teleportTime)
	{ 
		//랜덤한 위치(벡터)를 설정한다.
		float drawVectorY = FMath::RandRange(-810.0f, 810.0f);
		float drawVectorZ = FMath::RandRange(-370.0f, 370.0f);
		FVector teleportPos = FVector(0, drawVectorY, drawVectorZ);

		//그 위치로 이동한다.
		SetActorLocation(teleportPos);

		//currentTime을 다시 초기화한다.
		currentTime = 0;
	}

}

//InTrap 함수 구현
void ATrap::InTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//오버랩된 액터가 플레이어일 경우
	player = Cast<APlayerFlight>(OtherActor);
	if (player != nullptr)
	{
		//플레이어 클래스에 있는 isTrapOn을 참으로 변환
		player->isTrapOn = true;
	}
}

void ATrap::OutTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	player = Cast<APlayerFlight>(OtherActor);
	if (player != nullptr)
	{
		//플레이어 클래스에 있는 isTrapOn을 거짓으로 변환
		player->isTrapOn = false;

	}
}
