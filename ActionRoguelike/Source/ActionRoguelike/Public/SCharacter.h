// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "SCharacter.generated.h"

class USActionComponent;
class UAIPerceptionStimuliSourceComponent;
class UAIPerceptionComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;
class UNiagaraSystem;


UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	virtual void PostInitializeComponents() override;
	
protected:
	FGenericTeamId TeamId;
	
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Move;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_LookMouse;
	
	// UPROPERTY(EditDefaultsOnly, Category="Input")
	// TObjectPtr<UInputAction> Input_LookStick;
	//
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Jump;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Interact;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Sprint;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Dash;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_PrimaryAttack;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_SecondaryAttack;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USInteractionComponent> InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USAttributeComponent> AttributeComp;


	// How to UAIPerceptionStimuliSourceComponent
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAIPerceptionStimuliSourceComponent> PerceptionStimuliComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USActionComponent> ActionComp;
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionInstance& Instance);

	void SprintStart();

	void SprintStop();

	void LookMouse(const FInputActionValue& InputValue);
	
	void LookStick(const FInputActionValue& InputValue);

	void PrimaryAttack();

	void SecondaryAttack();

	
	void Interact();

	void Dash();
	

	


	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);


	// 크로스헤어(카메라 중앙) 기준으로 행동해야 하므로 엔진 API 오버라이딩 해서 사용.
	virtual FVector GetPawnViewLocation() const override;

	FRotator GetPawnViewRotation() const;
	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

	// How to Exec : 콘솔 커맨드로 만들 수 있음.
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
	
};

