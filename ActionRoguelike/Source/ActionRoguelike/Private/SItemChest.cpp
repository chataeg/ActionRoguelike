// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Components/StaticMeshComponent.h"

ASItemChest::ASItemChest()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);
	
	TargetPitch = 110;
}


void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	// BP 에서 오버라이드 하였기 때문에 동작하지 않는다.
	UE_LOG(LogTemp,Log,TEXT("Interact_Implementation"));
	LidMesh->SetRelativeRotation(FRotator(TargetPitch,0,0));
}
