// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	//MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Price = 80;
}

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{

	if (!ensure(InstigatorPawn))
	{
		return;
	}
	
	if (USAttributeComponent* AttributeComp = Cast<USAttributeComponent>((InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()))))
	{
		if (ensure(AttributeComp))
		{
			if (ASPlayerState* PlayerState = Cast<ASPlayerState>(InstigatorPawn->GetPlayerState()))
			{
				if (PlayerState->GetCredits() < Price )
				{
					return;
				}

				if (!bIsActive || AttributeComp->GetHealth() > 0.0f || AttributeComp->GetHealth() != AttributeComp->GetHealthMax())
				{
					if (PlayerState->RemoveCredits(Price) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
					{
						// 성공적으로 힐이 완료되고 돈이 빠져나갔을 때
						HideAndCooldownPowerUp();
					}
				}
			}
		}
	}
}
