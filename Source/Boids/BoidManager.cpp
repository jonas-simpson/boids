// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidManager.h"

#include "Kismet/KismetMathLibrary.h"

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
	Box->SetHiddenInGame(false);
	Box->SetMobility(EComponentMobility::Static);

	MemberPopulation = 50;
	MaxSpeed = 20;
	MinSpeed = 5;
	VisualRange = 750;
	ProtectedRange = 100;
	CenteringFactor = 0.005;
	AvoidFactor = 0.05;
	MatchingFactor = 0.5;
	BoundsFactor = 0.5;	
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

		NewMember->SetActorLocation(UKismetMathLibrary::RandomPointInBoundingBox(Box->GetComponentLocation(),Box->GetUnscaledBoxExtent()));
		
		Members.Add(NewMember);
	}
}

void ABoidManager::MoveAllMembers()
{
	//Look at every member
	for (ABoidMember* B : Members)
	{
		FVector CloseDelta, SeparationV, AvgNeighborV, AlignmentV, AvgNeighborL, CohesionV {0,0,0};
		int32 NumNeighbors {0};
		FVector BV = B->GetV();
		FVector BL = B->GetActorLocation();
		
		//Compare to every other member
		for (ABoidMember* Other : Members)
		{
			if(B!=Other)
			{
				FVector OV = Other->GetV();
				FVector OL = Other->GetActorLocation();
				float Distance = UKismetMathLibrary::Vector_Distance(BL,OL);

				//Check if other member is within my protected range
				if(Distance < ProtectedRange)
				{
					//Separation
					CloseDelta += BL - OL;
				}
				//Check if other member is within observable range
				else if(Distance < VisualRange)
				{
					NumNeighbors++;

					//Alignment
					AvgNeighborV += OV;

					//Cohesion
					AvgNeighborL += OL;
				}
			}
		}

		//Separation
		SeparationV = CloseDelta * AvoidFactor;
		if(NumNeighbors > 0)
		{
			//Alignment
			AvgNeighborV /= NumNeighbors;
			AlignmentV = (AvgNeighborV - BV) * MatchingFactor;

			//Cohesion
			AvgNeighborL /= NumNeighbors;
			CohesionV = (AvgNeighborL - BL) * CenteringFactor;

			BV += AlignmentV + CohesionV;
		}

		BV += SeparationV;
		
	}
}

