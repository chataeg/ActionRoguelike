// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = MeshComp;

	RespawnTime = 10.f;
	bIsActive = true;
	

}

void ASPowerupActor::HideAndCooldownPowerUp()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	bIsActive = false;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_PowerupPotion,this,&ASPowerupActor::ShowAndCoolEndPowerUp, RespawnTime);
	
}

void ASPowerupActor::ShowAndCoolEndPowerUp()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	bIsActive = true;
}

bool ASPowerupActor::IsActive() const
{
	return bIsActive;
}



