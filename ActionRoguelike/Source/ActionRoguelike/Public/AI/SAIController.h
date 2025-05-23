// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SAIController.generated.h"

/**
 * 
 */
class UBehaviorTree;
class UAIPerceptionComponent;


UCLASS()
class ACTIONROGUELIKE_API ASAIController : public AAIController
{
	GENERATED_BODY()

	ASAIController();
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	// How to AIPerceptionComponent : Pawn 과 분리, 유연성을 위해 AIController 에 선언한다.
	// 25.05.21 현재 BP 에 로직이 있음.
	// https://dev.epicgames.com/documentation/ko-kr/unreal-engine/ai-perception-in-unreal-engine
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComp;

	
	FGenericTeamId TeamId;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void PostInitializeComponents() override;

public: 
	virtual FGenericTeamId GetGenericTeamId() const override;
	
};
