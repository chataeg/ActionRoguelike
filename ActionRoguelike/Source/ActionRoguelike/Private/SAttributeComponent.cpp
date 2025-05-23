// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	
	Health = 100;
	HealthMax = 100;
	
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}



bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	float OldHealth = Health;
	float NewHealth = FMath::Clamp(OldHealth + Delta, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;
	
	// How to Broadcast : 
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth,ActualDelta);
	Health = NewHealth;
	
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

