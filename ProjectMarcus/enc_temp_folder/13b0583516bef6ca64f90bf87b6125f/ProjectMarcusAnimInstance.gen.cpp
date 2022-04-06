// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ProjectMarcus/Character/ProjectMarcusAnimInstance.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeProjectMarcusAnimInstance() {}
// Cross Module References
	PROJECTMARCUS_API UEnum* Z_Construct_UEnum_ProjectMarcus_EAimOffsetState();
	UPackage* Z_Construct_UPackage__Script_ProjectMarcus();
	PROJECTMARCUS_API UClass* Z_Construct_UClass_UProjectMarcusAnimInstance_NoRegister();
	PROJECTMARCUS_API UClass* Z_Construct_UClass_UProjectMarcusAnimInstance();
	ENGINE_API UClass* Z_Construct_UClass_UAnimInstance();
	PROJECTMARCUS_API UClass* Z_Construct_UClass_AProjectMarcusCharacter_NoRegister();
// End Cross Module References
	static UEnum* EAimOffsetState_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_ProjectMarcus_EAimOffsetState, Z_Construct_UPackage__Script_ProjectMarcus(), TEXT("EAimOffsetState"));
		}
		return Singleton;
	}
	template<> PROJECTMARCUS_API UEnum* StaticEnum<EAimOffsetState>()
	{
		return EAimOffsetState_StaticEnum();
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EAimOffsetState(EAimOffsetState_StaticEnum, TEXT("/Script/ProjectMarcus"), TEXT("EAimOffsetState"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_ProjectMarcus_EAimOffsetState_Hash() { return 2581629081U; }
	UEnum* Z_Construct_UEnum_ProjectMarcus_EAimOffsetState()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_ProjectMarcus();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EAimOffsetState"), 0, Get_Z_Construct_UEnum_ProjectMarcus_EAimOffsetState_Hash(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EAimOffsetState::EAOS_ADS", (int64)EAimOffsetState::EAOS_ADS },
				{ "EAimOffsetState::EAOS_Hip", (int64)EAimOffsetState::EAOS_Hip },
				{ "EAimOffsetState::EAOS_Reloading", (int64)EAimOffsetState::EAOS_Reloading },
				{ "EAimOffsetState::EAOS_InAir", (int64)EAimOffsetState::EAOS_InAir },
				{ "EAimOffsetState::EAOS_Max", (int64)EAimOffsetState::EAOS_Max },
			};
#if WITH_METADATA
			const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "EAOS_ADS.DisplayName", "Aim Down Sight" },
				{ "EAOS_ADS.Name", "EAimOffsetState::EAOS_ADS" },
				{ "EAOS_Hip.DisplayName", "Hip Placement" },
				{ "EAOS_Hip.Name", "EAimOffsetState::EAOS_Hip" },
				{ "EAOS_InAir.DisplayName", "InAir" },
				{ "EAOS_InAir.Name", "EAimOffsetState::EAOS_InAir" },
				{ "EAOS_Max.DisplayName", "InvalidMax" },
				{ "EAOS_Max.Name", "EAimOffsetState::EAOS_Max" },
				{ "EAOS_Reloading.DisplayName", "Reload" },
				{ "EAOS_Reloading.Name", "EAimOffsetState::EAOS_Reloading" },
				{ "ModuleRelativePath", "Character/ProjectMarcusAnimInstance.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_ProjectMarcus,
				nullptr,
				"EAimOffsetState",
				"EAimOffsetState",
				Enumerators,
				UE_ARRAY_COUNT(Enumerators),
				RF_Public|RF_Transient|RF_MarkAsNative,
				EEnumFlags::None,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				(uint8)UEnum::ECppForm::EnumClass,
				METADATA_PARAMS(Enum_MetaDataParams, UE_ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	DEFINE_FUNCTION(UProjectMarcusAnimInstance::execUpdateAnimationProperties)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_DeltaTime);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->UpdateAnimationProperties(Z_Param_DeltaTime);
		P_NATIVE_END;
	}
	void UProjectMarcusAnimInstance::StaticRegisterNativesUProjectMarcusAnimInstance()
	{
		UClass* Class = UProjectMarcusAnimInstance::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "UpdateAnimationProperties", &UProjectMarcusAnimInstance::execUpdateAnimationProperties },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UProjectMarcusAnimInstance_UpdateAnimationProperties_Statics
	{
		struct ProjectMarcusAnimInstance_eventUpdateAnimationProperties_Parms
		{
			float DeltaTime;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_DeltaTime;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UProjectMarcusAnimInstance_UpdateAnimationProperties_Statics::NewProp_DeltaTime = { "DeltaTime", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ProjectMarcusAnimInstance_eventUpdateAnimationProperties_Parms, DeltaTime), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UProjectMarcusAnimInstance_UpdateAnimationProperties_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UProjectMarcusAnimInstance_UpdateAnimationProperties_Statics::NewProp_DeltaTime,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UProjectMarcusAnimInstance_UpdateAnimationProperties_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Character/ProjectMarcusAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UProjectMarcusAnimInstance_UpdateAnimationProperties_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UProjectMarcusAnimInstance, nullptr, "UpdateAnimationProperties", nullptr, nullptr, sizeof(ProjectMarcusAnimInstance_eventUpdateAnimationProperties_Parms), Z_Construct_UFunction_UProjectMarcusAnimInstance_UpdateAnimationProperties_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UProjectMarcusAnimInstance_UpdateAnimationProperties_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UProjectMarcusAnimInstance_UpdateAnimationProperties_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UProjectMarcusAnimInstance_UpdateAnimationProperties_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UProjectMarcusAnimInstance_UpdateAnimationProperties()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UProjectMarcusAnimInstance_UpdateAnimationProperties_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UProjectMarcusAnimInstance_NoRegister()
	{
		return UProjectMarcusAnimInstance::StaticClass();
	}
	struct Z_Construct_UClass_UProjectMarcusAnimInstance_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PMCharacter_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_PMCharacter;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FootSpeed_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_FootSpeed;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bIsInAir_MetaData[];
#endif
		static void NewProp_bIsInAir_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bIsInAir;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bIsAccelerating_MetaData[];
#endif
		static void NewProp_bIsAccelerating_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bIsAccelerating;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AimMovementDiff_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_AimMovementDiff;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LastAimMovementDiff_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LastAimMovementDiff;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bIsAiming_MetaData[];
#endif
		static void NewProp_bIsAiming_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bIsAiming;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_YawDiffFromRootToCharacter_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_YawDiffFromRootToCharacter;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TurnInPlaceYawThreshold_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_TurnInPlaceYawThreshold;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_CurrentPitch_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_CurrentPitch;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bReloadingInProgress_MetaData[];
#endif
		static void NewProp_bReloadingInProgress_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bReloadingInProgress;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_AimOffsetState_Underlying;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AimOffsetState_MetaData[];
#endif
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_AimOffsetState;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UAnimInstance,
		(UObject* (*)())Z_Construct_UPackage__Script_ProjectMarcus,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UProjectMarcusAnimInstance_UpdateAnimationProperties, "UpdateAnimationProperties" }, // 824257756
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "AnimInstance" },
		{ "IncludePath", "Character/ProjectMarcusAnimInstance.h" },
		{ "ModuleRelativePath", "Character/ProjectMarcusAnimInstance.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_PMCharacter_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Character/ProjectMarcusAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_PMCharacter = { "PMCharacter", nullptr, (EPropertyFlags)0x0040000000020015, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UProjectMarcusAnimInstance, PMCharacter), Z_Construct_UClass_AProjectMarcusCharacter_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_PMCharacter_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_PMCharacter_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_FootSpeed_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Movement" },
		{ "Comment", "// Speed of the character\n" },
		{ "ModuleRelativePath", "Character/ProjectMarcusAnimInstance.h" },
		{ "ToolTip", "Speed of the character" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_FootSpeed = { "FootSpeed", nullptr, (EPropertyFlags)0x0040000000020015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UProjectMarcusAnimInstance, FootSpeed), METADATA_PARAMS(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_FootSpeed_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_FootSpeed_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsInAir_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Movement" },
		{ "Comment", "// Indicates if the character is in the air\n" },
		{ "ModuleRelativePath", "Character/ProjectMarcusAnimInstance.h" },
		{ "ToolTip", "Indicates if the character is in the air" },
	};
#endif
	void Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsInAir_SetBit(void* Obj)
	{
		((UProjectMarcusAnimInstance*)Obj)->bIsInAir = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsInAir = { "bIsInAir", nullptr, (EPropertyFlags)0x0040000000020015, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UProjectMarcusAnimInstance), &Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsInAir_SetBit, METADATA_PARAMS(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsInAir_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsInAir_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsAccelerating_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Movement" },
		{ "Comment", "// Indicates if the character is accelerating (acceleration > 0)\n" },
		{ "ModuleRelativePath", "Character/ProjectMarcusAnimInstance.h" },
		{ "ToolTip", "Indicates if the character is accelerating (acceleration > 0)" },
	};
#endif
	void Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsAccelerating_SetBit(void* Obj)
	{
		((UProjectMarcusAnimInstance*)Obj)->bIsAccelerating = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsAccelerating = { "bIsAccelerating", nullptr, (EPropertyFlags)0x0040000000020015, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UProjectMarcusAnimInstance), &Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsAccelerating_SetBit, METADATA_PARAMS(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsAccelerating_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsAccelerating_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_AimMovementDiff_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Movement" },
		{ "Comment", "// Difference between [-180,180] the aim direction and movement direction. Used for strafing animations\n" },
		{ "ModuleRelativePath", "Character/ProjectMarcusAnimInstance.h" },
		{ "ToolTip", "Difference between [-180,180] the aim direction and movement direction. Used for strafing animations" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_AimMovementDiff = { "AimMovementDiff", nullptr, (EPropertyFlags)0x0040000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UProjectMarcusAnimInstance, AimMovementDiff), METADATA_PARAMS(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_AimMovementDiff_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_AimMovementDiff_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_LastAimMovementDiff_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Movement" },
		{ "Comment", "// strafing data in the last frame used for strafe ending animations\n" },
		{ "ModuleRelativePath", "Character/ProjectMarcusAnimInstance.h" },
		{ "ToolTip", "strafing data in the last frame used for strafe ending animations" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_LastAimMovementDiff = { "LastAimMovementDiff", nullptr, (EPropertyFlags)0x0040000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UProjectMarcusAnimInstance, LastAimMovementDiff), METADATA_PARAMS(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_LastAimMovementDiff_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_LastAimMovementDiff_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsAiming_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Character/ProjectMarcusAnimInstance.h" },
	};
#endif
	void Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsAiming_SetBit(void* Obj)
	{
		((UProjectMarcusAnimInstance*)Obj)->bIsAiming = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsAiming = { "bIsAiming", nullptr, (EPropertyFlags)0x0040000000020015, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UProjectMarcusAnimInstance), &Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsAiming_SetBit, METADATA_PARAMS(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsAiming_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsAiming_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_YawDiffFromRootToCharacter_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Turn in Place" },
		{ "Comment", "// Horizontal rotation difference between where the character is facing and the root is facing.\n" },
		{ "ModuleRelativePath", "Character/ProjectMarcusAnimInstance.h" },
		{ "ToolTip", "Horizontal rotation difference between where the character is facing and the root is facing." },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_YawDiffFromRootToCharacter = { "YawDiffFromRootToCharacter", nullptr, (EPropertyFlags)0x0040000000020015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UProjectMarcusAnimInstance, YawDiffFromRootToCharacter), METADATA_PARAMS(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_YawDiffFromRootToCharacter_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_YawDiffFromRootToCharacter_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_TurnInPlaceYawThreshold_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Turn in Place" },
		{ "Comment", "// Threshold which needs to be passed before we actually turn in place\n" },
		{ "ModuleRelativePath", "Character/ProjectMarcusAnimInstance.h" },
		{ "ToolTip", "Threshold which needs to be passed before we actually turn in place" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_TurnInPlaceYawThreshold = { "TurnInPlaceYawThreshold", nullptr, (EPropertyFlags)0x0040000000020015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UProjectMarcusAnimInstance, TurnInPlaceYawThreshold), METADATA_PARAMS(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_TurnInPlaceYawThreshold_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_TurnInPlaceYawThreshold_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_CurrentPitch_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Turn in Place" },
		{ "ModuleRelativePath", "Character/ProjectMarcusAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_CurrentPitch = { "CurrentPitch", nullptr, (EPropertyFlags)0x0040000000020015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UProjectMarcusAnimInstance, CurrentPitch), METADATA_PARAMS(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_CurrentPitch_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_CurrentPitch_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bReloadingInProgress_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Turn in Place" },
		{ "Comment", "// True when reloading, used to prevent hip aim offset while reloading\n" },
		{ "ModuleRelativePath", "Character/ProjectMarcusAnimInstance.h" },
		{ "ToolTip", "True when reloading, used to prevent hip aim offset while reloading" },
	};
#endif
	void Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bReloadingInProgress_SetBit(void* Obj)
	{
		((UProjectMarcusAnimInstance*)Obj)->bReloadingInProgress = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bReloadingInProgress = { "bReloadingInProgress", nullptr, (EPropertyFlags)0x0040000000020015, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UProjectMarcusAnimInstance), &Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bReloadingInProgress_SetBit, METADATA_PARAMS(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bReloadingInProgress_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bReloadingInProgress_MetaData)) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_AimOffsetState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_AimOffsetState_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Turn in Place" },
		{ "Comment", "// Used to determine which animation AimOffset to use (ADS or Hip or Reloading)\n" },
		{ "ModuleRelativePath", "Character/ProjectMarcusAnimInstance.h" },
		{ "ToolTip", "Used to determine which animation AimOffset to use (ADS or Hip or Reloading)" },
	};
#endif
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_AimOffsetState = { "AimOffsetState", nullptr, (EPropertyFlags)0x0040000000020015, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UProjectMarcusAnimInstance, AimOffsetState), Z_Construct_UEnum_ProjectMarcus_EAimOffsetState, METADATA_PARAMS(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_AimOffsetState_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_AimOffsetState_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_PMCharacter,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_FootSpeed,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsInAir,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsAccelerating,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_AimMovementDiff,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_LastAimMovementDiff,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bIsAiming,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_YawDiffFromRootToCharacter,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_TurnInPlaceYawThreshold,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_CurrentPitch,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_bReloadingInProgress,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_AimOffsetState_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::NewProp_AimOffsetState,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UProjectMarcusAnimInstance>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::ClassParams = {
		&UProjectMarcusAnimInstance::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::PropPointers),
		0,
		0x009000A8u,
		METADATA_PARAMS(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UProjectMarcusAnimInstance()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UProjectMarcusAnimInstance_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UProjectMarcusAnimInstance, 2149387035);
	template<> PROJECTMARCUS_API UClass* StaticClass<UProjectMarcusAnimInstance>()
	{
		return UProjectMarcusAnimInstance::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UProjectMarcusAnimInstance(Z_Construct_UClass_UProjectMarcusAnimInstance, &UProjectMarcusAnimInstance::StaticClass, TEXT("/Script/ProjectMarcus"), TEXT("UProjectMarcusAnimInstance"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UProjectMarcusAnimInstance);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
