// Copyright Epic Games, Inc. All Rights Reserved.

#include "DugeonEscapeCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DugeonEscape.h"
#include "CollectableItem.h"
#include "Lock.h"

ADugeonEscapeCharacter::ADugeonEscapeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void ADugeonEscapeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADugeonEscapeCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADugeonEscapeCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADugeonEscapeCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADugeonEscapeCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ADugeonEscapeCharacter::LookInput);
		
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ADugeonEscapeCharacter::Interact);
	}
	else
	{
		UE_LOG(LogDugeonEscape, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADugeonEscapeCharacter::Interact()
{
	UE_LOG(LogTemp,Display,TEXT("Interact"));
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();//获取摄像机位置作为起始点
	FVector End = Start + (FirstPersonCameraComponent->GetForwardVector() * MaxInteractDistance);//终点位置=起始点+（摄像机前向量*最大交互距离）
	
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false,5.0f);//该函数参数从左到右依次是获取世界，开始位置，结束位置，线的颜色，是否一直存在,存在多久

	FCollisionShape InteractSphere = FCollisionShape::MakeSphere(InteractSphereRadius);//创建一个球形碰撞体，参数是半径
	//DrawDebugSphere(GetWorld(), End, InteractSphereRadius, 12, FColor::Blue, false, 5.0f);//该函数参数从左到右依次是获取世界，中心位置，半径，线段数量，颜色，是否一直存在,存在多久

	FHitResult HitResult;//创建一个碰撞结果结构体，用于存储碰撞检测的结果
	//这个函数是进行碰撞检测的函数，使用球形碰撞体从起始点到终点进行检测，检测碰撞通道为ECC_GameTraceChannel2（自定义通道），将结果存储在HitResult中
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult, 
		Start, End, 
		FQuat::Identity, 
		ECC_GameTraceChannel2,
		InteractSphere);//该函数参数从左到右依次是碰撞结果，开始位置，结束位置，旋转（这里使用默认值），碰撞通道(自定义通道)，碰撞形状
	
	if (HasHit)
	{
		AActor* HitActor = HitResult.GetActor();//获取被碰撞到的Actor
		//UE_LOG(LogTemp, Display, TEXT("Hit Actor: %s"), *HitActor->GetActorNameOrLabel());//输出被碰撞到的Actor的名字

		if (HitActor->ActorHasTag("CollectableItem"))
		{
			//碰撞的actor是CollectableItem标签
			
			ACollectableItem* CollectableItem = Cast<ACollectableItem>(HitActor);//将被碰撞到的Actor转换为ACollectableItem类型,这样就可以访问ACollectableItem类中的属性和方法了
			if (CollectableItem)
			{
				//UE_LOG(LogTemp, Display, TEXT("Hit a Collectable Item with name %s!"),*CollectableItem->ItemName);
				ItemList.Add(CollectableItem->ItemName);//将被碰撞到的物品的名字添加到角色的物品列表中
				CollectableItem->Destroy();//销毁被碰撞到的物品
			}
		}

		else if (HitActor->ActorHasTag("Lock"))
		{
			//碰撞的actor是Lock标签
			
			ALock* LockActor = Cast<ALock>(HitActor);
			if (LockActor)
			{
				//UE_LOG(LogTemp, Display, TEXT("Hit a Lock with name %s!"),*LockActor->KeyItemName);
                if (!LockActor->GetIsKeyPlaced()) //检查机关上是否放置钥匙，改为使用LockActor的GetIsKeyPlaced()
				{
					int32 ItemsRemoved = ItemList.RemoveSingle(LockActor->KeyItemName);//从物品列表中移除一个与锁的钥匙名字相同的物品
					if (ItemsRemoved) 
					{
						LockActor->SetIsKeyPlaced(true);
					}
					else
					{
						UE_LOG(LogTemp, Display, TEXT("The lock is not in inventory!"));
					}
				}
				else//机关上有钥匙
				{
					ItemList.Add(LockActor->KeyItemName);
					LockActor->SetIsKeyPlaced(false);
				}

			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No Actor Hit"));//如果没有碰撞到任何Actor，输出没有碰撞到Actor
	}
}


void ADugeonEscapeCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void ADugeonEscapeCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void ADugeonEscapeCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ADugeonEscapeCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void ADugeonEscapeCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void ADugeonEscapeCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}
