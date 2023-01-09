// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFlight.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Bullet.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "EngineUtils.h"
#include "MyShootingGameModeBase.h"
#include "DrawDebugHelpers.h"

// Sets default values
APlayerFlight::APlayerFlight()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//�浹 �ڽ� ������Ʈ�� �����Ѵ�.
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));

	//������ �浹 �ڽ� ������Ʈ�� ��Ʈ ������Ʈ�� �����Ѵ�.
	SetRootComponent(boxComp);

	//�ڽ� �ݸ����� ũ�⸦ ����*����*���� ��� 50cm�� �����Ѵ�
	boxComp->SetBoxExtent(FVector(50.0f));
	//���� ���� XYZ��� ������ �� �ϳ��� �Է����൵ �ȴ�

	//�ڽ� �ݸ����� �浹 ó�� �������� "PlayerPreset"���� �����Ѵ�
	boxComp->SetCollisionProfileName(TEXT("PlayerPreset"));


	//�޽� ������Ʈ�� ����
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	//�޽� ������Ʈ�� ��Ʈ ������Ʈ�� ���� ��ü�� ����Ѵ� boxComp��� ��� �ǰ�, RootComponent��� ��� �˾Ƽ� ã����
	meshComp->SetupAttachment(RootComponent);

	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//�޽� ������Ʈ�� static mesh �׸� ť�� ������ �Ҵ��Ѵ�.
	ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));

	//���� ť�� ������ �ε��ϴµ� �����ߴٸ�
	if (cubeMesh.Succeeded())
	{
		//�ε��� ������ �޽� ������Ʈ�� static mesh �׸� �ִ´�.
		meshComp->SetStaticMesh(cubeMesh.Object);
	}
}

// Called when the game starts or when spawned
void APlayerFlight::BeginPlay()
{
	Super::BeginPlay();

	
	//�÷��̾� ��Ʈ�ѷ��� ĳ�����Ѵ�.
	APlayerController* PlayerCon = Cast<APlayerController>(GetController());

	if (PlayerCon != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerCon->GetLocalPlayer());

		if (subsys != nullptr)
		{
			subsys->AddMappingContext(IMC_MyMapping, 0);
		}
	}
	
	//���� ���� ���� �����Ѵ�.
	UMaterialInterface* iMat = meshComp->GetMaterial(0);
	FHashedMaterialParameterInfo param = FHashedMaterialParameterInfo(TEXT("Param"));

	//Material Instance���� ���� �Ķ���� ���� initColor ������ �����Ѵ�.
	iMat->GetVectorParameterValue(param, initColor);

	//UE_LOG(LogTemp, Warning, TEXT("R: %f, G = %f, B = %"), initColor.R, initColor.G, initColor.B);

	//Material Instance�� �̿��ؼ� Materianl Instance Dynamic ��ü�� �����.
	dynamicMaterial = UMaterialInstanceDynamic::Create(iMat, this);

	//������ Dynamic Material�� �޽��� �����Ѵ�.
	meshComp->SetMaterial(0, dynamicMaterial);

}

// Called every frame
void APlayerFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//����ڰ� �Է��� ������ �̵��ϰ� �ʹ�
	//���͸� ����ȭ
	direction.Normalize();

	// P = P0 + vt
	FVector dir = GetActorLocation() + direction * moveSpeed * DeltaTime;
	SetActorLocation(dir, true);

	/*UE_LOG(LogTemp, Warning, TEXT("%f"), moveSpeed);*/
	

}

// Called to bind functionality to input
void APlayerFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//������ UInputComponent* ������ UEnhancedInputComponent*�� ��ȯ�Ѵ�/
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//�Լ� �����ϱ�
	enhancedInputComponent->BindAction(IA_Horizontal, ETriggerEvent::Triggered, this, &APlayerFlight::Horizontal);
	enhancedInputComponent->BindAction(IA_Horizontal, ETriggerEvent::Completed, this, &APlayerFlight::Horizontal);
	enhancedInputComponent->BindAction(IA_Vertical, ETriggerEvent::Triggered, this, &APlayerFlight::Vertical);
	enhancedInputComponent->BindAction(IA_Vertical, ETriggerEvent::Completed, this, &APlayerFlight::Vertical);
	enhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &APlayerFlight::FireBullet);
	//enhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Triggered, this, &APlayerFlight::Boost);
	//enhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Completed, this, &APlayerFlight::BoostOff);
	enhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Triggered, this, &APlayerFlight::Boosting);
	enhancedInputComponent->BindAction(IA_Ult, ETriggerEvent::Triggered, this, &APlayerFlight::CheckEnemies);

// 	//Horizontal Axis �Է¿� �Լ��� �����Ѵ� (���ε� Binding)
// 	//this�� ��ũ��Ʈ �ν��Ͻ� ��ü�� ����Ŵ
// 	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerFlight::Horizontal);
// 	//Vertical Axis �Է¿� �Լ��� �����Ѵ� (���ε� Binding)
// 	PlayerInputComponent->BindAxis("Vertical", this, &APlayerFlight::Vertical); 
// 
// 	//Fire Action �Է¿� �Լ��� �����Ѵ�.
// 	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerFlight::FireBullet);
}

void APlayerFlight::ReservationHitColor(float time)
{
	//1. ������ Red�� �����Ѵ�.
	dynamicMaterial->SetVectorParameterValue(TEXT("Param"), (FVector4)FLinearColor::Red);

	//2. ���� �������� �ǵ����� �Լ��� ���ε��� Ÿ�̸Ӹ� �����Ѵ�.
	GetWorld()->GetTimerManager().SetTimer(colorTimer, this, &APlayerFlight::changeOriginColor, 0.2f, false);
}

void APlayerFlight::changeOriginColor()
{
	dynamicMaterial->SetVectorParameterValue(TEXT("Param"), (FVector)initColor);
}

//�¿� �Է��� ������ ����� �Լ�
// void APlayerFlight::Horizontal(float value)
// {
// 	h = value;
// 	/*UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), h);*/
// 	direction.Y = h;
// }

void APlayerFlight::Horizontal(const FInputActionValue& value)
{
	h = value.Get<float>();
	direction.Y = h;
}

//���� �Է��� ������ ����� �Լ�
// void APlayerFlight::Vertical(float value)
// {
// 	v = value;
// 	/*UE_LOG(LogTemp, Warning, TEXT("v : %.4f"), v);*/
// 	direction.Z = v;
// }

void APlayerFlight::Vertical(const FInputActionValue& value)
{
	v = value.Get<float>();
	direction.Z = v;
}


//���콺 ���� Ŭ�� ������ ����� �Լ�
void APlayerFlight::FireBullet()
{
	if(!isTrapOn){
	// �Ѿ��� �����Ѵ�.
	// �Ѿ� �������Ʈ ����
	FRotator spawnRotation = FRotator(90,0,0);
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector spawnPosition;

	//bullet Count ����ŭ ���� ���͸� �ݺ��Ѵ�.
	for (int32 i = 1; i <= bulletCount; i++)
	{
		//������������ bullet count -1 * bulletSpacing ��ŭ Y�� �������� �и���
		//¦�� Ȧ�� ������� �������� (bulletCount -1)* -75 ��ŭ Y������ ��ܼ� ����
		spawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f + FVector(0, (i - 1) * bulletSpacing - (bulletCount-1) * bulletSpacing/2, 0);
		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, spawnPosition, spawnRotation, param);

	if (bullet != nullptr)
		{
		//������ �Ѿ�(bullet)�� bulletAngle��ŭ �����ϰ� ȸ����Ų��.
		//bullet->SetActorRotation(spawnRotation + FRotator(0, (i - 1) * bulletAngle - (bulletCount - 1) * bulletAngle / 2, 0));
		bullet->AddActorWorldRotation(FRotator(0, 0, (i - 1) * bulletAngle - (bulletCount - 1) * bulletAngle / 2));
		}
	}
	//�Ѿ� �߻� ȿ������ �����Ѵ�.
	UGameplayStatics::PlaySound2D(this, fireSound, 0.2f);
	}
	else {
		return;
	}
}

void APlayerFlight::Boost()
{
	//UE_LOG(LogTemp, Warning, TEXT("Start Boost"));
	//�� ���� ���� ���
	/*moveSpeed = moveSpeed * 2;*/
	moveSpeed *= 2;
}

void APlayerFlight::BoostOff()
{
	//UE_LOG(LogTemp, Warning, TEXT("End Boost"));
	//�� ���� ���� ���
	moveSpeed *= 0.25;
	/*moveSpeed = moveSpeed / 2;*/
}

void APlayerFlight::Boosting()
{
	//���
	isBoosting = !isBoosting;

	if (isBoosting)
	{
		moveSpeed = moveSpeedOrigin *2;
	}
	else
	{
		moveSpeed = moveSpeedOrigin;
	}
}

void APlayerFlight::ExplosionAll()
{
	//��� Enemy�� �ı��Ѵ�.

	//1. TActorIterator�� Enemy�� ã�Ƴ��� ���ִ� ���
	//for (TActorIterator<AEnemy> it(GetWorld()); it; ++it)
	//{
	//	it->DestroyMyself();
	//}

	//2. TArray<T>�迭�� �̿��� ���
// 	AMyShootingGameModeBase* gm = Cast<AMyShootingGameModeBase>(GetWorld()->GetAuthGameMode());
// 	if(gm != nullptr)
// 	{
// 		for(int32 i = 0; i < gm->enemies.Num(); i++)
// 		{
// 			//Pending Kill ���� üũ
// 			if (IsValid(gm->enemies[i]))
// 			{
// 				gm->enemies[i]->DestroyMyself();
// 			}
// 			//����Ʈ�� �ʱ�ȭ�Ѵ�.
// 			//gm->enemies.Empty();
// 		
// 		}
// 	}

	//3. ��������Ʈ�� �����Ѵ�
	//playerBomb.Broadcast();
	dirMod.Broadcast(FVector(0,-1,0));
}

void APlayerFlight::CheckEnemies()
{
	//�ݰ� 5���� �̳��� �ִ� ��� AEnemy ���͵��� �����Ѵ�.
	//������ ���ʹ̵��� ������ ���� ������ �迭
	TArray<FOverlapResult> enemiesInfo;
	FVector centerLoc = GetActorLocation() + GetActorUpVector() * 700;
	FQuat centerRot = GetActorRotation().Quaternion();
	FCollisionObjectQueryParams params = ECC_GameTraceChannel2;
	FCollisionShape checkShape = FCollisionShape::MakeSphere(500);

	GetWorld()->OverlapMultiByObjectType(enemiesInfo, centerLoc, centerRot, params, checkShape);

	//üũ�� ��� ���ʹ��� �̸��� ����Ѵ�.
	for(FOverlapResult enemyInfo : enemiesInfo)
	{
	UE_LOG(LogTemp, Warning, TEXT("Hit : %s"), *enemyInfo.GetActor()->GetName());

	enemyInfo.GetActor()->Destroy();
	}
	
	DrawDebugSphere(GetWorld(), centerLoc, 500, 20, FColor::Yellow, false, 2);
}