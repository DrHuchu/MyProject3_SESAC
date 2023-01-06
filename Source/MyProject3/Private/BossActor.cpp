// Fill out your copyright notice in the Description page of Project Settings.


#include "BossActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyBullet.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABossActor::ABossActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f, 400.0f, 150.0f));
	boxComp->SetGenerateOverlapEvents(true);
	boxComp->SetCollisionProfileName(TEXT("BossPreset"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeScale3D(FVector(1, 8, 3));

}

// Called when the game starts or when spawned
void ABossActor::BeginPlay()
{
	Super::BeginPlay();
	
	startLoc = GetActorLocation();
}

// Called every frame
void ABossActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

// 	현재 위치에서 (0,0,200) 위치까지 이동한다.
			float distance = (startLoc - GetActorLocation()).Length();
			float shaking = FMath::RandRange(-0.5f, 0.5f);
		
			if (distance < 530)
			{
				FVector dir = GetActorUpVector() * -1;
				dir.Y=shaking;
				SetActorLocation(GetActorLocation() + dir * 100 * DeltaTime);
			}
			else
			{
				SetActorLocation(FVector(0,0,250));
			}

// 		testValue += DeltaTime * 0.3f;
// 		testValue = FMath::Clamp(testValue, 0.0f, 1.0f);
// 
// 		FVector result = FMath::Lerp(startLoc, endLoc, testValue);
// 		SetActorLocation(result);

		if (currentTime > pattern_delay)
		{
			BossAttack1(15, 8);
			currentTime = 0;
		}
		else
		{
			currentTime += DeltaTime;
		}
}

void ABossActor::BossAttack1(float angle, int32 count)
{
	//angle 간격으로 count 수만큼 총알을 생성한다.

	
	for(int32 i = 1; i <= count; i++)
	{
	float rot = -90 - (i - 1) * angle + angle * (count - 1) * 0.5f;
	FRotator bulletRot = FRotator(rot, 90,0);
	FVector bulletPos = GetActorLocation();
	AEnemyBullet* enemyBullet = GetWorld()->SpawnActor<AEnemyBullet>(bullet, bulletPos, bulletRot);

	enemyBullet->SetActorLocation(enemyBullet->GetActorLocation() + enemyBullet->GetActorForwardVector() * 300);
	}

	//시작 각도 = 270 - angle * (count - 1) * 0.5f
// 	float startAngle = 270 - (angle * (count -1) * 0.5f);
// 
// 	for (int32 i = 0; i < count; i++)
// 	{
// 		
// 		//기본 좌표 (0, r cos@, r sin@)
// 		FVector bullet_base = FVector(0, 1000* FMath::Cos(FMath::DegreesToRadians(startAngle + angle * i)), 1000* FMath::Sin(FMath::DegreesToRadians(startAngle + angle * i)));
// 		
// 		AEnemyBullet* enemyBullet = GetWorld()->SpawnActor<AEnemyBullet>(bullet, GetActorLocation() + bullet_base, FRotator(-90,0,0));
// 		//enemyBullet->AddActorLocalRotation(FRotator(0, startAngle + angle * i, 0));
// 
// 		//원본을 노말라이즈 하지 않고 노말라이즈 된 값만 가져오는 함수 GetSafeNormal
// 		//enemyBullet->SetDirection(bullet_base.GetSafeNormal());
// 
// 		//Up Vector 축을 고정시켜놓고 bullet_base 방향으로 회전시켰을 때의 로테이터 값을 계산한다.
// 		FRotator rot = UKismetMathLibrary::MakeRotFromZX(bullet_base.GetSafeNormal(), enemyBullet->GetActorUpVector());
// 
// 		enemyBullet->SetActorRotation(rot);
// 		enemyBullet->SetDirection(enemyBullet->GetActorForwardVector());
// 
// 	}

}
