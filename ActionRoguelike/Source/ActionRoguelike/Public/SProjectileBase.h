// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraEmitterHandle.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UAudioComponent;
class USoundCue;
class UCameraShakeBase;


/* SparseData to reduce memory footprint, see class description for URL*/
// How to SparseClassData : 상수화가 될 수 있는 데이터를 단일 공유 구조체 인스턴스로 전송해서 사본 하나만 유지. (메모리 절약)
// https://dev.epicgames.com/documentation/ko-kr/unreal-engine/sparse-class-data-in-unreal-engine
USTRUCT(BlueprintType)
struct FProjectileSparseData
{
	GENERATED_BODY()

	FProjectileSparseData()
	: ImpactShakeInnerRadius(0.f),
	ImpactShakeOuterRadius(1500.f)
	{ }
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeOuterRadius;
};


/*
 * Example of Implementing SparseClassData, reduces memory by specifying a set of properties that won't change per-instance. More info: https://docs.unrealengine.com/en-US/sparse-class-data-in-unreal-engine/
 */
// How to ABSTRACT : 클래스를 추상 클래스로 만듦 (객체가 인스턴스화 되지 않는다.)
UCLASS(ABSTRACT, SparseClassDataTypes = ProjectileSparseData) // 'ABSTRACT' marks this class as incomplete, keeping this out of certain dropdowns windows like SpawnActor in Unreal Editor

class ACTIONROGUELIKE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UParticleSystemComponent> EffectComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAudioComponent> AudioComp;

	// Deprecated particleSystem
	// UPROPERTY(EditDefaultsOnly, Category = "Effects")
	// TObjectPtr<UParticleSystem> ImpactVFX;

	UPROPERTY(EditDefaultsOnly, Category= "Effects")
	TObjectPtr<UNiagaraSystem> ImpactVFX;
	
	UPROPERTY(EditDefaultsOnly, Category= "Effects")
	TObjectPtr<USoundCue> ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	TSubclassOf<UCameraShakeBase> ImpactShake;
	
	// 'virtual' so we can override this in child-classes
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// BlueprintCallable to allow child classes to trigger explosions
	// Not required for assignment, useful for expanding in Blueprint later on

	// How to BlueprintNativeEvent : C++ 에서 정의 가능, BP 에서 오버라이드 가능 / virtual 을 선언하지 않아도 가상 함수로 처리된다.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	virtual void PostInitializeComponents() override;

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;




// ### Deprecated ####
// #if WITH_EDITORONLY_DATA
// 	//~ These properties are moving out to the FMySparseClassData struct:
// private:
// 	
// 	UPROPERTY()
// 	float ImpactShakeInnerRadius_DEPRECATED;
//
// 	UPROPERTY()
// 	float ImpactShakeOuterRadius_DEPRECATED;
// #endif
//
// #if WITH_EDITOR
// public:
// 	// ~ This function transfers existing data into FMySparseClassData.
// 	virtual void MoveDataToSparseClassDataStruct() const override;
// #endif
};
