// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	
	Health = 100;
	HealthMax = 100;
	
	// ...
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}


bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	//float ActualDelta = NewHealth - OldHealth;
	
	// How to Broadcast
	OnHealthChanged.Broadcast(nullptr, this, NewHealth,Delta);

	return true;
}

