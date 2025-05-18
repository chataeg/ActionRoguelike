// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"

#include "SAttributeComponent.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	AttributeComponent->OnHealthChanged.AddDynamic(this,&ASTargetDummy::OnHealthChanged);

}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.0f)
	{
		// How to SetScalarParameterValueOnMaterials : Material 에 파라미터로 지정한 파라미터 이름에 값을 전달
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit",GetWorld()->TimeSeconds);
	}
}
