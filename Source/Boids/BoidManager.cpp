// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidManager.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABoidManager::ABoidManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Init components
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(Root);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Bounds"));
	Box->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform );
	Box->SetBoxExtent(FVector(1000,1000,500));
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetHiddenInGame(false);
	Box->SetMobility(EComponentMobility::Static);

	//Init variables
	MemberPopulation = 50;
	MaxSpeed = 10;
	MinSpeed = 1;
	VisualRange = 500;
	ProtectedRange = 100;
	CenteringFactor = 0.0005;
	AvoidFactor = 0.005;
	MatchingFactor = 0.005;
	BoundsFactor = 0.1;

	//Setup timer
	ShouldStart = false;
	TimerDelay = 0.5;
}



// Called when the game starts or when spawned
void ABoidManager::BeginPlay()
{
	Super::BeginPlay();

	InitializeMembers();

	//Start moving Boids after they have all spawned
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABoidManager::TimerElapsed, TimerDelay, false);
}




// Called every frame
void ABoidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ShouldStart)
		MoveAllMembers();
}

void ABoidManager::InitializeMembers()
{
	// Create a new Boid Member to add to our array and set its initial position
	for(int32 i = 0; i < MemberPopulation; i++)
	{
		FVector SpawnLoc = UKismetMathLibrary::RandomPointInBoundingBox(Box->GetComponentLocation(),Box->GetUnscaledBoxExtent());
		FRotator SpawnRot = UKismetMathLibrary::RandomRotator();
		FActorSpawnParameters SpawnParams;

		
		ABoidMember* NewMember = GetWorld()->SpawnActor<ABoidMember>(BoidBP, SpawnLoc, SpawnRot, SpawnParams);

		//NewMember->SetActorLocation(UKismetMathLibrary::RandomPointInBoundingBox(Box->GetComponentLocation(),Box->GetUnscaledBoxExtent()));
		
		Members.Add(NewMember);
	}
}

void ABoidManager::MoveAllMembers()
{
	//Look at every member
	for (ABoidMember* B : Members)
	{
		FVector CloseDelta {0,0,0}, SeparationV {0,0,0}, AvgNeighborV {0,0,0}, AlignmentV {0,0,0}, AvgNeighborL {0,0,0}, CohesionV {0,0,0};
		int32 NumNeighbors {0};
		//FVector BV = B->GetV();
		FVector BV {0,0,0};
		FVector BL = B->GetActorLocation();
		
		//Compare to every other member
		for (ABoidMember* Other : Members)
		{
			if(B!=Other)
			{
				FVector OV = Other->GetV();
				FVector OL = Other->GetActorLocation();
				float Distance = UKismetMathLibrary::Abs(UKismetMathLibrary::Vector_Distance(BL,OL));

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
		}

		//Apply boundary force
		FVector BoundaryV = GetBoundaryForce(BL);

		//Apply all forces to current velocity
		BV = B->GetV() + CohesionV + AlignmentV + SeparationV + BoundaryV;

		//Check speed & confine within limits
		BV = ClampToSpeed(BV);

		//Apply new velocity and update position!
		B->SetV(BV);
		B->UpdateLocation();
	}
}
void ABoidManager::TimerElapsed()
{
	ShouldStart = true;
}

FVector ABoidManager::GetBoundaryForce(FVector BoidLoc) const
{
	FVector Influence{0,0,0};
	if(BoidLoc.X > Box->GetComponentLocation().X+Box->GetUnscaledBoxExtent().X)
	{
		Influence.X = -BoundsFactor;
	}
	else if(BoidLoc.X < Box->GetComponentLocation().X-Box->GetUnscaledBoxExtent().X)
	{
		Influence.X = BoundsFactor;
	}
	if(BoidLoc.Y > Box->GetComponentLocation().Y+Box->GetUnscaledBoxExtent().Y)
	{
		Influence.Y = -BoundsFactor;
	}
	else if(BoidLoc.Y < Box->GetComponentLocation().Y-Box->GetUnscaledBoxExtent().Y)
	{
		Influence.Y = BoundsFactor;
	}
	if(BoidLoc.Z > Box->GetComponentLocation().Z+Box->GetUnscaledBoxExtent().Z)
	{
		Influence.Z = -BoundsFactor;
	}
	else if(BoidLoc.Z < Box->GetComponentLocation().Z-Box->GetUnscaledBoxExtent().Z)
	{
		Influence.Z = BoundsFactor;
	}

	return Influence;
}

FVector ABoidManager::ClampToSpeed(FVector BoidVel) const
{
	FVector ClampedVel = BoidVel;
	
	//Check speed & confine within limits
	if(ClampedVel.Length() > MaxSpeed)
	{
		ClampedVel = ClampedVel.GetSafeNormal() * MaxSpeed;
	}
	else if(ClampedVel.Length() < MinSpeed)
	{
		ClampedVel = ClampedVel.GetSafeNormal() * MinSpeed;
	}

	return ClampedVel;
}


