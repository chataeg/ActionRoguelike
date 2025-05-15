// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"

#include "SMagicProjectile.h"
#include "kismet/GameplayStatics.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/FastReferenceCollector.h"

// Sets default values
ASProjectileBase::ASProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");

	// SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	// 에디터 Project Settings -> Collision 에서 프로파일 추가
	SphereComp->SetCollisionProfileName("Projectile");
	// 모두 무시
	// SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	// 따로 설정도 가능
	//SphereComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	
	RootComponent = SphereComp;
	
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 8000.f;
	MovementComp->ProjectileGravityScale = 0.0f;
	// 속도 방향에 따라 액터의 회전이 자동으로 바뀜
	MovementComp->bRotationFollowsVelocity = true;
	// 속도가 액터가 바라보는 방향 기준으로 적용
	MovementComp->bInitialVelocityInLocalSpace = true;

	

	
}

void ASProjectileBase::Explode_Implementation()
{
	// Check to make sure we aren't already being 'destroyed'
	// Adding ensure to see if we encounter this situation at all

	 if (ensure(!IsEliminatingGarbage(EGCOptions::None)))
	 {
		 UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(),GetActorRotation());
		
		 Destroy();
	}

	
}

void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
		//Explode();
}

void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
}

// Called when the game starts or when spawned
void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

