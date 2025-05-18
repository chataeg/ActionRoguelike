// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"

#include "SAttributeComponent.h"

ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	
}

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	UE_LOG(LogTemp,Warning,TEXT("HealthPotion"));

	
	if (USAttributeComponent* AttributeComp = Cast<USAttributeComponent>((InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()))))
	{
		if (!bIsActive || AttributeComp->GetHealth() > 0.0f || AttributeComp->GetHealth() != AttributeComp->GetHealthMax())
		{
			AttributeComp->ApplyHealthChange(AttributeComp->GetHealthMax());
			HideAndCooldownPowerUp();
		}
	}
	
	
}
