// Fill out your copyright notice in the Description page of Project Settings.

#include "PaperCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PaperFlipbookComponent.h"
#include "Math/Vector.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "StateMachineComponent.h"

#define COLLISION_GRAPPABLE		ECC_GameTraceChannel1
#define DETECTION_GRAPPABLE		ECC_GameTraceChannel2

#define GP_TAG_IDLE				"PlayerState.Idle"

const FName GrappleSocket = "Grapple Location";

APaperCharacterBase::APaperCharacterBase()
{
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Grapple Detection"));
	BoxCollider->SetupAttachment(GetCapsuleComponent());
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollider->SetCollisionObjectType(DETECTION_GRAPPABLE);
	BoxCollider->SetGenerateOverlapEvents(true);
	BoxCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollider->SetCollisionResponseToChannel(COLLISION_GRAPPABLE, ECR_Overlap);
	BoxCollider->SetBoxExtent(FVector(420.f, 80.f, 280.f));
	BoxCollider->SetRelativeLocation(FVector(0.f, 0.f, 420.f));
	m_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	m_springArm->TargetArmLength = 1200.f;
	m_springArm->SetupAttachment(GetSprite());

	// STATE MACHINE /////////////////////
	m_StateMachine = CreateDefaultSubobject<UStateMachineComponent>(TEXT("State Machine Component"));
	////////////////////////////////

	
	m_cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_cameraComponent->SetupAttachment(m_springArm);

	m_springArm->AddLocalRotation(FRotator(0, 0, -90));
	m_springArm->bUsePawnControlRotation = true;
	m_springArm->bInheritPitch = false;
	m_springArm->bInheritRoll = false;
	m_springArm->bInheritYaw = false;
	
	// Ground movement
	GetCharacterMovement()->JumpZVelocity = 900.f;
	GetCharacterMovement()->MaxAcceleration = 2048;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->BrakingFriction = 10;
	GetCharacterMovement()->GroundFriction = 8.0f;

	// Air movement
	JumpMaxCount = 2;
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->AirControlBoostMultiplier = 2.0f;
	GetCharacterMovement()->FallingLateralFriction = 10.0f;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.f, -1.0f, 0.f));
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	m_pIsDashInCooldown = false;
	m_pIsDashActivated = false;
	m_pJumpsRemaining = maxJumps;
	m_pCanGrapple = false;
	m_pIsGrappleActivated = false;
}

void APaperCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &APaperCharacterBase::OnGrappleDetectionOverlapBegin);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &APaperCharacterBase::OnGrappleDetectionOverlapEnd);
}

void APaperCharacterBase::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (!GetCharacterMovement()->IsFalling())
	{
		m_pJumpsRemaining = maxJumps;
	}
	// ANIMATIONS //////////////////////////////////
	if (!IsMovementBlocked())
	{
		if (GetCharacterMovement()->GetLastUpdateVelocity().Length() == 0.0f)
		{
			GetSprite()->SetFlipbook(m_IdleAnimation);
		}
		else
		{
			if (GetCharacterMovement()->GetLastUpdateVelocity().X > 0)
				GetSprite()->SetWorldRotation(FRotator(0, 0, 0));
			else if (GetCharacterMovement()->GetLastUpdateVelocity().X < 0)
				GetSprite()->SetWorldRotation(FRotator(0, -180, 0));
			if (m_RunningAnimation && GetCharacterMovement()->GetLastUpdateVelocity().Z == 0)
				GetSprite()->SetFlipbook(m_RunningAnimation);
		}
		if (GetCharacterMovement()->IsFalling())
		{
			if (m_FallingAnimation && m_JumpingAnimation)
			{
				if (GetCharacterMovement()->GetLastUpdateVelocity().Z > 0 && m_JumpingAnimation)
					GetSprite()->SetFlipbook(m_JumpingAnimation);
				else if (GetCharacterMovement()->GetLastUpdateVelocity().Z < 0 && m_FallingAnimation)
					GetSprite()->SetFlipbook(m_FallingAnimation);
			}
			else if (!m_FallingAnimation && GetCharacterMovement()->GetLastUpdateVelocity().Z != 0)
			{
				GetSprite()->SetFlipbook(m_JumpingAnimation);
			}

		}
	}
	//////////////////////////////////////////////

	// Dash Handling ////////////////////////////////////////////////////////////
	if (m_pIsDashActivated)
	{
		m_pIsDashInCooldown = true;
		if (m_pDashTimeElapsed < dashDuration)
		{
			LaunchCharacter(GetDashVelocity(), true, true);
			m_pDashTimeElapsed += deltaTime;
		}
		else
		{
			m_pIsDashActivated = false;
			GetWorldTimerManager().SetTimer(m_pDashCooldownTimerHandle, this, &APaperCharacterBase::OnDashCooldownTimerOver, timerCooldown, false);
		}
	}
	/////////////////////////////////////////////////////////////////////////

	if (m_pIsGrappleActivated)
	{
		DrawDebugLine(GetWorld(), GetActorLocation(), m_pGrappableLocation, FColor::Red, false, -1.f, 0U, 5.0f);
	}
}


void APaperCharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APaperCharacterBase::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APaperCharacter::StopJumping);
	// PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APaperCharacterBase::Dash_Implementation);
	PlayerInputComponent->BindAction("Grapple", IE_Pressed, this, &APaperCharacterBase::Grapple);
	PlayerInputComponent->BindAxis("MoveRight", this, &APaperCharacterBase::MoveRight);
}

void APaperCharacterBase::OnGrappleDetectionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionObjectType() == COLLISION_GRAPPABLE)
	{
		m_pCanGrapple = true;
		m_pGrappableLocation = OtherComp->GetSocketLocation("Grapple Location");
		UE_LOG(LogTemp, Warning, TEXT("Grapple Location: %s"), *m_pGrappableLocation.ToString());
	}
}

void APaperCharacterBase::OnGrappleDetectionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->GetCollisionObjectType() == COLLISION_GRAPPABLE)
	{
		m_pCanGrapple = false;
		//m_pGrappableLocation = OtherComp->GetSocketLocation("Grapple Location");
		m_pIsGrappleActivated = false;
		UE_LOG(LogTemp, Warning, TEXT("Grapple Detection Overlap End"));
	}
}

void APaperCharacterBase::MoveRight(float value)
{
	AddMovementInput(FVector(1.0, 0, 0), value);
}
#pragma region DASH
void APaperCharacterBase::Dash()
{
	if (!m_pIsDashInCooldown && GetVelocity().X != 0.f)
	{
		if (m_DashAnimation)
			GetSprite()->SetFlipbook(m_DashAnimation);
		m_pDashTimeElapsed = 0.f;
		m_pIsDashActivated = true;
		m_pDashTargetLocation = GetActorLocation() + GetSprite()->GetForwardVector() * dashDistance;
		m_pDashStartLocation = GetActorLocation();
	}
}

void APaperCharacterBase::Dash_Implementation()
{
	if (!m_pIsDashInCooldown && GetVelocity().X != 0.f)
	{
		if (m_DashAnimation)
			GetSprite()->SetFlipbook(m_DashAnimation);
		m_pDashTimeElapsed = 0.f;
		// m_pIsDashActivated = true;
		m_pDashTargetLocation = GetActorLocation() + GetSprite()->GetForwardVector() * dashDistance;
		m_pDashStartLocation = GetActorLocation();
	
		// GetCharacterMovement()->bUseSeparateBrakingFriction = false;
		// GetCharacterMovement()->BrakingFriction = 0;
		m_pIsDashInCooldown = true;
		
		GetWorldTimerManager().SetTimer(m_pDashTimerHandle, this, &APaperCharacterBase::Dash_TimeElapsed, 0.016, true);
	}
	// FApp::GetDeltaTime();
	
}

void APaperCharacterBase::Dash_TimeElapsed()
{
	float timeElapsed = GetWorldTimerManager().GetTimerElapsed(m_pDashTimerHandle);
	m_pDashTimeElapsed += timeElapsed;
	UE_LOG(LogTemp, Warning, TEXT("Dash Timer: %f"), m_pDashTimeElapsed)
	if(m_pDashTimeElapsed >= dashDuration)
	{
		GetWorldTimerManager().ClearTimer(m_pDashTimerHandle);
		GetWorldTimerManager().SetTimer(m_pDashTimerHandle, this, &APaperCharacterBase::OnDashCooldownTimerOver, timerCooldown, false);
	}
	LaunchCharacter(GetDashVelocity(), true, true);
}

void APaperCharacterBase::OnDashCooldownTimerOver()
{
	m_pIsDashInCooldown = false;
	GetWorldTimerManager().ClearTimer(m_pDashCooldownTimerHandle);
}

void APaperCharacterBase::OnDashOver()
{
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->BrakingFriction = 10;
	GetCharacterMovement()->GroundFriction = 8.0f;
}

#pragma endregion
void APaperCharacterBase::Grapple()
{
	if (m_pCanGrapple)
	{
		FVector direction = m_pGrappableLocation - GetActorLocation();
		m_pIsGrappleActivated = true;
		direction.Y = 0.f;
		//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + direction*grappleStrengthCoefficient, FColor::Red, true, 5.0f);
		LaunchCharacter(direction * grappleStrengthCoefficient, true, true);
	}
}

void APaperCharacterBase::Jump()
{
	FHitResult hit;
	if (DetectWall(hit) && GetCharacterMovement()->IsFalling())
	{
		WallJump(hit);
		return;
	}
	if (m_pJumpsRemaining > 0)
	{
		APaperCharacter::Jump();
		m_pJumpsRemaining--;
	}
}

void APaperCharacterBase::WallJump(FHitResult& hit)
{
	float sign = FMath::Sign(GetActorLocation().X - hit.Location.X);
	FVector launchVelocity = FVector(sign * wallJumpHorizontalStrength, 0.0f, GetCharacterMovement()->JumpZVelocity);
	LaunchCharacter(launchVelocity, true, true);
}

bool APaperCharacterBase::DetectWall(FHitResult& OutHit)
{
	FCollisionQueryParams params;
	params.AddIgnoredActor(this->GetOwner()); 
	float radius = GetCapsuleComponent()->GetScaledCapsuleRadius() + raycastDistance;
	//FVector startPos = GetActorLocation() - GetSprite()->GetForwardVector() * radius;
	FVector startPos = GetActorLocation();
	FVector endPos = GetActorLocation() + GetSprite()->GetForwardVector() * radius;
	DrawDebugLine(GetWorld(), startPos, endPos, FColor::Red);
	return GetWorld()->LineTraceSingleByChannel(OutHit, startPos, endPos, ECC_Visibility, params);

}





