// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FGameplayTag;
#ifdef STATEMACHINE_StateMachineComponent_generated_h
#error "StateMachineComponent.generated.h already included, missing '#pragma once' in StateMachineComponent.h"
#endif
#define STATEMACHINE_StateMachineComponent_generated_h

#define FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_11_DELEGATE \
struct _Script_StateMachine_eventStateChangedSignature_Parms \
{ \
	FGameplayTag NewStateTag; \
}; \
static inline void FStateChangedSignature_DelegateWrapper(const FMulticastScriptDelegate& StateChangedSignature, FGameplayTag const& NewStateTag) \
{ \
	_Script_StateMachine_eventStateChangedSignature_Parms Parms; \
	Parms.NewStateTag=NewStateTag; \
	StateChangedSignature.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_12_DELEGATE \
struct _Script_StateMachine_eventInitStateSignature_Parms \
{ \
	FGameplayTag StateTag; \
}; \
static inline void FInitStateSignature_DelegateWrapper(const FMulticastScriptDelegate& InitStateSignature, FGameplayTag const& StateTag) \
{ \
	_Script_StateMachine_eventInitStateSignature_Parms Parms; \
	Parms.StateTag=StateTag; \
	InitStateSignature.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_13_DELEGATE \
struct _Script_StateMachine_eventEndStateSignature_Parms \
{ \
	FGameplayTag StateTag; \
}; \
static inline void FEndStateSignature_DelegateWrapper(const FMulticastScriptDelegate& EndStateSignature, FGameplayTag const& StateTag) \
{ \
	_Script_StateMachine_eventEndStateSignature_Parms Parms; \
	Parms.StateTag=StateTag; \
	EndStateSignature.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_14_DELEGATE \
struct _Script_StateMachine_eventTickStateSignature_Parms \
{ \
	float DeltaTime; \
	FGameplayTag StateTag; \
}; \
static inline void FTickStateSignature_DelegateWrapper(const FMulticastScriptDelegate& TickStateSignature, float DeltaTime, FGameplayTag const& StateTag) \
{ \
	_Script_StateMachine_eventTickStateSignature_Parms Parms; \
	Parms.DeltaTime=DeltaTime; \
	Parms.StateTag=StateTag; \
	TickStateSignature.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_19_SPARSE_DATA
#define FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_19_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execSwitchState);


#define FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_19_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execSwitchState);


#define FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_19_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUStateMachineComponent(); \
	friend struct Z_Construct_UClass_UStateMachineComponent_Statics; \
public: \
	DECLARE_CLASS(UStateMachineComponent, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/StateMachine"), NO_API) \
	DECLARE_SERIALIZER(UStateMachineComponent)


#define FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_19_INCLASS \
private: \
	static void StaticRegisterNativesUStateMachineComponent(); \
	friend struct Z_Construct_UClass_UStateMachineComponent_Statics; \
public: \
	DECLARE_CLASS(UStateMachineComponent, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/StateMachine"), NO_API) \
	DECLARE_SERIALIZER(UStateMachineComponent)


#define FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_19_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UStateMachineComponent(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UStateMachineComponent) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UStateMachineComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UStateMachineComponent); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UStateMachineComponent(UStateMachineComponent&&); \
	NO_API UStateMachineComponent(const UStateMachineComponent&); \
public:


#define FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_19_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UStateMachineComponent(UStateMachineComponent&&); \
	NO_API UStateMachineComponent(const UStateMachineComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UStateMachineComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UStateMachineComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UStateMachineComponent)


#define FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_16_PROLOG
#define FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_19_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_19_SPARSE_DATA \
	FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_19_RPC_WRAPPERS \
	FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_19_INCLASS \
	FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_19_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_19_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_19_SPARSE_DATA \
	FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_19_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_19_INCLASS_NO_PURE_DECLS \
	FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h_19_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> STATEMACHINE_API UClass* StaticClass<class UStateMachineComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Platformer2D_Plugins_StateMachine_Source_StateMachine_Public_StateMachineComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
