// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/StaticMeshComponent.h"

#include "BoidMember.generated.h"

UCLASS()
class BOIDS_API ABoidMember : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidMember();

	//UPROPERTY(BlueprintReadWrite)
	//class UStaticMeshComponent* StaticMesh;

	UFUNCTION(BlueprintCallable)
	FVector GetV() { return V; }

	UFUNCTION(BlueprintCallable)
	void SetV(FVector NewV) { V = NewV;}

	UFUNCTION(BlueprintCallable)
	void UpdateLocation() { SetActorLocation(GetActorLocation() + V); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//Current Velocity
	UPROPERTY()
	FVector V;

};
