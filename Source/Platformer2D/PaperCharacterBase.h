// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperCharacterBase.generated.h"

class UPaperFlipbook;
/**
 * 
 */
UCLASS()
class PLATFORMER2D_API APaperCharacterBase : public APaperCharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* m_springArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* m_cameraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State Machine")
	class UStateMachineComponent* m_StateMachine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UPaperFlipbook* m_IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UPaperFlipbook* m_RunningAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UPaperFlipbook* m_FallingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UPaperFlipbook* m_JumpingAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UPaperFlipbook* m_DashAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementMechanics)
	int maxJumps = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementMechanics)
	float dashDistance = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementMechanics)
	float dashDuration = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementMechanics)
	float timerCooldown = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementMechanics)
	float wallJumpHorizontalStrength = 4500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementMechanics)
	float grappleStrengthCoefficient = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ObstacleDetection)
	float raycastDistance = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CollisionDetection)
	class UBoxComponent* BoxCollider;

	bool m_pIsDashInCooldown;
	FTimerHandle m_pDashCooldownTimerHandle;
	FTimerHandle m_pDashTimerHandle;
	bool m_pIsDashActivated;
	float m_pDashTimeElapsed;
	FVector m_pDashTargetLocation;
	FVector m_pDashStartLocation;
	int m_pJumpsRemaining;

	FVector m_pGrappableLocation;
	bool m_pCanGrapple;
	bool m_pIsGrappleActivated;

public:
	APaperCharacterBase();

	FORCEINLINE bool IsMovementBlocked() const { return m_pIsDashActivated; }
	FORCEINLINE FVector GetDashVelocity() const { return (m_pDashTargetLocation - m_pDashStartLocation) / dashDuration;}

protected:
	void MoveRight(float value);
	void Dash();

	void Dash_Implementation();
	UFUNCTION()
	void Dash_TimeElapsed();
	void Jump();
	void WallJump(FHitResult& hit);
	void Grapple();
	UFUNCTION()
	void OnDashCooldownTimerOver();
	UFUNCTION()
	void OnDashOver();
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnGrappleDetectionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnGrappleDetectionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual void Tick(float deltaTime) override;
	bool DetectWall(FHitResult& OutHit1);


private:
	
};
