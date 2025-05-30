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
	// How to Effect : 마지막 틱을 놓치지 않기 위해 남은 타이머의 시간을 확인한다.
	// Ex) 타이머 = 3초 , 주기 = 1초 일때 1,2까지는 보장되지만 3초일 시 타이머가 먼저 종료되면 다음 틱(3번째) 가 실행 안될 수 있다.
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
	// Implemented in BP

	
}
