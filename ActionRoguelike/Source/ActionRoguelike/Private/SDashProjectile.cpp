// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASDashProjectile::ASDashProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;
	MovementComp->InitialSpeed = 6000.f;
}

// Called when the game starts or when spawned
void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASDashProjectile::Explode, DetonateDelay);

	// 움직이고 있을 때 시전자를 무시 (생성자에서 호출하면 시전자 설정이 안된다.
	SphereComp->IgnoreActorWhenMoving(GetInstigator(),true);
}

void ASDashProjectile::Explode_Implementation()
{
	// Clear timer if the Explode was already called through another source like OnActorHit
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	//UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());

	EffectComp->DeactivateSystem();
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);

	// Skip base implementation as it will destroy actor (we need to stay alive a bit longer to finish the 2nd timer)
	//Super::Explode_Implementation();
}

void ASDashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (!ensure(ActorToTeleport)) return;


	// ++ 매번 작동되지 않는 이슈가 있어 바닥으로 LineTrace 후 지면으로 위 100.f 정도 올려줌.

	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = TraceStart - FVector(0, 0, 5000.0f); // 아래로 500cm 탐색

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(ActorToTeleport);

	// 지면 찾기
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_WorldStatic, Params))
	{
		// 지면 위 50cm 지점으로 텔레포트
		FVector TeleportLocation = HitResult.ImpactPoint + FVector(0, 0, 100.0f);

		bool bSuccess = ActorToTeleport->TeleportTo(TeleportLocation, ActorToTeleport->GetActorRotation(), false, false);
		UE_LOG(LogTemp, Warning, TEXT("TeleportTo (adjusted to ground) result: %s"), bSuccess ? TEXT("Success") : TEXT("Fail"));
	}
	else
	{
		// 못 찾으면 그냥 현재 위치
		bool bSuccess = ActorToTeleport->TeleportTo(TraceStart, ActorToTeleport->GetActorRotation(), false, false);
		UE_LOG(LogTemp, Warning, TEXT("TeleportTo (fallback) result: %s"), bSuccess ? TEXT("Success") : TEXT("Fail"));
	}

	// 종료 후 비로소 Destroy 
	Destroy();
}


// Called every frame
void ASDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

