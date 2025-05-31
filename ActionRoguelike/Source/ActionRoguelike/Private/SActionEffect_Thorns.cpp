// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"

#include "SAttributeComponent.h"
#include "SActionComponent.h"
#include "SGameplayFunctionLibrary.h"

USActionEffect_Thorns::USActionEffect_Thorns()
{
	Duration = 0.0f;
	Period = 0.0f;
	DamageFractionRate = 0.2f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}


void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp,
	float NewValue, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	float FractionDamage = 0.0f;

	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		
		FractionDamage = Delta * DamageFractionRate;

		FractionDamage = FMath::RoundToInt(FractionDamage);
		if (FractionDamage == 0)
		{
			return;
		}
		
		FractionDamage = FMath::Abs(FractionDamage);

		if (USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, FractionDamage))
		{
			//StopAction(InstigatorActor);
		}
	}
	
}