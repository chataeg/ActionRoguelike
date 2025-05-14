// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "GameFramework/Actor.h"
#include "SDashProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDashProjectile();

protected:
	UPROPERTY(EditDefaultsOnly)
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly)
	float DetonateDelay;

	// Handle to cancel timer if we already hit something
	FTimerHandle TimerHandle_DelayedDetonate;

	// Base class using BlueprintNativeEvent, we must override the _Implementation not the Explode()
	virtual void Explode_Implementation() override;
	
	
	void TeleportInstigator();


	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
