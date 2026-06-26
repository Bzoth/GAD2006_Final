#include "CharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "SmallProjectile.h"
#include "Algo/ForEach.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "LevelInstance/LevelInstanceTypes.h"
#include "ProfilingDebugging/CookStats.h"
#include "Runtime/Engine/Internal/Kismet/BlueprintTypeConversions.h"

ACharacterBase::ACharacterBase()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

    HatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HatMesh"));
    HatMesh->SetupAttachment(GetMesh(), TEXT("head"));

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true; 
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void ACharacterBase::BeginPlay()
{
    Super::BeginPlay();
    
  
    MaxHealth = 100.0f + (Vigor * 10.0f);
    MaxStamina = 80.0f + (Endurance * 5.0f);
    MaxMana = 50.0f + (Intelligence * 8.0f);

    CurrentHealth = MaxHealth;
    CurrentStamina = MaxStamina;
    CurrentMana = MaxMana;
    CurrentPotions = MaxPotions;

    if (GetMesh()->GetMaterial(0))
    {
        DynamicMaterial = GetMesh()->CreateDynamicMaterialInstance(0);
    }
}

void ACharacterBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsSprinting)
    {
        RegenStamina(DeltaTime);
    }
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &ACharacterBase::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &ACharacterBase::LookUp);

    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACharacterBase::SprintStart);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACharacterBase::SprintEnd);
    PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ACharacterBase::Dodge);
    PlayerInputComponent->BindAction("UsePotion", IE_Pressed, this, &ACharacterBase::UsePotion);

    PlayerInputComponent->BindAction("LightAttack", IE_Pressed, this, &ACharacterBase::LightAttack);
    PlayerInputComponent->BindAction("HeavyAttack", IE_Pressed, this, &ACharacterBase::HeavyAttack);
    PlayerInputComponent->BindAction("SpecialAttack", IE_Pressed, this, &ACharacterBase::SpecialAttack);

    PlayerInputComponent->BindAction("LightMagic", IE_Pressed, this, &ACharacterBase::LightMagicAttack);
    PlayerInputComponent->BindAction("SpecialMagic", IE_Pressed, this, &ACharacterBase::SpecialMagicAttack);
}

void ACharacterBase::MoveForward(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ACharacterBase::MoveRight(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void ACharacterBase::SprintStart()
{
    if (CurrentStamina > 10.0f)
    {
        bIsSprinting = true;
        GetCharacterMovement()->MaxWalkSpeed = 650.0f;
    }
}

void ACharacterBase::SprintEnd()
{
    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void ACharacterBase::Dodge()
{
    if (CurrentStamina >= DodgeStaminaCost && CanDodge && CanAttack)
    {
        CurrentStamina -= DodgeStaminaCost;
        OnDodgeTriggered(); 
    }
}

void ACharacterBase::LightAttack()
{
    if (CurrentStamina >= LightAttackStaminaCost && CanAttack)
    {
        CanAttack = false;
        CurrentStamina -= LightAttackStaminaCost;
        Damage = LightAttackDamage*Strength;
        OnLightAttackTriggered(); 
    }
}

void ACharacterBase::HeavyAttack()
{
    if (CurrentStamina >= HeavyAttackStaminaCost && CanAttack)
    {
        CanAttack = false;
        CurrentStamina -= HeavyAttackStaminaCost;
        Damage = HeavyAttackDamage*Strength;
        OnHeavyAttackTriggered(); 
    }
}

void ACharacterBase::SpecialAttack()
{
    if (CurrentStamina >= SpecialAttackStaminaCost && CanAttack)
    {
        CanAttack = false;
        CurrentStamina -= SpecialAttackStaminaCost;
        Damage = SpecialAttackDamage*Strength;
        OnSpecialAttackTriggered();
    }
}

TArray<FAttackReturn> ACharacterBase::AttackTrace(UStaticMeshComponent* Sword)
{

    TArray<FHitResult> HitResults;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    
    bool bHit = UKismetSystemLibrary::SphereTraceMulti(
        GetWorld(),
        Sword->GetSocketLocation("Start"),
        Sword->GetSocketLocation("End"),
        15.0f,
        UEngineTypes::ConvertToTraceType(ECC_Pawn),
        true,
        ActorsToIgnore,
        EDrawDebugTrace::ForOneFrame, 
        HitResults,
        true
        );


    TArray<FAttackReturn> Outputs;

    for (const FHitResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();
        FVector HitLocation = Hit.ImpactPoint;
        FAttackReturn Output;
        
        if (HitActor)
        {
            Output.Actor = HitActor;
            Output.HitLocation = HitLocation;
            Outputs.Add(Output);
        }
        else
        {
            Output.Actor = nullptr;
            Output.HitLocation = FVector::ZeroVector;
            Outputs.Add(Output);
        }
    }
    
    return Outputs;
}



void ACharacterBase::LightMagicAttack()
{
    if (CurrentMana >= LightMagicManaCost && CanAttack)
    {
        CurrentMana -= LightMagicManaCost;
        Damage = LightMagicDamage*Intelligence;
        
        if (SmallProjectileClass)
        {
            FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT("Head")) + GetActorForwardVector() * 45; // Sağ el soket ismi
            FRotator SpawnRotation = GetActorRotation();

            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();
            
            GetWorld()->SpawnActor<ASmallProjectile>(SmallProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
        }
    }
}

void ACharacterBase::SpecialMagicAttack()
{
    if (CurrentMana >= SpecialMagicManaCost && CanAttack)
    {
        CurrentMana -= SpecialMagicManaCost;
        Damage = SpecialMagicDamage*Intelligence;
        OnSpecialMagicTriggered();
    }
}

TArray<FAttackReturn> ACharacterBase::SpecialMagicAttackTrace()
{
    TArray<FHitResult> HitResults;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    
    bool bHit = UKismetSystemLibrary::SphereTraceMulti(
        GetWorld(),
        GetActorLocation(),
        GetActorLocation(),
        350.0f,
        UEngineTypes::ConvertToTraceType(ECC_Pawn),
        true,
        ActorsToIgnore,
        EDrawDebugTrace::ForOneFrame, 
        HitResults,
        true
        );

    TArray<FAttackReturn> Outputs;

    for (const FHitResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();
        FVector HitLocation = Hit.ImpactPoint;
        FAttackReturn Output;
        
        if (HitActor)
        {
            Output.Actor = HitActor;
            Output.HitLocation = HitLocation;
            Outputs.Add(Output);
        }
        else
        {
            Output.Actor = nullptr;
            Output.HitLocation = FVector::ZeroVector;
            Outputs.Add(Output);
        }
    }
    
    return Outputs;
}

void ACharacterBase::UsePotion()
{
    if (CurrentPotions > 0 && CurrentHealth < MaxHealth && CanAttack)
    {
        CanAttack = false;
        CurrentPotions--;
        CurrentHealth = FMath::Clamp(CurrentHealth + PotionHealAmount, 0.0f, MaxHealth);
        OnPotionUsed(); 
    }
}

void ACharacterBase::TakeDamage(float DamageAmount)
{
    if (!IsDead)
    {
        if (CanTakeDamage)
        {
            CanTakeDamage = false;
            CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

            if (CurrentHealth <= 0.0f)
            {
                IsDead = true;
                OnDeath();
            }
            else
            {
                OnTakeHit();
            }
            
        }
    }
}

void ACharacterBase::HealPotionPickup()
{
    if (CurrentPotions < MaxPotions) CurrentPotions++;
}

void ACharacterBase::ChangeModelColor(FLinearColor NewColor)
{
    if (DynamicMaterial)
    {
        DynamicMaterial->SetVectorParameterValue(TEXT("BodyColor"), NewColor);
    }
}

void ACharacterBase::EquipHat(UStaticMesh* NewHatMesh)
{
    if (HatMesh && NewHatMesh)
    {
        HatMesh->SetStaticMesh(NewHatMesh);
    }
}

void ACharacterBase::Turn(float Value) { AddControllerYawInput(Value); }
void ACharacterBase::LookUp(float Value) { AddControllerPitchInput(Value); }
void ACharacterBase::RegenStamina(float DeltaTime)
{
    if (CurrentStamina < MaxStamina)
    {
        CurrentStamina = FMath::Clamp(CurrentStamina + (StaminaRegenRate * DeltaTime), 0.0f, MaxStamina);
    }
}