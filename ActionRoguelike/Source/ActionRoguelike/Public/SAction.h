// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.generated.h"

class UWorld;

/**
 * 
 */
// How to Blueprintable : C++ 클래스의 블루프린트 자식 클래스를 만들 수 있게 해줌.
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);
	
	// How to FName : Name Pool 에 문자열을 등록해 인덱스끼리 비교하는 해시 시스템이기 때문에 빠르다.
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;


	// BP 에서 접근하기 위해서 오버라이딩 : BP 의 여러 함수들을 접근하기 위해서
	UFUNCTION()
	virtual class UWorld* GetWorld() const override;

	
};
