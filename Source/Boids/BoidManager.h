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
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	USceneComponent* Root;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABoidMember> BoidBP;

	
	// Swarm influence variables ----------------------------
	
	// Number of BoidMembers to spawn and manage
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MemberPopulation;

	// Max Boid speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpeed;

	// Min Boid speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinSpeed;

	// Radius at which members observe neighbors for Alignment and Cohesion
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VisualRange;

	// Radius at which members observe neighbors for Avoidance
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProtectedRange;

	// Influence of Cohesion
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CenteringFactor;

	// Influence of Separation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AvoidFactor;

	//Influence of Alignment
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MatchingFactor;

	// Influence pushing members into Manager bounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoundsFactor;

	void InitializeMembers();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void MoveAllMembers();

private:
	TArray<ABoidMember*> Members;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
