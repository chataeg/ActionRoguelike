// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


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
	
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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


void ASCharacter::PrimaryAttack_TimeElasped()
{
	// How to Debug
	// ensure 은 한번만 트리거된다. 항상 하려면 ensureAlways 사용 shpping에서 ensure 는 사라진다.
	// check 는 잘 안쓴다. 트리거되면 계속 진행이 안 되어서.
	
	if (ensure(ProjectileClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
        
        	// 컨트롤러가 보고 있는 방향과 손 소켓의 위치로 Projectile을 발사할 트랜스폼 설정
        	FTransform SpawnTM = FTransform(GetControlRotation(),HandLocation);
        
        	// 스폰 관련해 파라미터 설정할 구조체
        	FActorSpawnParameters SpawnParams;
        	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        	// 내장된 Instigator 로 시전자 설정 
        	SpawnParams.Instigator = this;
        	
        	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
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




// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	// 바인딩
	// General
	InputComp->BindAction(Input_Move, ETriggerEvent::Triggered,this,&ASCharacter::Move);
	InputComp->BindAction(Input_LookMouse, ETriggerEvent::Triggered,this,&ASCharacter::LookMouse);
	
	InputComp->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAttack);
	InputComp->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ASCharacter::Jump);
	InputComp->BindAction(Input_Interact, ETriggerEvent::Triggered, this, &ASCharacter::Interact);
	
}