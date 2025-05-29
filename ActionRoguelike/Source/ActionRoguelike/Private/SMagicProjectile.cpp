// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SActionComponent.h"
#include "SActionEffect.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	InitialLifeSpan = 10.f;
	DamageAmount = 50.0f;
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	
	if (OtherActor && OtherActor != GetInstigator())
	{
		
		// How to GetComponentByClass : StaticClass 를 통해 액터가 컴포넌트를 가지고 있는지 확인
		// USAttributeComponent* AttributeComp = Cast<USAttributeComponent>((OtherActor->GetComponentByClass(USAttributeComponent::StaticClass())));
		// if (AttributeComp)
		// {
		// 	// How to IsA : 특정 클래스인지 확인할 수 있음
		// 	if (OtherActor->IsA(ASCharacter::StaticClass()) )
		// 	{
		// 		AttributeComp->ApplyHealthChange(GetInstigator(),-1.0f);
		// 	}
		// 	else
		// 	{
		// 		AttributeComp->ApplyHealthChange(GetInstigator(),-DamageAmount);
		// 	}
		// 		
		// 	Explode();
		// }


		//FName Muzzle = "Muzzle_01";

		//static FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Status.Parrying");
		
		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			// MovementComp->bRotationFollowsVelocity = true;  in projectilebase.cpp
			MovementComp->Velocity = -MovementComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		
		if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount,SweepResult))
		{
			Explode();

			if (ActionComp)
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass );
			}
		}
		
		
	}
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&ASMagicProjectile::OnActorOverlap);
}
