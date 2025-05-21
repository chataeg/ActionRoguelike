// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	// How to ensureMsgf : 메세지와 함께 ensure 디버그 가능, 강의에서는 디버그에도 좋지만 디자이너 등에게 Bug Fix TODO 를 알려주는데도 좋다고 한다.
	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}
	
	// APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//
	// if (MyPawn)
	// {
	// 	GetBlackboardComponent()->SetValueAsVector("MoveToLocation",MyPawn->GetActorLocation());
	// 	
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	// }

	

	
	
	
}
