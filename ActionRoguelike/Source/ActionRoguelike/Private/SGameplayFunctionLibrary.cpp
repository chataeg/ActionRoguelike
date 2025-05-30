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
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();
			
			// 방향벡터 반전
			HitComp->AddImpulseAtLocation(Direction * 30000.f, HitResult.ImpactPoint, HitResult.BoneName);

#if WITH_EDITOR
			// 디버그: Direction 벡터 (Trace 방향)
			// MagicProjectile을 발사한 방향
			DrawDebugLine(
				TargetActor->GetWorld(),
				HitResult.ImpactPoint,
				HitResult.ImpactPoint + Direction * 100.0f,
				FColor::Blue,
				false, 10.0f, 0, 2.0f
			);

			// 디버그: -ImpactNormal 벡터 (표면 법선 반대방향)
			// 이 경우에는 hit 한 Bone 메쉬의 법선벡터에 Impulse 를 주기 때문에 MagicProjectile이 가던 방향이 아니다.
			DrawDebugLine(
				TargetActor->GetWorld(),
				HitResult.ImpactPoint,
				HitResult.ImpactPoint - HitResult.ImpactNormal * 100.0f,
				FColor::Red,
				false, 10.0f, 0, 2.0f
			);
#endif
		}
		return true;
	}
	
	return false;
}
