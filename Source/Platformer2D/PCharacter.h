// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "GameFramework/Character.h"
#include "PCharacter.generated.h"

UCLASS()
class PLATFORMER2D_API APCharacter : public APaperCharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Primitives)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category="Jump")
	float CoyoteTime;
	UPROPERTY(EditDefaultsOnly, Category="Jump")
	bool bCanDoubleJump;
	UPROPERTY(EditDefaultsOnly, Category="Jump")
	float JumpBufferDuration;

	FTimerHandle JumpBufferTimerHandle;
	FTimerHandle CoyoteJumpTimerHandle;

	bool bJumpBuffered;
public:
	// Sets default values for this character's properties
	APCharacter();

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Jump();

	
	void MoveRight(float X);
	virtual bool CanJumpInternal_Implementation() const override;

	UFUNCTION()
	void CoyoteTimerElapsed();

	UFUNCTION()
	void JumpBufferTimerElapsed();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	void SetupMovementComponent();
	bool bHasDoubleJumped;
};
