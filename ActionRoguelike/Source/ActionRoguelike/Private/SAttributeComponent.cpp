// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SCharacter.h"
#include "SGameModeBase.h"
#include "AI/SAICharacter.h"
#include "Net/UnrealNetwork.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."),ECVF_Cheat);


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	
	Health = 100;
	HealthMax = 100;
	Rage = 0.0f;
	RageMax = 50.f;

	// How to SetIsReplicatedByDefault : 컴포넌트에만 존재하는 함수. 컴포넌트가 붙는 시점에 리플리케이션을 켜도록 설정해 주는 기능
	SetIsReplicatedByDefault(true);
	// 이 컴포넌트 인스턴스를 런타임에 리플리케이션 대상으로 지정한다. 어떤 컴포넌트들은 리플리케이트 하지 않게 할 수 있어야 한다.(성능 최적화, 개발자 재량)
	SetIsReplicated(true);
	
	
}



bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::CanRage() const
{
	return Rage >= RageMax;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		// for console multiply
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}
	
	float OldHealth = Health;
	float NewHealth = FMath::Clamp(OldHealth + Delta, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;
	 
	
	Health = NewHealth;
	// How to Broadcast :
	//OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, ActualDelta);

	if (ActualDelta != 0.0f)
	{
		MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
	}
	
	// Died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);	
		}
		 
	}
	

	return ActualDelta != 0;
}

bool USAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	
	const float OldRage = Rage;

	Rage = FMath::Clamp(Rage+Delta, 0.0f, RageMax);
	
	float ActualDelta = Rage - OldRage;

	// Will be zero delta if we already at max or min
	if (!FMath::IsNearlyZero(ActualDelta))
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
		return true;
	}

	return false;
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

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewValue, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor,this, NewValue, Delta);	
}


void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// How to DOREPLIFETIME : 원하는 프로퍼티를 리플리케이션 대상으로 지정해서 값 변경이 있을 때 클라이언트 <-> 서버 간에 동기화 된다.
	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);

	// How to DOREPLIFETIME_CONDITION : 설정을 전달해서 replicate 가능
	//DOREPLIFETIME_CONDITION(USAttributeComponent, HealthMax,COND_OwnerOnly);
	 
}
