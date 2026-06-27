#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

USTRUCT(BlueprintType)
struct FAttackReturn
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Output")
    AActor* Actor = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Output")
    FVector HitLocation = FVector::ZeroVector;
};

UCLASS()
class GAD2006_FINAL_API ACharacterBase : public ACharacter
{
    GENERATED_BODY()

public:
    ACharacterBase();

protected:
    virtual void BeginPlay() override;

public:    
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Customization")
    class UStaticMeshComponent* HatMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TSubclassOf<class ASmallProjectile> SmallProjectileClass;

    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Vigor = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Endurance = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Intelligence = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Strength = 10;

    
    UPROPERTY(BlueprintReadOnly, Category = "Resources")
    float MaxHealth;
    UPROPERTY(BlueprintReadOnly, Category = "Resources")
    float CurrentHealth;
    UPROPERTY(BlueprintReadOnly, Category = "Resources")
    float MaxStamina;
    UPROPERTY(BlueprintReadOnly, Category = "Resources")
    float CurrentStamina;
    UPROPERTY(BlueprintReadOnly, Category = "Resources")
    float MaxMana;
    UPROPERTY(BlueprintReadOnly, Category = "Resources")
    float CurrentMana;
    UPROPERTY(BlueprintReadWrite, Category = "Resources")
    bool CanTakeDamage = true;
    UPROPERTY(BlueprintReadOnly, Category = "Resources")
    bool IsDead = false;

    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float StaminaRegenRate = 15.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float DodgeStaminaCost = 25.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float LightAttackStaminaCost = 15.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float LightAttackDamage = 10.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float HeavyAttackStaminaCost = 30.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float HeavyAttackDamage = 15.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float SpecialAttackStaminaCost = 45.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float SpecialAttackDamage = 12.5f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float LightMagicManaCost = 10.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float LightMagicDamage = 7.5f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float SpecialMagicManaCost = 30.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float SpecialMagicDamage = 15.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float Damage = 10.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    bool CanAttack = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    bool CanDodge = true;

    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
    int32 MaxPotions = 5;
    UPROPERTY(BlueprintReadOnly, Category = "Items")
    int32 CurrentPotions;   
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
    float PotionHealAmount = 40.0f;

    UFUNCTION(BlueprintCallable, Category = "Items")
    void HealPotionPickup();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    TArray<FAttackReturn> AttackTrace (UStaticMeshComponent* Sword);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    TArray<FAttackReturn> SpecialMagicAttackTrace();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void TakeDamage(float DamageAmount);

   
    UFUNCTION(BlueprintCallable, Category = "Customization")
    void ChangeModelColor(FLinearColor NewColor);

    UFUNCTION(BlueprintCallable, Category = "Customization")
    void EquipHat(UStaticMesh* NewHatMesh);

protected:
   
    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);
    void SprintStart();
    void SprintEnd();
    
   
    void Dodge();
    void UsePotion();
    void RegenStamina(float DeltaTime);


    void LightAttack();
    void HeavyAttack();
    void SpecialAttack();
    void LightMagicAttack();
    void SpecialMagicAttack();
    

   
    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnLightAttackTriggered();

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnHeavyAttackTriggered();

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnSpecialAttackTriggered();

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnDodgeTriggered();

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnSpecialMagicTriggered();

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnPotionUsed();

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnTakeHit();

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnDeath();

  
    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    bool bIsInvulnerable = false;

    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    bool bIsSprinting = false;

private:
    class UMaterialInstanceDynamic* DynamicMaterial;
};