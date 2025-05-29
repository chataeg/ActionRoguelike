// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect.h"

#include "SActionComponent.h"

USActionEffect::USActionEffect()
{
	bAutoStart = true;
}

void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &ThisClass::StopAction, Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}
	
	if (Period > 0.0f)
	{
		// Can bind in-line using FTimerDelegate::CreateUObject instead
		GetWorld()->GetTimerManager().SetTimer(
			PeriodHandle,
			FTimerDelegate::CreateUObject(this, &ThisClass::ExecutePeriodicEffect, Instigator),
			Period,
			true);
	}
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{
	// Don`t miss out last tick
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
			ExecutePeriodicEffect(Instigator);		
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	USActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAction(this);
	}
	
}


void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{


	
}
