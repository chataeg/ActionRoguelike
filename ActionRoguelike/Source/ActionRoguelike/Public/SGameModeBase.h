// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

namespace EEnvQueryStatus
{
	enum Type : int;
}

class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;

class UEnvQuery;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UEnvQuery> SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UCurveFloat> DifficultyCurve;

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UFUNCTION()
	void SpawnBotTimerElasped();

	UFUNCTION()
	void OnQueryCompleted( UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	// How to GameMode : 게임모드는 모든 액터들에 대해 Beginplay 를 호출하는 역할을 하기 때문에 BeginPlay 가 없다. 그 대신 StartPlay 사용.

public:
	ASGameModeBase();
	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAll();
	
	
};