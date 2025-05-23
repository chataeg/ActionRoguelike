// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include "ActionRoguelike/ActionRoguelike.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"

ASAIController::ASAIController()
{
	
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	
	
	TeamId = TEAM_ID_BOTS;
}

ETeamAttitude::Type ASAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(&Other);
	if (TeamAgent)
	{
		UE_LOG(LogTemp,Warning,TEXT("Other %s : %d  / self %s :  %d"),*Other.GetName(),TeamAgent->GetGenericTeamId().GetId(),*GetName(),GetGenericTeamId().GetId());
		return (TeamAgent->GetGenericTeamId() == GetGenericTeamId()) ? ETeamAttitude::Friendly : ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Neutral;
}

void ASAIController::BeginPlay()
{
	Super::BeginPlay();


	//SetGenericTeamId(FGenericTeamId(1));
	
	// How to ensureMsgf : 메세지와 함께 ensure 디버그 가능, 강의에서는 디버그에도 좋지만 디자이너 등에게 Bug Fix TODO 를 알려주는데도 좋다고 한다.
	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}

	// APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	// GetBlackboardComponent()->SetValueAsObject("TargetActor",MyPawn);
	// if (MyPawn)
	// {
	// 	GetBlackboardComponent()->SetValueAsVector("MoveToLocation",MyPawn->GetActorLocation());
	// 	
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	// }
	
}

FGenericTeamId ASAIController::GetGenericTeamId() const
{
	return TeamId;
}

