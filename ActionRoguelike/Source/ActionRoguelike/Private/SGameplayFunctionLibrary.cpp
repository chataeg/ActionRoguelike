// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"

#include "SAttributeComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor,DamageAmount))
	{
		// How to UPrimitiveComponent : 충돌, 물리, 렌더링 기능을 가지는 모든 컴포넌트들의 베이스 클래스
		//UObject
		//└── UActorComponent
		//	└── USceneComponent
		//		└── UPrimitiveComponent  ← 여기!
		//			├── UStaticMeshComponent
		//			├── USkeletalMeshComponent
		//			├── UCapsuleComponent
		//			├── USphereComponent

		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			// 방향벡터 반전
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	
	return false;
}
