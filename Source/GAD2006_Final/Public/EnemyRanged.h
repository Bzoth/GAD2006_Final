#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyRanged.generated.h"

UCLASS()
class GAD2006_FINAL_API AEnemyRanged : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyRanged();

protected:

public:

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool CanSeePlayer(AActor* Player);
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void FireProjectile(FVector Head);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<class ASmallProjectile> ProjectileClass;


protected:
	

private:
	
	bool bCanAttack = true;
	
};