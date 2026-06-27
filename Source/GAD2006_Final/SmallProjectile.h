#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmallProjectile.generated.h"

UCLASS()
class GAD2006_FINAL_API ASmallProjectile : public AActor
{
	GENERATED_BODY()
    
public:    
	ASmallProjectile();

protected:
	virtual void BeginPlay() override;

public:    
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class USphereComponent* CollisionComponent;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovement;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visuals")
	class UParticleSystemComponent* ProjectileEffect;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float InitialSpeed = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxSpeed = 2000.0f;



protected:
	
};