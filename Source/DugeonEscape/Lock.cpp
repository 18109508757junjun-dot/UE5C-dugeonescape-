// Fill out your copyright notice in the Description page of Project Settings.


#include "Lock.h"

// Sets default values
ALock::ALock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("Lock");//该类的实例将被标记为CollectableItem，可以通过标签系统进行识别和操作
	//以下均是通过函数createdefaultsubobject创建组件，并设置组件之间的父子关系
	//这样可以直接在c++中访问这三个组件，如果在蓝图中创建组件，将无法访问
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);//将RootComp设置为根组件

	TriggerComp = CreateDefaultSubobject<UTriggerComponent>(TEXT("TriggerComp"));
	TriggerComp->SetupAttachment(RootComp);//将TriggerComp附加到RootComp上，使其成为RootComp的子组件

	KeyItemComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyItemComp"));
	KeyItemComp->SetupAttachment(RootComp);//将KeyItemComp附加到RootComp上，使其成为RootComp的子组件
}

// Called when the game starts or when spawned
void ALock::BeginPlay()
{
	Super::BeginPlay();
	SetIsKeyPlaced(false);
}

// Called every frame
void ALock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*double time = GetWorld()->TimeSeconds;
	UE_LOG(LogTemp, Display, TEXT("time: %f"), time);*/

}

//该函数用于获取IsKeyPlaced的值，表示钥匙是否被放置在锁上
bool ALock::GetIsKeyPlaced()
{
	return IsKeyPlaced;
}

//该函数用于设置IsKeyPlaced的值，表示钥匙是否被放置在锁上
void ALock::SetIsKeyPlaced(bool NewIsKeyPlaced)
{
	IsKeyPlaced = NewIsKeyPlaced;

	TriggerComp->Trigger(NewIsKeyPlaced);
	KeyItemComp->SetVisibility(NewIsKeyPlaced);
}

