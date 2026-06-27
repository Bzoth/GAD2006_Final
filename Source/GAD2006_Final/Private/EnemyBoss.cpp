#include "EnemyBoss.h"
#include "SmallProjectile.h"
#include "CharacterBase.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

AEnemyBoss::AEnemyBoss()
{
    MaxHealth = 500.0f; 
    
}

void AEnemyBoss::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyBoss::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
}

bool AEnemyBoss::CheckInRange(AActor* Player)
{
    if (ShouldCheckRange)
    {
        if (FVector::Dist(Player->GetActorLocation(), this->GetActorLocation()) > 1000)
        {
            ShouldCheckRange = false;
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return true;
    }
}


TArray<FBAttackReturn> AEnemyBoss::AttackTrace(FVector Fist, float Size)
{

    TArray<FHitResult> HitResults;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    
    bool bHit = UKismetSystemLibrary::SphereTraceMulti(
        GetWorld(),
        Fist,
        Fist,
        Size,
        UEngineTypes::ConvertToTraceType(ECC_Pawn),
        true,
        ActorsToIgnore,
        EDrawDebugTrace::ForOneFrame, 
        HitResults,
        true
        );


    TArray<FBAttackReturn> Outputs;

    for (const FHitResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();
        FVector HitLocation = Hit.ImpactPoint;
        FBAttackReturn Output;
        
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


void AEnemyBoss::ExplodeTransition()
{
    if (CurrentHealth <= MaxHealth/2 && Phase1)
    {
        Phase1 = false;
        PhaseChanged();
    }
}

bool AEnemyBoss::TookHeavyDamage(float Damage)
{
    TakenDamage = Damage + TakenDamage;

    if (TakenDamage > 200.0f)
    {
        TakenDamage = TakenDamage - 200.0f;
        CanHitAnim = true;
        return true;
    }
    else
    {
        return false;
    }
}



void AEnemyBoss::FireBossProjectile(FVector FirePoint)
{
    if (!ProjectileClass) return;

    AActor* PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!PlayerActor) return;

    FVector SpawnLocation = FirePoint + GetActorForwardVector() * 45.0f; 
    FRotator SpawnRotation = (PlayerActor->GetActorLocation() - SpawnLocation).Rotation();

    

    GetWorld()->SpawnActor<ASmallProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
}

