// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealth.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* MyController = OwnerComp.GetAIOwner();
	
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(AIPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (ensure(AttributeComp))
	{
		const bool bLowHealth = AttributeComp->GetHealth()  < 30.f;

		UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
		BlackBoardComp->SetValueAsBool(LowHealthKey.SelectedKeyName,bLowHealth);		
		
	}


	
}
