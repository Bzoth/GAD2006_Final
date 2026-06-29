#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyBoss.generated.h"


USTRUCT(BlueprintType)
struct FBAttackReturn
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Output")
    AActor* Actor = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Output")
    FVector HitLocation = FVector::ZeroVector;
};

UCLASS()
class GAD2006_FINAL_API AEnemyBoss : public AEnemyBase
{
    GENERATED_BODY()

public:
    AEnemyBoss();

    UPROPERTY(BlueprintReadWrite, Category = "Boss|Combat")
    bool CanHitAnim = false;

    UPROPERTY(BlueprintReadWrite, Category = "Boss|Combat")
    bool ShouldCheckRange = true;

    float TakenDamage = 0.0f;

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

public:
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Setup")
    bool Phase1 = true;
    
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Combat")
    TSubclassOf<class ASmallProjectile> ProjectileClass;
    
    UFUNCTION(BlueprintCallable, Category = "Boss|Combat")
    void FireBossProjectile(FVector FirePoint);
    
    UFUNCTION(BlueprintCallable, Category = "Combat")
    TArray<FBAttackReturn> AttackTrace(FVector Start, FVector End, float Size);
    
    UFUNCTION(BlueprintCallable, Category = "Boss|Animation")
    bool TookHeavyDamage(float Damage);
    
    UFUNCTION(BlueprintCallable, Category = "Boss|Animation")
    void ExplodeTransition();

    UFUNCTION(BlueprintCallable, Category = "Boss|Animation")
    bool CheckInRange(AActor* Player);
    
    UFUNCTION(BlueprintImplementableEvent, Category = "Boss|Animation")
    void PhaseChanged();

protected:
    

private:
    
};