// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ASItemChest::ASItemChest()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);
	
	TargetPitch = 110;

	SetReplicates(true);
}

void ASItemChest::OnRep_LidOpened()
{
	float CurrPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch,0,0));
}


void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	UE_LOG(LogTemp,Log,TEXT("Interact_Implementation"));
	bLidOpened = !bLidOpened;
	// 서버에서는 직접 값을 바꾸기 때문에 자동으로 OnRep_LidOpened 가 호출되지 않는다. 따라서 직접 호출해야 한다.
	OnRep_LidOpened();
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// How to DEREPLIFETIME : 서버에서 변경된 값을 클라이언트로 자동 전송
	DOREPLIFETIME(ASItemChest, bLidOpened);
}

