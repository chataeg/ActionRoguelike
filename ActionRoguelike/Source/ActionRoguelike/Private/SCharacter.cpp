// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
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
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::Move(const FInputActionInstance& Instance)
{

	UE_LOG(LogTemp,Log,TEXT("ASCharacter::Move"));
	
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
	UE_LOG(LogTemp,Log,TEXT("ASCharacter::LookMouse"));
	const FVector2D Value = InputValue.Get<FVector2D>();
	
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

void ASCharacter::LookStick(const FInputActionValue& InputValue)
{
	
}

void ASCharacter::PrimaryAttack()
{
	UE_LOG(LogTemp,Log,TEXT("ASCharacter::PrimaryAttack"));
	
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FTransform SpawnTM = FTransform(GetControlRotation(),HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM,SpawnParams);
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
	
	
	
}