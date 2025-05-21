// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealSelf.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// How to TaskNode : TaskNode 는 Super 호출 필요 없음.
	UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();

	AAIController* MyController = OwnerComp.GetAIOwner();

	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(MyPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

		if (ensure(AttributeComp))
		{
			AttributeComp->ApplyHealthChange(AttributeComp->GetHealthMax());
		}


		return EBTNodeResult::Succeeded;
		
	}
	

	
	return EBTNodeResult::Failed;
	
}
