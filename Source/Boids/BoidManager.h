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

	// Number of BoidMembers to spawn and manage
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MemberPopulation;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// FVector Bounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Box;

	void InitializeMembers();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TArray<ABoidMember*> Members;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
