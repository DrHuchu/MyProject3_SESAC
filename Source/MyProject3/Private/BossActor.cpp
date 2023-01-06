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

// 	���� ��ġ���� (0,0,200) ��ġ���� �̵��Ѵ�.
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
	//angle �������� count ����ŭ �Ѿ��� �����Ѵ�.

	
	for(int32 i = 1; i <= count; i++)
	{
	float rot = -90 - (i - 1) * angle + angle * (count - 1) * 0.5f;
	FRotator bulletRot = FRotator(rot, 90,0);
	FVector bulletPos = GetActorLocation();
	AEnemyBullet* enemyBullet = GetWorld()->SpawnActor<AEnemyBullet>(bullet, bulletPos, bulletRot);

	enemyBullet->SetActorLocation(enemyBullet->GetActorLocation() + enemyBullet->GetActorForwardVector() * 300);
	}

	//���� ���� = 270 - angle * (count - 1) * 0.5f
// 	float startAngle = 270 - (angle * (count -1) * 0.5f);
// 
// 	for (int32 i = 0; i < count; i++)
// 	{
// 		
// 		//�⺻ ��ǥ (0, r cos@, r sin@)
// 		FVector bullet_base = FVector(0, 1000* FMath::Cos(FMath::DegreesToRadians(startAngle + angle * i)), 1000* FMath::Sin(FMath::DegreesToRadians(startAngle + angle * i)));
// 		
// 		AEnemyBullet* enemyBullet = GetWorld()->SpawnActor<AEnemyBullet>(bullet, GetActorLocation() + bullet_base, FRotator(-90,0,0));
// 		//enemyBullet->AddActorLocalRotation(FRotator(0, startAngle + angle * i, 0));
// 
// 		//������ �븻������ ���� �ʰ� �븻������ �� ���� �������� �Լ� GetSafeNormal
// 		//enemyBullet->SetDirection(bullet_base.GetSafeNormal());
// 
// 		//Up Vector ���� �������ѳ��� bullet_base �������� ȸ�������� ���� �������� ���� ����Ѵ�.
// 		FRotator rot = UKismetMathLibrary::MakeRotFromZX(bullet_base.GetSafeNormal(), enemyBullet->GetActorUpVector());
// 
// 		enemyBullet->SetActorRotation(rot);
// 		enemyBullet->SetDirection(enemyBullet->GetActorForwardVector());
// 
// 	}

}
