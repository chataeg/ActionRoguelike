// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"

#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);
		
		// UNiagaraFunctionLibrary::SpawnSystemAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector,
		// 											 FRotator::ZeroRotator,
		// 											 EAttachLocation::SnapToTarget, true, true,
		// 											 ENCPoolMethod::AutoRelease);

		if (Character->HasAuthority())
		{
			FTimerHandle TimerHandle_AttackDelay;
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "AttackDelay_Elasped", Character);

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
		}
	}

	
}

void USAction_ProjectileAttack::AttackDelay_Elasped(ACharacter* InstigatorChracter)
{
	if (ensure(ProjectileClass))
	{
		FVector HandLocation = InstigatorChracter->GetMesh()->GetSocketLocation("Muzzle_01");

		// 항상 스폰하게 하고 시전자 설정
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorChracter;

		// Sweep 구체 크기 설정
		FCollisionShape Shape;
		Shape.SetSphere(20.f);

		// 충돌 속성 설정
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorChracter);

		// 충돌 쿼리 설정
		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		// 카메라와 충돌을 막기 위해 30.f 앞에서 발사
		FVector TraceStart = InstigatorChracter->GetPawnViewLocation() + InstigatorChracter->GetControlRotation().Vector() + 30.f;
		
		FVector TraceEnd = TraceStart + (InstigatorChracter->GetControlRotation().Vector() * 50000.f);

		// Hit 시 정보를 저장할 구조체
		FHitResult Hit;

		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);


		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

		// How to Niagara : PublicDependencyModuleNames 에 Niagara 추가 필요. 기본적으로 cascade particle system 의 최신 버젼이라 생각중.
		// 	 UNiagaraFunctionLibrary::SpawnSystemAttached(
		// 	CastingEffect,                    // 1. 이펙트 에셋
		// 	GetMesh(),                        // 2. 붙일 대상 컴포넌트
		// 	TEXT("Muzzle_01"),                // 3. 붙일 소켓 이름
		// 	FVector::ZeroVector,              // 4. 위치 오프셋
		// 	FRotator::ZeroRotator,            // 5. 회전 오프셋
		// 	EAttachLocation::SnapToTarget,    // 6. 위치 기준 타입
		// 	true,                             // 7. 자동 파괴 여부
		// 	true,                             // 8. 자동 활성화 여부
		// 	ENCPoolMethod::AutoRelease        // 9. 풀링 방식
		// );

		UNiagaraFunctionLibrary::SpawnSystemAttached(CastingEffect, InstigatorChracter->GetMesh(),TEXT("Muzzle_01"), FVector::ZeroVector,
		                                             FRotator::ZeroRotator,
		                                             EAttachLocation::SnapToTarget, true, true,
		                                             ENCPoolMethod::AutoRelease);


		FVector DebugEnd = Hit.bBlockingHit ? Hit.Location : TraceEnd;
		DrawDebugLine(GetWorld(), HandLocation, DebugEnd, FColor::Red, false, 2.0f, 0, 1.0f);
		//UE_LOG(LogTemp, Warning, TEXT("Instigator: %s"), *GetInstigator()->GetName());
	}
	
	StopAction(InstigatorChracter);
}


