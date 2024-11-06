// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BoidMember.h"
#include "Components/BoxComponent.h"

#include "BoidManager.generated.h"

UCLASS(Blueprintable)
class BOIDS_API ABoidManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidManager();

	//Actor Components --------------------------------------
	UPROPERTY(BlueprintReadWrite)
	USceneComponent* Root;
	
	UPROPERTY(BlueprintReadWrite)
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABoidMember> BoidBP;

	
	// Swarm influence variables ----------------------------
	
	// Number of BoidMembers to spawn and manage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin = 5, UIMax = 100))
	int32 MemberPopulation;

	// Max Boid speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin = 5, UIMax = 100))
	float MaxSpeed;

	// Min Boid speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin = 0, UIMax = 25))
	float MinSpeed;

	// Radius at which members observe neighbors for Alignment and Cohesion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin = 300, UIMax = 1000))
	float VisualRange;

	// Radius at which members observe neighbors for Avoidance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin = 50, UIMax = 300))
	float ProtectedRange;

	// Influence of Cohesion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin = 0, UIMax = 0.05))
	float CenteringFactor;

	// Influence of Separation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin = 0, UIMax = 0.05))
	float AvoidFactor;

	//Influence of Alignment
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin = 0, UIMax = 0.05))
	float MatchingFactor;

	// Influence pushing members into Manager bounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin = 0, UIMax = 0.5))
	float BoundsFactor;

	void InitializeMembers();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void MoveAllMembers();

private:
	TArray<ABoidMember*> Members;
	bool ShouldStart;
	void TimerElapsed();
	float TimerDelay;

	UFUNCTION(BlueprintCallable)
	FVector GetBoundaryForce(FVector BoidLoc) const;
	FVector ClampToSpeed(FVector BoidVel) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
