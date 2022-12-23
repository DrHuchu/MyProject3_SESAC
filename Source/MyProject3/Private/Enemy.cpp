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

	//Collision Enabled ���� Query and Physics�� �����Ѵ�.
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//���� ä���� Object Type(Collision Channel)�� Enemy�� �����Ѵ�. 
	//������Ʈ ����� config ������ DefaultEngine.ini�� ��� �� �� ����.
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel2);

	//���� ä���� �ϰ������� Ignore ����
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	//���� ä���� Player�� Bullet�� ���ؼ��� Overlap���� ó���Ѵ�.
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);



	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//��÷�� �ؼ� �ϳ��� ���� ����, �� �ٸ� �ϳ��� �÷��̾� �������� �̵��ϰ� �ʹ�
	//1. ��÷�� �Ѵ�. Ȯ���� 70:30 ������ �Ѵ�.
	int32 drawNumber = FMath::RandRange(1, 100);

	//2. ����, ���� ���ڰ� 30���� �۰ų� ������ ����
	if (drawNumber <= traceRate)

		//2-1. �÷��̾� ���� ã��
	{
		/*AActor* target = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerFlight::StaticClass());*/


		//���忡 �ִ� ���͸� ã���� �ַ� ���� ���
		
		for (TActorIterator<APlayerFlight> it(GetWorld()); it; ++it)
		{
			target = *it;
		}
		if (target != nullptr)
		{
			float temp = target->moveSpeed;
		}

		//2-2. �÷��̾� ���� ����(������ �ϴ� ��(�÷��̾�)�� ��ġ - ��(���ʹ�)�� ��ġ)
		if (target != nullptr) {

			FVector targetDir = target->GetActorLocation() - GetActorLocation();
			//Ÿ�� ������ ����ȭ ���������� �Ÿ�*�ӵ��� �̵��ؼ� �ָ� ���������� �� �̻����� ���̳��� �Ÿ��� ��
			targetDir.Normalize();
			direction = targetDir;
		}
		
	}
	//3. �׷��� ������, 
	else {

		//3-1. �������� ���� ����
		direction = GetActorForwardVector();
	}

	//Enemy�� ������ �����Ҷ� �Լ��� ����
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
	// ���� �ε��� ����� �÷��̾���
	// �ƴ����͸� �÷��̾ ĳ������ �����Ѵٸ�-> �����ͺ����� �������͸� ����Ű�� �ʴ´ٸ�
	APlayerFlight* PlayerFlight = Cast<APlayerFlight>(OtherActor);
	if (PlayerFlight != nullptr)

		//���� �����ϰ�
	{
		PlayerFlight->Destroy();


		//�� �ڽŵ� �����Ѵ�.
		Destroy();

	}
}

