// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"


// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
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

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	
	
	UE_LOG(LogTemp,Log,TEXT("USInteractionComponent::PrimaryInteract"));
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	// 캐릭터 가져오기
	AActor* MyOwner = GetOwner();
	
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	
	// FHitResult Hit;
	// bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End ,ObjectQueryParams);

	TArray<FHitResult> Hits;

	float Radius = 30.f;
	
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
		
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits,EyeLocation,End,FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
		
			UE_LOG(LogTemp,Log,TEXT("USInteractionComponent::ActorHit"));
			// 인터페이스를 구현하는지 확인(접두 U 로 해야 함, 사용할 땐 I)
			if(HitActor->Implements<USGameplayInterface>())
				//if(HitActor->GetClass()->ImplementsInterface(USGameplayInterface::StaticClass()))
			{
			
				APawn* MyPawn = Cast<APawn>(MyOwner);

				// 인터페이스 실행은 Execute_메서드이름  으로 한다.
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
		
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint,Radius,32, LineColor, false, 2.0f);
	}
	
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f,0,2.0f);
	
}


