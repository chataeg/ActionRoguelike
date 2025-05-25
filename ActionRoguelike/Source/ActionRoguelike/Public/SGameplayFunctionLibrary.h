// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GamePlay")
	static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount);

	// How to Const : BP 에서는 읽기 전용. 그리고 복사 없이 참조 전달되므로 성능 손실 없이 원본 데이터를 사용할 수 있다.
	// 참고:
	// - const Type&     → Input 핀 (읽기 전용)
	// - Type            → Input 핀 (복사본)
	// - Type&           → Output 핀 (함수가 값을 변경해서 반환할 목적)
	// - UPARAM(ref)     → Input + Output 핀 (입력받고, 결과도 반환)
	// - UPARAM(out)     → Output 핀만 표시 (함수가 값을 설정해서 반환)
	UFUNCTION(BlueprintCallable, Category = "GamePlay")
	static bool ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);

	
	
};
