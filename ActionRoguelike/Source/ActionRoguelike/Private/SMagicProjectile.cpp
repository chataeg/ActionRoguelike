// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	InitialLifeSpan = 10.f;
	Damage = 50.0f;
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	
	if (OtherActor && OtherActor != GetInstigator())
	{
		
		// How to GetComponentByClass : StaticClass 를 통해 액터가 컴포넌트를 가지고 있는지 확인
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>((OtherActor->GetComponentByClass(USAttributeComponent::StaticClass())));
		if (AttributeComp)
		{
			// How to IsA : 특정 클래스인지 확인할 수 있음
			if (OtherActor->IsA(ASCharacter::StaticClass()) )
			{
				AttributeComp->ApplyHealthChange(GetInstigator(),-1.0f);
			}
			else
			{
				AttributeComp->ApplyHealthChange(GetInstigator(),-Damage);
			}
				
			// 맞았으면 Destroy
			// Destroy();

			Explode();
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

