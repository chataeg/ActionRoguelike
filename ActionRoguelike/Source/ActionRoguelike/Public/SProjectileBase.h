// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UAudioComponent;
class USoundCue;

UCLASS()
class ACTIONROGUELIKE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UParticleSystemComponent> EffectComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAudioComponent> AudioComp;

	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> ImpactVFX;

	UPROPERTY(EditDefaultsOnly, Category= "Effects")
	TObjectPtr<USoundCue> ImpactSound;
	
	// 'virtual' so we can override this in child-classes
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// BlueprintCallable to allow child classes to trigger explosions
	// Not required for assignment, useful for expanding in Blueprint later on

	// How to BlueprintNativeEvent : C++ 에서 정의 가능, BP 에서 오버라이드 가능
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	virtual void PostInitializeComponents() override;

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
