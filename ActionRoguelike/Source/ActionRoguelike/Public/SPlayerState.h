// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

class ASPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, NewCredits, int32, Delta);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

	ASPlayerState();
	
protected:

	// How to Transient : 세이브 파일에 저장되지 않는 일시적인 정보가 된다. 언리얼의 직렬화 시스템에서 제외된다.
	UPROPERTY(Transient, EditDefaultsOnly, Category = "Credits")
	int32 Credits;

	
public:


	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	
	// 블루프린트에서 바인딩 되어 있음 ~~ UI에 Credits 값을 갱신 시켜 준다.
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;
	
};
