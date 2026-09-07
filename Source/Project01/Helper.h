#pragma once

#include "CoreMinimal.h"

class PROJECT01_API Helper
{
public:
	// 씬 컴포넌트 만들기
	template<typename T>
	static T* CreateSceneComponent(AActor* InOwner, FName InComponentName, USceneComponent* InParent = nullptr, FName InSocketName = NAME_None)
	{
		T* Component = InOwner->CreateDefaultSubobject<T>(InComponentName);

		if (InParent != nullptr)
			Component->SetupAttachment(InParent, InSocketName);
		else
			InOwner->SetRootComponent(Component);

		return Component;
	}

	// 액터 컴포넌트 만들기
	template<typename T>
	static T* CreateActorComponent(AActor* Owner, FName ComponentName)
	{
		return Owner->CreateDefaultSubobject<T>(ComponentName);
	}

	// 에셋 불러오기
	template<typename T>
	static T* GetAsset(FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> Asset(*InPath);

		return Cast<T>(Asset.Object);
	}

	template<typename T>
	static T* GetAssetDynamic(FString InPath)
	{
		UObject* obj = StaticLoadObject(T::StaticClass(), NULL, *InPath);
		return Cast<T>(obj);
	}

	// 클래스 불러오기
	template<typename T>
	static TSubclassOf<T> GetClass(FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> Asset(*InPath);

		return Asset.Class;
	}
};

#define MAPPING_CLICK(EnhancedInputComponent, InputAction, StartedAction, CompletedAction) \
	if (InputAction) \
	{ \
	EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &StartedAction); \
	EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &CompletedAction); \
	}

#define MAPPING_TRIGGERED(EnhancedInputComponent, InputAction, Action) \
	if (InputAction) \
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &Action);

#define MULTICAST_DELEGATE_BROADCAST(Delegate) \
	if (Delegate.IsBound()) Delegate.Broadcast()