// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values

ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// 캐릭터 / 캐릭터 메쉬가 이동 방향으로 자동 회전
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// 컨트롤러 회전을 캐릭터가 따라가지 않게 함 (카메라와 입력 분리)
	bUseControllerRotationYaw = false;
	
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	// 카메라가 컨트롤러 회전을 따라감
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	// 부모(캐릭터) 회전을 무시하고 절대 좌표로 회전함
	SpringArmComp->SetUsingAbsoluteRotation(true);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	// 상호작용 하게 해주는 액터컴포넌트 부착
	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComp"));

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this,&ASCharacter::OnHealthChanged);
	
}



// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
// 플레이어 컨트롤러가 폰을 소유했을 때 자동적으로 한번 호출된다.
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PC = GetController<APlayerController>();
	const ULocalPlayer* LP = PC->GetLocalPlayer();

	// 입력 서브시스템을 가져오고 check
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	
	Subsystem->ClearAllMappings();

	Subsystem->AddMappingContext(DefaultInputMapping,0);

	UEnhancedInputComponent* InputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// input binding바인딩
	// How to EnhancedInput : BP에서 InputAction 생성 뒤 C++ 에서 해당 입력에 따른 함수를 바인딩 해주어야 함.
	InputComp->BindAction(Input_Move, ETriggerEvent::Triggered,this,&ASCharacter::Move);
	InputComp->BindAction(Input_LookMouse, ETriggerEvent::Triggered,this,&ASCharacter::LookMouse);
	
	InputComp->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAttack);
	InputComp->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ASCharacter::Jump);
	InputComp->BindAction(Input_Interact, ETriggerEvent::Triggered, this, &ASCharacter::Interact);
	InputComp->BindAction(Input_SecondaryAttack, ETriggerEvent::Triggered, this, &ASCharacter::SecondaryAttack);
	InputComp->BindAction(Input_Dash, ETriggerEvent::Triggered, this, &ASCharacter::Dash);
	
}

void ASCharacter::Move(const FInputActionInstance& Instance)
{
	//UE_LOG(LogTemp,Log,TEXT("ASCharacter::Move"));
	// 컨트롤러 yaw 회전값 가져오기
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// 입력으로부터 값 가져오기
	const FVector2d AxisValue = Instance.GetValue().Get<FVector2d>();

	// 앞 뒤 이동
	AddMovementInput(ControlRot.Vector(),AxisValue.Y);
	
	// 오른쪽 & 왼쪽 이동
	const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector,AxisValue.X);

	// 아래처럼 작성도 가능하다. (똑같이 동작함.)
	// const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	// AddMovementInput(GetActorForwardVector(),AxisValue.Y);
	// AddMovementInput(GetActorRightVector(),AxisValue.X);
	
}

void ASCharacter::LookMouse(const FInputActionValue& InputValue)
{
	//UE_LOG(LogTemp,Log,TEXT("ASCharacter::LookMouse"));
	const FVector2D Value = InputValue.Get<FVector2D>();
	
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

void ASCharacter::LookStick(const FInputActionValue& InputValue)
{
}

void ASCharacter::PrimaryAttack()
{
	//UE_LOG(LogTemp,Log,TEXT("ASCharacter::PrimaryAttack"));
	// AnimMontage 플레이
	PlayAnimMontage(AttackAnim);

	// 0.2초가 지나면 메서드가 불리게 한다.
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack,this,&ASCharacter::PrimaryAttack_TimeElasped,0.2f);

	// 타이머 종료 방법 
	// GetWorldTimerManager().ClearTimer(TimerHanlde_PrimaryAttack);
}

void ASCharacter::SecondaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_SecondaryAttack,this,&ASCharacter::SecondaryAttack_TimeElasped,0.2f);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{

	// How to Debug
	// ensure 은 한번만 트리거된다. 항상 하려면 ensureAlways 사용 shpping에서 ensure 는 사라진다.
	// check 는 잘 안쓴다. 트리거되면 계속 진행이 안 되고 abort 해서.
	
	// if (ensure(ClassToSpawn))
	// {
	// 	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	// 	// FRotator HandRotation = GetMesh()->GetSocketRotation(("Muzzle_o1"));
	// 	// 컨트롤러가 보고 있는 방향과 손 소켓의 위치로 Projectile을 발사할 트랜스폼 설정
	// 	// AssignMent 2 - 크로스헤어에 맞게 보정
 //  
	// 	FVector PawnLocation = CameraComp->GetComponentLocation();
	// 	
	// 	
	// 	FHitResult Hit;
	// 	FVector End = CameraComp->GetComponentLocation() + (CameraComp->GetComponentRotation().Vector() * 50000);
	// 	FCollisionObjectQueryParams ObjectQueryParams;
	// 	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	// 	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	// 	
	// 	if (GetWorld()->LineTraceSingleByObjectType(Hit,PawnLocation, End, ObjectQueryParams))
	// 	{
	// 		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation,Hit.ImpactPoint);
 //  
	// 		FTransform SpawnTM = FTransform(NewRotation,HandLocation);
	//        
	// 		// 스폰 관련해 파라미터 설정할 구조체
	// 		FActorSpawnParameters SpawnParams;
	// 		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// 		// 내장된 Instigator 로 시전자 설정 
	// 		SpawnParams.Instigator = this;
 //       		
	// 		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	// 	
	// 	}
	// 	DrawDebugLine(GetWorld(), HandLocation, Hit.Location, FColor::Red, false, 2.0f, 0, 1.0f);
	// }

	// How to LineTrace
	// How to Sweep
	if (ensure(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		// 항상 스폰하게 하고 시전자 설정
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		// Sweep 구체 크기 설정
		FCollisionShape Shape;
		Shape.SetSphere(20.f);

		// 충돌 속성 설정
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		// 충돌 쿼리 설정
		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		// 카메라와 충돌을 막기 위해 30.f 앞에서 발사
		FVector TraceStart = CameraComp->GetComponentLocation() + GetControlRotation().Vector() * 30.f;
	
		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 50000);

		// Hit 시 정보를 저장할 구조체
		FHitResult Hit;
	
		if (GetWorld()->SweepSingleByObjectType(Hit,TraceStart,TraceEnd,FQuat::Identity, ObjParams,Shape,Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}
	
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd-HandLocation).Rotator();
	
		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
		
		FVector DebugEnd = Hit.bBlockingHit ? Hit.Location : TraceEnd;
		DrawDebugLine(GetWorld(), HandLocation, DebugEnd, FColor::Red, false, 2.0f, 0, 1.0f);
		//UE_LOG(LogTemp, Warning, TEXT("Instigator: %s"), *GetInstigator()->GetName());
	}
}


void ASCharacter::PrimaryAttack_TimeElasped()
{
	SpawnProjectile(PrimaryAttackClass);
}
void ASCharacter::SecondaryAttack_TimeElasped()
{
	SpawnProjectile(SecondaryAttackClass);
}
void ASCharacter::Interact()
{
	//UE_LOG(LogTemp,Log,TEXT("ASCharacter::PrimaryInteract"));

	// nullptr 일 경우는 없지만 (생명 주기 때문에) 안전하게 널체크
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::Dash()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_Dash,this,&ASCharacter::Dash_TimeElasped,0.2f);
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
                                  float Delta)
{

	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
		
	}

	
	if (NewHealth <= 0.0f && Delta< 0.0f)
	{
		// How to DisableInput
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

void ASCharacter::Dash_TimeElasped()
{
	SpawnProjectile(DashProjectileClass);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

