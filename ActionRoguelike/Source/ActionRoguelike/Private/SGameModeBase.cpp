// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn in more bots.
	// Actual amount of bots and whether it`s allowed to spawn determined by spawn logic later in the chain . . .
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElasped, SpawnTimerInterval, true);
	
}

void ASGameModeBase::SpawnBotTimerElasped()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
	
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComponent = Cast<USAttributeComponent> (Bot->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComponent && AttributeComponent->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	float MaxBotCount = 10.f;
	
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NrOfAliveBots >= MaxBotCount)
	{
		return;
	}
	
	
	// How to TArray : https://dev.epicgames.com/documentation/ko-kr/unreal-engine/array-containers-in-unreal-engine
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	

	if (Locations.IsValidIndex(0))
	{
		
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), Locations[0], 100.0f, 12, FColor::Red, false, 30.0f);
		//GetWorld()->SpawnActor<AActor>(MinionClass, FVector(0.0f,0.0f,100.f), FRotator::ZeroRotator);
	}


	
}