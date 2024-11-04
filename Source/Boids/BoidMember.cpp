// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidMember.h"

// Sets default values
ABoidMember::ABoidMember()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);

}

// Called when the game starts or when spawned
void ABoidMember::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoidMember::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
