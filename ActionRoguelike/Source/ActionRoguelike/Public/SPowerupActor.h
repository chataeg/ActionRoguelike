// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"


UCLASS()
class ACTIONROGUELIKE_API ASPowerupActor : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerupActor();

	
	FTimerHandle TimerHandle_PowerupPotion;
	
protected:

	// How to bool : bool 로 선언하면 내부에선 실제로 uint8 기반으로 처리된다.
	UPROPERTY(VisibleAnywhere , Category = "Attributes")
	uint8 bIsActive : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float RespawnTime;

	void HideAndCooldownPowerUp();

	void ShowAndCoolEndPowerUp();
	
public:

	bool IsActive() const;

};
