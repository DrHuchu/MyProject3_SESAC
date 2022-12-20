// Fill out your copyright notice in the Description page of Project Settings.


#include "PointerClass.h"

// Sets default values
APointerClass::APointerClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APointerClass::BeginPlay()
{
	Super::BeginPlay();
	
	myAge = &realAge;
	 
}

// Called every frame
void APointerClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

