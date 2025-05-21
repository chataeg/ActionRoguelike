// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
	
	RespawnTime = 10.f;
	

}

void ASPowerupActor::HideAndCooldownPowerUp()
{
	SetPowerupState(false);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_PowerupPotion,this,&ASPowerupActor::ShowPowerUp, RespawnTime);
}


void ASPowerupActor::ShowPowerUp()
{
	SetPowerupState(true);
}

void ASPowerupActor::SetPowerupState(bool bNewIsActive)
{
	SetActorEnableCollision(true);
	// How to PropagateToChildren : 부모에서 자식까지 쭉 내려가면서 불리언을 바꿔줄 수 있다.
	RootComponent->SetVisibility(bNewIsActive, true);
}



