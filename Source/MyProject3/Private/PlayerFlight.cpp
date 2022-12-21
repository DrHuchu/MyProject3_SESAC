// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFlight.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Bullet.h"

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

	//메쉬 컴포넌트를 생성
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	//메쉬 컴포넌트를 루트 컴포넌트의 하위 개체로 등록한다 boxComp라고 적어도 되고, RootComponent라고 적어도 알아서 찾아줌
	meshComp->SetupAttachment(RootComponent);

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
	SetActorLocation(dir);


}

// Called to bind functionality to input
void APlayerFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Horizontal Axis 입력에 함수를 연결한다 (바인딩 Binding)
	//this는 스크립트 인스턴스 자체를 가리킴
	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerFlight::Horizontal);
	//Vertical Axis 입력에 함수를 연결한다 (바인딩 Binding)
	PlayerInputComponent->BindAxis("Vertical", this, &APlayerFlight::Vertical); 

	//Fire Action 입력에 함수를 연결한다.
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerFlight::FireBullet);
}

//좌우 입력이 있을때 실행될 함수
void APlayerFlight::Horizontal(float value)
{
	h = value;
	/*UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), h);*/
	direction.Y = h;
}

//상하 입력이 있을때 실행될 함수
void APlayerFlight::Vertical(float value)
{
	v = value;
	/*UE_LOG(LogTemp, Warning, TEXT("v : %.4f"), v);*/
	direction.Z = v;
}


//마우스 왼쪽 클릭 됐을때 실행될 함수
void APlayerFlight::FireBullet()
{
	// 총알을 생성한다.
	// 총알 블루프린트 변수
	FVector spawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
	FRotator spawnRotation = FRotator(90,0,0);
	//코드랑 언리얼이랑 축 순서가 다름
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<ABullet>(bulletFactory, spawnPosition, spawnRotation);
}

