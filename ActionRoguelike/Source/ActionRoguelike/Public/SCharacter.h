// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "SCharacter.generated.h"

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

protected:

	// How to TSubclassof : 에디터에서 특정 클래스슬 지정 가능하게 만드는 템플릿 클래스 타입. (포인터 아님)
	UPROPERTY(EditAnywhere,  Category="Attack")
	TSubclassOf<AActor> PrimaryAttackClass;

	UPROPERTY(EditAnywhere,  Category="Attack")
	TSubclassOf<AActor> SecondaryAttackClass;
	
	UPROPERTY(EditAnywhere,  Category="Attack")
	TSubclassOf<AActor> DashProjectileClass;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	TObjectPtr<UAnimMontage> AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;
	
	FTimerHandle TimerHandle_SecondaryAttack;

	FTimerHandle TimerHandle_Dash;
	

	void PrimaryAttack_TimeElasped();
	void SecondaryAttack_TimeElasped();
	void Dash_TimeElasped();
	
public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	virtual void PostInitializeComponents() override;
	
protected:
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
	//
	// UPROPERTY(EditDefaultsOnly, Category="Input")
	// TObjectPtr<UInputAction> Input_Sprint;
	
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
	
	

	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UNiagaraSystem> CastingEffect;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionInstance& Instance);

	void LookMouse(const FInputActionValue& InputValue);
	
	void LookStick(const FInputActionValue& InputValue);

	void PrimaryAttack();

	void SecondaryAttack();

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);
	
	void Interact();

	void Dash();


	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
