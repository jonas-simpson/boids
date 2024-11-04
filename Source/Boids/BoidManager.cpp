// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidManager.h"

// Sets default values
ABoidManager::ABoidManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(Root);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Bounds"));
	Box->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform );
	Box->SetBoxExtent(FVector(1000,1000,500));
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
}



// Called when the game starts or when spawned
void ABoidManager::BeginPlay()
{
	Super::BeginPlay();

	InitializeMembers();
	
}

// Called every frame
void ABoidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoidManager::InitializeMembers()
{
	// Create a new Boid Member to add to our array and set its initial position
	for(int32 i = 0; i < MemberPopulation; i++)
	{
		ABoidMember* NewMember = GetWorld()->SpawnActor<ABoidMember>();
		Members.Add(NewMember);
	}
}
