// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "SAttributeComponent.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USAttributeComponent;
// Deprecated 
class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components")
	TObjectPtr<USAttributeComponent> AttributeComp;

	virtual void PostInitializeComponents() override;

	void SetTargetActor(AActor* NewTarget);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	FGenericTeamId TeamId;
	

public: 
	virtual FGenericTeamId GetGenericTeamId() const override;
	
	

	// How to FSeePawnDelegate : Deprecated
	// UFUNCTION()
	// void OnPawnSeen(APawn* Pawn);

	

};
