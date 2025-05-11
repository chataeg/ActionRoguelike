// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetSimulatePhysics(true);
	RootComponent = StaticMesh;
	
	//StaticMesh->
	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForce->SetupAttachment(RootComponent);

	RadialForce->SetAutoActivate(false);

	RadialForce->Radius = 750.f;
	RadialForce->ImpulseStrength = 2500.f;
	// 다른 오브젝트의 원래 속도를 무시함 (질량을 무시)
	RadialForce->bImpulseVelChange = true;

	// WorldDynamic 에도 영향을 줄 수 있게 변경
	RadialForce->AddCollisionChannelToAffect(ECC_WorldDynamic);
	

	
	
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	// BeginPlay 이전에 호출된다.
	// 무조건 부모 호출
	Super::PostInitializeComponents();

	// 원래 존재하는 DELEGATE 에 바인딩
	// Beginplay 에 넣어도 되지만 hotreload 시 안 될 수 있음 (beginplay 전에 construct 되기 때문) 
	StaticMesh->OnComponentHit.AddDynamic(this,&ASExplosiveBarrel::OnHit);
	
}

void ASExplosiveBarrel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector NormalImpulse, const FHitResult& Hit)
{
	
	UE_LOG(LogTemp,Log,TEXT("ASExplosiveBarrel::OnHit"));
	
	RadialForce->FireImpulse();
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

