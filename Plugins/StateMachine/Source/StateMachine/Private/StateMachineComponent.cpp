// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachineComponent.h"

// Sets default values for this component's properties
UStateMachineComponent::UStateMachineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


bool UStateMachineComponent::SwitchState(FGameplayTag Tag)
{
	
	if(Tag.MatchesTagExact(StateTag)) 
	{ 
		if(bDebug)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not switch state for %s because it is already in %s"), *GetOwner()->GetName(), *Tag.ToString());
		}
		return false;
	}
	// To prevent the old state ticking after switching to a new one
	bCanTickState = false;
	EndState();
	StateTag = Tag;
	InitState();
	bCanTickState = true;

	if(StateChangedDelegate.IsBound())
	{
		StateChangedDelegate.Broadcast(StateTag);
	}
	return true;
}

// Called when the game starts
void UStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SwitchState(InitialStateTag);
}


// Called every frame
void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if(bCanTickState)
	{
		TickState(DeltaTime);
	}

	if(bDebug)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("Current State for %s: %s"), *GetOwner()->GetName(), *StateTag.ToString()));
		if(StateHistory.Num() > 0)
		{
			for(int32 i = 0; i < StateHistory.Num(); ++i)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, FString::Printf(TEXT("%s"), *StateHistory[i].ToString()));
			}
		}
	}
}

void UStateMachineComponent::InitState()
{
	if(InitStateDelegate.IsBound())
	{
		InitStateDelegate.Broadcast(StateTag);
	}
}

void UStateMachineComponent::TickState(float deltaTime)
{
	if(TickStateDelegate.IsBound())
	{
		TickStateDelegate.Broadcast(deltaTime, StateTag);
	}
}

void UStateMachineComponent::EndState()
{
	if(StateHistory.Num() >= StateHistoryLength)
	{
		StateHistory.RemoveAt(0);
		
	}
	StateHistory.Push(StateTag);
	if(EndStateDelegate.IsBound())
	{
		EndStateDelegate.Broadcast(StateTag);
	}
}

