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


	//충돌 박스 컴포넌트를 생성한다.
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));

	//생성된 충돌 박스 컴포넌트를 루트 컴포넌트로 설정한다.
	SetRootComponent(boxComp);

	//박스 콜리전의 크기를 가로*세로*높이 모두 50cm로 설정한다
	boxComp->SetBoxExtent(FVector(50.0f));
	//벡터 값이 XYZ모두 같으면 값 하나만 입력해줘도 된다

	//박스 콜리전의 충돌 처리 프리셋을 "PlayerPreset"으로 설정한다
	boxComp->SetCollisionProfileName(TEXT("PlayerPreset"));


	//메쉬 컴포넌트를 생성
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	//메쉬 컴포넌트를 루트 컴포넌트의 하위 개체로 등록한다 boxComp라고 적어도 되고, RootComponent라고 적어도 알아서 찾아줌
	meshComp->SetupAttachment(RootComponent);

	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//메쉬 컴포넌트의 static mesh 항목에 큐브 파일을 할당한다.
	ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));

	//만일 큐브 파일을 로드하는데 성공했다면
	if (cubeMesh.Succeeded())
	{
		//로드한 파일을 메쉬 컴포넌트의 static mesh 항목에 넣는다.
		meshComp->SetStaticMesh(cubeMesh.Object);
	}
}

// Called when the game starts or when spawned
void APlayerFlight::BeginPlay()
{
	Super::BeginPlay();

	
	//플레이어 컨트롤러를 캐스팅한다.
	APlayerController* PlayerCon = Cast<APlayerController>(GetController());

	if (PlayerCon != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerCon->GetLocalPlayer());

		if (subsys != nullptr)
		{
			subsys->AddMappingContext(IMC_MyMapping, 0);
		}
	}
	
	//현재 색상 값을 저장한다.
	UMaterialInterface* iMat = meshComp->GetMaterial(0);
	FHashedMaterialParameterInfo param = FHashedMaterialParameterInfo(TEXT("Param"));

	//Material Instance에서 벡터 파라미터 값을 initColor 변수에 저장한다.
	iMat->GetVectorParameterValue(param, initColor);

	//UE_LOG(LogTemp, Warning, TEXT("R: %f, G = %f, B = %"), initColor.R, initColor.G, initColor.B);

	//Material Instance를 이용해서 Materianl Instance Dynamic 개체를 만든다.
	dynamicMaterial = UMaterialInstanceDynamic::Create(iMat, this);

	//생성한 Dynamic Material을 메쉬에 설정한다.
	meshComp->SetMaterial(0, dynamicMaterial);

}

// Called every frame
void APlayerFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//사용자가 입력한 방향대로 이동하고 싶다
	//벡터를 정규화
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

	//기존의 UInputComponent* 변수를 UEnhancedInputComponent*로 변환한다/
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//함수 연결하기
	enhancedInputComponent->BindAction(IA_Horizontal, ETriggerEvent::Triggered, this, &APlayerFlight::Horizontal);
	enhancedInputComponent->BindAction(IA_Horizontal, ETriggerEvent::Completed, this, &APlayerFlight::Horizontal);
	enhancedInputComponent->BindAction(IA_Vertical, ETriggerEvent::Triggered, this, &APlayerFlight::Vertical);
	enhancedInputComponent->BindAction(IA_Vertical, ETriggerEvent::Completed, this, &APlayerFlight::Vertical);
	enhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &APlayerFlight::FireBullet);
	//enhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Triggered, this, &APlayerFlight::Boost);
	//enhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Completed, this, &APlayerFlight::BoostOff);
	enhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Triggered, this, &APlayerFlight::Boosting);
	enhancedInputComponent->BindAction(IA_Ult, ETriggerEvent::Triggered, this, &APlayerFlight::CheckEnemies);

// 	//Horizontal Axis 입력에 함수를 연결한다 (바인딩 Binding)
// 	//this는 스크립트 인스턴스 자체를 가리킴
// 	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerFlight::Horizontal);
// 	//Vertical Axis 입력에 함수를 연결한다 (바인딩 Binding)
// 	PlayerInputComponent->BindAxis("Vertical", this, &APlayerFlight::Vertical); 
// 
// 	//Fire Action 입력에 함수를 연결한다.
// 	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerFlight::FireBullet);
}

void APlayerFlight::ReservationHitColor(float time)
{
	//1. 색상을 Red로 변경한다.
	dynamicMaterial->SetVectorParameterValue(TEXT("Param"), (FVector4)FLinearColor::Red);

	//2. 원래 색상으로 되돌리는 함수를 바인딩한 타이머를 예약한다.
	GetWorld()->GetTimerManager().SetTimer(colorTimer, this, &APlayerFlight::changeOriginColor, 0.2f, false);
}

void APlayerFlight::changeOriginColor()
{
	dynamicMaterial->SetVectorParameterValue(TEXT("Param"), (FVector)initColor);
}

//좌우 입력이 있을때 실행될 함수
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

//상하 입력이 있을때 실행될 함수
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


//마우스 왼쪽 클릭 됐을때 실행될 함수
void APlayerFlight::FireBullet()
{
	if(!isTrapOn){
	// 총알을 생성한다.
	// 총알 블루프린트 변수
	FRotator spawnRotation = FRotator(90,0,0);
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector spawnPosition;

	//bullet Count 수만큼 스폰 액터를 반복한다.
	for (int32 i = 1; i <= bulletCount; i++)
	{
		//스폰포지션은 bullet count -1 * bulletSpacing 만큼 Y축 방향으로 밀리게
		//짝수 홀수 상관없이 시작점을 (bulletCount -1)* -75 만큼 Y축으로 당겨서 시작
		spawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f + FVector(0, (i - 1) * bulletSpacing - (bulletCount-1) * bulletSpacing/2, 0);
		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, spawnPosition, spawnRotation, param);

	if (bullet != nullptr)
		{
		//생성된 총알(bullet)을 bulletAngle만큼 일정하게 회전시킨다.
		//bullet->SetActorRotation(spawnRotation + FRotator(0, (i - 1) * bulletAngle - (bulletCount - 1) * bulletAngle / 2, 0));
		bullet->AddActorWorldRotation(FRotator(0, 0, (i - 1) * bulletAngle - (bulletCount - 1) * bulletAngle / 2));
		}
	}
	//총알 발사 효과음을 실행한다.
	UGameplayStatics::PlaySound2D(this, fireSound, 0.2f);
	}
	else {
		return;
	}
}

void APlayerFlight::Boost()
{
	//UE_LOG(LogTemp, Warning, TEXT("Start Boost"));
	//두 문장 같은 결과
	/*moveSpeed = moveSpeed * 2;*/
	moveSpeed *= 2;
}

void APlayerFlight::BoostOff()
{
	//UE_LOG(LogTemp, Warning, TEXT("End Boost"));
	//두 문장 같은 결과
	moveSpeed *= 0.25;
	/*moveSpeed = moveSpeed / 2;*/
}

void APlayerFlight::Boosting()
{
	//토글
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
	//모든 Enemy를 파괴한다.

	//1. TActorIterator로 Enemy를 찾아내서 없애는 방법
	//for (TActorIterator<AEnemy> it(GetWorld()); it; ++it)
	//{
	//	it->DestroyMyself();
	//}

	//2. TArray<T>배열을 이용한 방식
// 	AMyShootingGameModeBase* gm = Cast<AMyShootingGameModeBase>(GetWorld()->GetAuthGameMode());
// 	if(gm != nullptr)
// 	{
// 		for(int32 i = 0; i < gm->enemies.Num(); i++)
// 		{
// 			//Pending Kill 상태 체크
// 			if (IsValid(gm->enemies[i]))
// 			{
// 				gm->enemies[i]->DestroyMyself();
// 			}
// 			//리스트를 초기화한다.
// 			//gm->enemies.Empty();
// 		
// 		}
// 	}

	//3. 델리게이트를 실행한다
	//playerBomb.Broadcast();
	dirMod.Broadcast(FVector(0,-1,0));
}

void APlayerFlight::CheckEnemies()
{
	//반경 5미터 이내에 있는 모든 AEnemy 액터들을 감지한다.
	//감지된 에너미들의 정보를 담을 변수와 배열
	TArray<FOverlapResult> enemiesInfo;
	FVector centerLoc = GetActorLocation() + GetActorUpVector() * 700;
	FQuat centerRot = GetActorRotation().Quaternion();
	FCollisionObjectQueryParams params = ECC_GameTraceChannel2;
	FCollisionShape checkShape = FCollisionShape::MakeSphere(500);

	GetWorld()->OverlapMultiByObjectType(enemiesInfo, centerLoc, centerRot, params, checkShape);

	//체크된 모든 에너미의 이름을 출력한다.
	for(FOverlapResult enemyInfo : enemiesInfo)
	{
	UE_LOG(LogTemp, Warning, TEXT("Hit : %s"), *enemyInfo.GetActor()->GetName());

	enemyInfo.GetActor()->Destroy();
	}
	
	DrawDebugSphere(GetWorld(), centerLoc, 500, 20, FColor::Yellow, false, 2);
}