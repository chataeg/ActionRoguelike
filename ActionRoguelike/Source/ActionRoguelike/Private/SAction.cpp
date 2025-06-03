// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"


void USAction::Initialize(USActionComponent* NewActionComponent)
{
	ActionComp = NewActionComponent;
}


bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}
	
	USActionComponent* Comp = GetOwningComponent();

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	// How to GetSafeName : 크래시 없이 오브젝트의 이름을 반환. 이름이 없을 시 None  반환
	UE_LOG(LogTemp, Log,TEXT("Running : %s"), *GetNameSafe(this));
	//UE_LOGFMT(LogGame, Log, "Started: {ActionName}", GetName());

	//LogOnScreen(this,FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);
	
	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log,TEXT("Stopped : %s"), *GetNameSafe(this));
	//UE_LOGFMT(LogGame, Log, "Stopped: {name}", GetName());

	//LogOnScreen(this,FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
	
	// How to ensureAlways : false 일 때 매번 로그 출력
	//ensureAlways(bIsRunning);
	
	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
}

class UWorld* USAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	
	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return ActionComp;
}

void USAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

bool USAction::IsRunning() const
{
	return RepData.bIsRunning;
}

bool USAction::IsSupportedForNetworking() const
{
	return true;
}

void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComp);
}
