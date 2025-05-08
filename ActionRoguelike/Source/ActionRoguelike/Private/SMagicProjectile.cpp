// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "Chaos/AABBTree.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
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
	// SphereComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	
	RootComponent = SphereComp;
	
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.f;
	// 속도 방향에 따라 액터의 회전이 자동으로 바뀜
	MovementComp->bRotationFollowsVelocity = true;
	// 속도가 액터가 바라보는 방향 기준으로 적용
	MovementComp->bInitialVelocityInLocalSpace = true;
	
	
	
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

