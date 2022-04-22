// Fill out your copyright notice in the Description page of Project Settings.


#include "PCharacter.h"

#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
APCharacter::APCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(53.0f);
	GetCapsuleComponent()->SetCapsuleRadius(53.0f);
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->TargetArmLength = 1200.0f;
	SpringArm->SetupAttachment(GetSprite());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArm);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	StaticMesh->SetupAttachment(GetSprite());

	SetupMovementComponent();
	bCanDoubleJump = true;
	bHasDoubleJumped = false;
	bJumpBuffered = false;
	CoyoteTime = 0.25f;
	JumpBufferDuration = 0.1f;
}
// Called when the game starts or when spawned
void APCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("JumpCurrentCount = %d"), JumpCurrentCount);
}

// Called to bind functionality to input
void APCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APaperCharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &APCharacter::MoveRight);
}

void APCharacter::SetupMovementComponent()
{
	JumpMaxHoldTime = 0.25f;
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->MaxAcceleration = 4096.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	GetCharacterMovement()->BrakingFriction = 6.0f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->bApplyGravityWhileJumping = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);
}


void APCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if(PrevMovementMode == EMovementMode::MOVE_Walking && GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		UE_LOG(LogTemp, Warning, TEXT("Coyote Timer Started"));
		GetWorldTimerManager().SetTimer(CoyoteJumpTimerHandle, this, &APCharacter::CoyoteTimerElapsed, CoyoteTime);
	}
	else if(PrevMovementMode == EMovementMode::MOVE_Falling && GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
	{
		GetWorldTimerManager().ClearTimer(CoyoteJumpTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Landed, clearing coyote timer"));
		bHasDoubleJumped = false;

		if(bJumpBuffered)
		{
			Jump();
			bJumpBuffered = false;
		}
	}
}

void APCharacter::Jump()
{
	if(CanJumpInternal_Implementation())
		Super::Jump();
	else if(bCanDoubleJump && !bHasDoubleJumped)
	{
		const double ZVelocity = GetCharacterMovement()->JumpZVelocity;
		LaunchCharacter(FVector(0, 0, ZVelocity), false, true);
		bHasDoubleJumped = true;
	}
	else if(GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		bJumpBuffered = true;
		GetWorldTimerManager().SetTimer(JumpBufferTimerHandle, this, &APCharacter::JumpBufferTimerElapsed, JumpBufferDuration);
	}
}

void APCharacter::MoveRight(float X)
{
	AddMovementInput(FVector(1.0, 0, 0), X);
}

bool APCharacter::CanJumpInternal_Implementation() const
{
	// Ensure that the CharacterMovement state is valid
	bool bJumpIsAllowed = GetCharacterMovement()->CanAttemptJump();

	if (bJumpIsAllowed)
	{
		// Ensure JumpHoldTime and JumpCount are valid.
		if (!bWasJumping || GetJumpMaxHoldTime() <= 0.0f)
		{
			bJumpIsAllowed = JumpCurrentCount < JumpMaxCount;
		}
		else
		{
			// Only consider JumpKeyHoldTime as long as:
			// A) The jump limit hasn't been met OR
			// B) The jump limit has been met AND we were already jumping
			const bool bJumpKeyHeld = (bPressedJump && JumpKeyHoldTime < GetJumpMaxHoldTime());
			bJumpIsAllowed = bJumpKeyHeld &&
				((JumpCurrentCount < JumpMaxCount) || (bWasJumping && JumpCurrentCount == JumpMaxCount));
		}
	}
	
	const bool timerElapsed = GetWorldTimerManager().GetTimerRemaining(CoyoteJumpTimerHandle) > 0.0f;
	return timerElapsed || bJumpIsAllowed;
}

void APCharacter::CoyoteTimerElapsed()
{
	UE_LOG(LogTemp, Warning, TEXT("Coyote Timer Elapsed"));
}

void APCharacter::JumpBufferTimerElapsed()
{
	bJumpBuffered = false;
}



