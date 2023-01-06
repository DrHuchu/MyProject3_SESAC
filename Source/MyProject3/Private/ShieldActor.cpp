// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerFlight.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "MyShootingGameModeBase.h"

// Sets default values
AShieldActor::AShieldActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50, 150, 50));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeScale3D(FVector(1.0f, 3.0f, 1.0f));

}

// Called when the game starts or when spawned
void AShieldActor::BeginPlay()
{
	Super::BeginPlay();


	//플레이어랑 에너미 채널과만 충돌 응답(Overlap) 처리하기
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);

	//충돌시 실행할 함수 바인딩하기
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AShieldActor::ShieldOverlapped);
	
}

// Called every frame
void AShieldActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isShieldOn) {
		AddActorWorldRotation(FRotator(0,0,3));
		//this->SetActorRotation(GetActorRotation() + rotSpeed * FRotator(0,0,1) * DeltaTime);
	}
}

void AShieldActor::ShieldOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//부딪힌 대상이 플레이어라면
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);
	AEnemy* enemy = Cast<AEnemy>(OtherActor);

	if(player != nullptr)
	{ 
	//플레이어에게 부착된다.
		//AttachToActor(player, FAttachmentTransformRules::KeepWorldTransform);
		this->AttachToActor(player, FAttachmentTransformRules::KeepRelativeTransform);

		isShieldOn = true;

		FTimerHandle snapTimer;
		GetWorld()->GetTimerManager().SetTimer(snapTimer, this, &AShieldActor::ResetAttach, 3, false);
		

		
		
	}


	//부딪힌 대상이 에너미라면
	else if (enemy != nullptr) {

	//폭발 이펙트 추가
	FVector enemyLoc = enemy->GetActorLocation();
	FRotator enemyRot = enemy->GetActorRotation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosion, enemyLoc, enemyRot, true);

	//충돌한 에너미를 파괴한다.
	enemy->Destroy();

	//게임모드에 1점 추가
	AGameModeBase* gm = UGameplayStatics::GetGameMode(this);

	AMyShootingGameModeBase* myGM = Cast<AMyShootingGameModeBase>(gm);
	myGM->AddScore(1);
	}	
}

void AShieldActor::ResetAttach()
{
	//붙여놨던 부모를 해제한다.
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

