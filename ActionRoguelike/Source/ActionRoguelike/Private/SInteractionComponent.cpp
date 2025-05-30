// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "SWorldUserWidget.h"
#include "Blueprint/UserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."),ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 30.f;
	TraceDistance = 500.f;
	CollisionChannel = ECC_WorldDynamic;	
}



// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
}


void USInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	
	// Interaction Logic
	
	UE_LOG(LogTemp,Log,TEXT("USInteractionComponent::PrimaryInteract"));
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	// 캐릭터 / 오너  가져오기
	AActor* MyOwner = GetOwner();
	
	FVector EyeLocation;
	FRotator EyeRotation;
	// eye 의 Location / Rotation 을 가져옴
	MyOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);
	
	// LineTrace / RayCast 가 끝날 위치
	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);


	// 단일 LineTraace
	// FHitResult Hit;
	// bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End ,ObjectQueryParams);

	// Sweep
	TArray<FHitResult> Hits;
	
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
		
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits,EyeLocation,End,FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	// Clear ref before tryting to fill
	FocusedActor = nullptr;

	
	
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			// 인터페이스를 구현하는지 확인(접두 U 로 해야 함, 사용할 땐 I)
			if(HitActor->Implements<USGameplayInterface>())
				//if(HitActor->GetClass()->ImplementsInterface(USGameplayInterface::StaticClass()))
			{
		
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
	
	
	
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f,0,2.0f);
	}
}


void USInteractionComponent::PrimaryInteract()
{

	if (FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1,1.0f, FColor::Red, "No Focus Actor to Interact");
		return;
	}
	// Pawn 으로 강제 형변환. 형변환시 불가하면 nullptr 이 된다. 
	APawn* MyPawn = Cast<APawn>(GetOwner());

	// 인터페이스 실행은 Execute_메서드이름  으로 한다.
	ISGameplayInterface::Execute_Interact(FocusedActor, MyPawn);

		
}

