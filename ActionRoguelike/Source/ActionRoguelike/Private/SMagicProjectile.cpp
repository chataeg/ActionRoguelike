// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SAttributeComponent.h"
#include "Chaos/AABBTree.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	InitialLifeSpan = 10.f;
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UE_LOG(LogTemp,Warning,TEXT("dsafsdafasdfasdfsadfasdfsadf"));
	
	if (OtherActor)
	{
		// How to GetComponentByClass
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>((OtherActor->GetComponentByClass(USAttributeComponent::StaticClass())));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.0f);

			// 맞았으면 Destroy
			Destroy();
		}
	}
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&ASMagicProjectile::OnActorOverlap);
}


// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

