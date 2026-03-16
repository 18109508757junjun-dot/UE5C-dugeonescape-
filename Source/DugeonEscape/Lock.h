// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerComponent.h"
#include "Components/Staticmeshcomponent.h"
#include "Lock.generated.h"

UCLASS()
class DUGEONESCAPE_API ALock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;//根组件

	UPROPERTY(VisibleAnywhere)
	UTriggerComponent* TriggerComp;//触发器组件

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* KeyItemComp;//静态网格组件，用于显示可收集物品的外观

	bool GetIsKeyPlaced();
	void SetIsKeyPlaced(bool NewIsKeyPlaced);
	
	UPROPERTY(EditAnywhere)
	FString KeyItemName;

private:
	UPROPERTY(visibleAnywhere)
	bool IsKeyPlaced = false;
};
