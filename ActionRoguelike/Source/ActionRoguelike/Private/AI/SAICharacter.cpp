// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	 AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	
	TeamId = TEAM_ID_BOTS;
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	// Migrated to AIPerceptionComponent
	//AIPerceptionComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this,&ASAICharacter::OnHealthChanged);
	
	
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	
	if (NewHealth <= 0.0f && Delta< 0.0f)
	{
		// How to Stop AI , AI DEAD

		// AAIController* AIC = Cast<AAIController>(GetController());
		//
		// if (AIC)
		// {
		// 	AIC->StopMovement();
		// 	AIC->UnPossess();
		// 	
		// 	AIC->GetAIPerceptionComponent()->Deactivate();
		// 	
		// }
		//
		// GetCharacterMovement()->DisableMovement();
		// GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//
		// SetActorTickEnabled(false);
	}
}

FGenericTeamId ASAICharacter::GetGenericTeamId() const
{
	return TeamId;
}

// void ASAICharacter::OnPawnSeen(APawn* Pawn)
// {
//
// 	AAIController* AIC = Cast<AAIController>(GetController());
// 	if (AIC)
// 	{
// 		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
//
// 		BBComp->SetValueAsObject("TargetActor", Pawn);
//
// 		DrawDebugString(GetWorld(),GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true );
// 		
//
// 		
// 	}
// 	
// }

