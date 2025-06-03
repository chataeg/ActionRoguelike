// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TargetPitch;

	// How to Implement Interface
	// BlueprintNative 메타정보로 인터페이스를 선언하였기 때문에 _Implementation 을 작성해주어야 한다. (BP에서도 사용)
	// 그냥 사용한다면 Interact 로 사용 가능.
	void Interact_Implementation(APawn* InstigatorPawn);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Sets default values for this actor's properties
	ASItemChest();

protected:

	// How to Replicate property : Repliacted 명시해줘야함. 
	//UPROPERTY(Replicated, BlueprintReadOnly)
	// How to RepNotify / How to ReplicateedUsing : ReplicatedUsing 명시
	UPROPERTY(ReplicatedUsing = "OnRep_LidOpened", BlueprintReadOnly)
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> LidMesh;
};
