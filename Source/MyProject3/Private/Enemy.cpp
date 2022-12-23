// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerFlight.h"
#include "EngineUtils.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f));

	//Collision Enabled 값을 Query and Physics로 설정한다.
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//응답 채널의 Object Type(Collision Channel)을 Enemy로 설정한다. 
	//프로젝트 경로의 config 폴더의 DefaultEngine.ini를 열어서 볼 수 있음.
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel2);

	//응답 채널을 일괄적으로 Ignore 설정
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	//응답 채널을 Player와 Bullet에 대해서만 Overlap으로 처리한다.
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);



	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//추첨을 해서 하나는 정면 방향, 또 다른 하나는 플레이어 방향으로 이동하고 싶다
	//1. 추첨을 한다. 확률은 70:30 비율로 한다.
	int32 drawNumber = FMath::RandRange(1, 100);

	//2. 만일, 뽑은 숫자가 30보다 작거나 같으면 정면
	if (drawNumber <= traceRate)

		//2-1. 플레이어 액터 찾기
	{
		/*AActor* target = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerFlight::StaticClass());*/


		//월드에 있는 액터를 찾을때 주로 쓰는 방식
		
		for (TActorIterator<APlayerFlight> it(GetWorld()); it; ++it)
		{
			target = *it;
		}
		if (target != nullptr)
		{
			float temp = target->moveSpeed;
		}

		//2-2. 플레이어 방향 설정(가고자 하는 곳(플레이어)의 위치 - 나(에너미)의 위치)
		if (target != nullptr) {

			FVector targetDir = target->GetActorLocation() - GetActorLocation();
			//타겟 방향을 정규화 하지않으면 거리*속도로 이동해서 멀리 떨어질수록 그 이상으로 차이나는 거리로 감
			targetDir.Normalize();
			direction = targetDir;
		}
		
	}
	//3. 그렇지 않으면, 
	else {

		//3-1. 정면으로 방향 설정
		direction = GetActorForwardVector();
	}

	//Enemy가 오버랩 시작할때 함수를 연결
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);
	boxComp->SetGenerateOverlapEvents(true);

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*direction = FVector(0, 0, 1);*/
/*	direction = GetActorForwardVector();*/

	SetActorLocation(GetActorLocation() + direction * moveSpeed * DeltaTime);
}

void AEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 만일 부딪힌 대상이 플레이어라면
	// 아더액터를 플레이어에 캐스팅이 성공한다면-> 포인터변수가 널포인터를 가리키지 않는다면
	APlayerFlight* PlayerFlight = Cast<APlayerFlight>(OtherActor);
	if (PlayerFlight != nullptr)

		//적을 제거하고
	{
		PlayerFlight->Destroy();


		//나 자신도 제거한다.
		Destroy();

	}
}

