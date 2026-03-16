// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Math/UnrealMathUtility.h"
// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	Starlocation = GetOwner()->GetActorLocation();
	SetShouldMove(false);
	//在游戏一开始将开始位置设置为起始位置;
	
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*if (ShouldMove)
	{
		TargetLocation = Starlocation + MoveOffset;
	}
	else
	{
		TargetLocation = Starlocation;
	}*/
	//代码优化tips 上述代码放在tick当中意味着每一帧都在计算targetLocation，但其实TL是设置好的不变的，只有当shoulmove变化才应该重新计算，所以将上述代码放到SetShouldMove中可以优化。
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	if(!CurrentLocation.Equals(TargetLocation))
	{
		float Speed = MoveOffset.Length() / Movetime;
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
		GetOwner()->SetActorLocation(NewLocation);
		//UE_LOG(LogTemp, Display, TEXT("is moving"));
	}

	
}



bool UMover::GetShouldMove()
{
	return ShouldMove;
}

void UMover::SetShouldMove(bool NewShouldMove)
{
	ShouldMove = NewShouldMove;
	if (ShouldMove)
	{
		TargetLocation = Starlocation + MoveOffset;
	}
	else
	{
		TargetLocation = Starlocation;
	}
}



