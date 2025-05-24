// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SCharacter.h"
#include "AI/SAICharacter.h"


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	
	Health = 100;
	HealthMax = 100;
	
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}



bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}
	float OldHealth = Health;
	float NewHealth = FMath::Clamp(OldHealth + Delta, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;
	
	// How to Broadcast :
	Health = NewHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, ActualDelta);

	return ActualDelta != 0;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent> (FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	// AttributeComp 가 없으면 죽은 걸로 생각하겠다.
	return false;
}

