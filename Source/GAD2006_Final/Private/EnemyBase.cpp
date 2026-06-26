#include "EnemyBase.h"
#include "CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
    
	CurrentHealth = MaxHealth;
}

void AEnemyBase::Attack()
{
	return;
}


float AEnemyBase::TakeDamage(float DamageAmount)
{

	if (CanTakeHit) 
	{
		if (bIsDead) return 0.0f;

		CanTakeHit = false;
		CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

		if (CurrentHealth <= 0.0f)
		{
			bIsDead = true;
			GetCharacterMovement()->DisableMovement(); 
			GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore); 
        
			OnDeath(); 
		}
		else
		{
			OnHitReact(); 
		}

		return DamageAmount;
	}
	else
	{
		return 0.0f;
	}
	
}