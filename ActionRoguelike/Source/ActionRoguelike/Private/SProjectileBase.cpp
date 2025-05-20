// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"

#include "LocalizationDescriptor.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "UObject/FastReferenceCollector.h"
#include "Camera/CameraShakeBase.h"
#include "ProfilingDebugging/CountersTrace.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(SProjectileBase)

// NOTE: With SparseDataClass feature in use, some properties are replaced with "GetXXX()" which is generated automatically by UHT.
// Example: DamageAmount becomes GetDamageAmount() without this function visible in our own header.

TRACE_DECLARE_INT_COUNTER(COUNTER_GAME_ActiveProjectiles, TEXT("Game/ActiveProjectiles"));


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

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(SphereComp);
	

	
}

void ASProjectileBase::Explode_Implementation()
{
	// Check to make sure we aren't already being 'destroyed'
	// Adding ensure to see if we encounter this situation at all

	 if (ensure(!IsEliminatingGarbage(EGCOptions::None)))
	 {
	 	// Deprecated
	 	//UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(),GetActorRotation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactVFX,GetActorLocation(), GetActorRotation(),FVector(1.0f),true,true,ENCPoolMethod::AutoRelease,true);
		 	
	 	 UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());

	 	// How to PlayWorldCameraShake : InnerRadius -> 이 안에 있으면 흔들림 최대 적용 / OuterRadius -> 흔들림 적용 없음.
	 	 UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), GetImpactShakeInnerRadius(), GetImpactShakeOuterRadius());

		 Destroy();
	}

	
}

void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
		Explode();
}

// How to PostInitialIzeComponents : 생성자 다음에 실행
void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
}

// Called when the game starts or when spawned
void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Can use to fine-tune the pre allocated actor pool by checking how many projectiles are alive during gameplay
	TRACE_COUNTER_INCREMENT(COUNTER_GAME_ActiveProjectiles);
}

void ASProjectileBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	TRACE_COUNTER_DECREMENT(COUNTER_GAME_ActiveProjectiles);
}
// Called every frame
void ASProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//
// #if WITH_EDITOR
// // Only required to convert existing properties already stored in Blueprints into the 'new' system
// void ASProjectileBase::MoveDataToSparseClassDataStruct() const
// {
// 	// make sure we don't overwrite the sparse data if it has been saved already
// 	const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass());
// 	if (BPClass == nullptr || BPClass->bIsSparseClassDataSerializable == true)
// 	{
// 		return;
// 	}
// 	
// 	Super::MoveDataToSparseClassDataStruct();
//
// #if WITH_EDITORONLY_DATA
// 	// Unreal Header Tool (UHT) will create GetMySparseClassData automatically.
// 	FProjectileSparseData* SparseClassData = GetProjectileSparseData();
//
// 	// Modify these lines to include all Sparse Class Data properties.
// 	SparseClassData->ImpactShakeInnerRadius = ImpactShakeInnerRadius_DEPRECATED;
// 	SparseClassData->ImpactShakeOuterRadius = ImpactShakeOuterRadius_DEPRECATED;
// #endif // WITH_EDITORONLY_DATA
// }
// #endif

