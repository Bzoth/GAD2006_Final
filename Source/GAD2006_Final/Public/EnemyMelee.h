#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyMelee.generated.h"


USTRUCT(BlueprintType)
struct FMEAttackReturn
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Output")
	AActor* Actor = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Output")
	FVector HitLocation = FVector::ZeroVector;
};

UCLASS()
class GAD2006_FINAL_API AEnemyMelee : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyMelee();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	TArray<FMEAttackReturn> EnemyTrace(FVector Fist);

protected:
	virtual void BeginPlay() override;

public:
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackCooldown = 2.0f;


protected:
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
	void OnMeleeAttackTriggered();

private:
	
};