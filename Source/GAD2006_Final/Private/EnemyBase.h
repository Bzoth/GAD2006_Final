#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class GAD2006_FINAL_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyBase();

protected:
	virtual void BeginPlay() override;

public:    


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float BaseDamage = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool CanTakeHit = true;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool bIsDead = false;

	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	float TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Attack();

protected:


	UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
	void OnHitReact();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
	void OnDeath();
	
};