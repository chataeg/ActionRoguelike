// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "SAttributeComponent.h"
#include "SPowerup_Credits.h"
#include "SWorldUserWidget.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISense_Damage.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	TeamId = TEAM_ID_BOTS;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	//TimeToHitParamName = "TimeToHit";
	HitFlash_CustomPrimitiveIndex = 0;
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	// Migrated to AIPerceptionComponent
	//AIPerceptionComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this,&ASAICharacter::OnHealthChanged);
	
	
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
			
		//DrawDebugString(GetWorld(),GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true );
					
	}
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
                                    float Delta)
{
	
	if (Delta < 0.0f)
	{


		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}	
		}

		
		GetMesh()->SetCustomPrimitiveDataFloat(HitFlash_CustomPrimitiveIndex, GetWorld()->TimeSeconds);


		// Died
		if (NewHealth <= 0.0f)
		{
			// How to Stop AI , AI DEAD

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Instigator = this;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			GetWorld()->SpawnActor<ASPowerup_Credits>(GetActorLocation(), GetActorRotation(), SpawnParameters);
			
			// Stopping BT
			AAIController* AIC = Cast<AAIController>(GetController());

			if (AIC)
			{
				// How to BrainComponent : BrainComponent는 Behavior Tree 의 부모
				// AAIController
				// └── UBrainComponent
				// 	  ├── UBehaviorTreeComponent
				// 	  └── UStateTreeComponent
				
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			
			// Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			

			
			// Set Lifespan

			SetLifeSpan(10.0f);
		}
		else
		{
			// How to AISense_Damage : ReportDamageEvent 호출해서 간편하게 지정
			if (IsValid(InstigatorActor))
			{
				UAISense_Damage::ReportDamageEvent(this,this,InstigatorActor,FMath::Abs(Delta),
					InstigatorActor->GetActorLocation(),GetActorLocation());
			
			}
		}
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

