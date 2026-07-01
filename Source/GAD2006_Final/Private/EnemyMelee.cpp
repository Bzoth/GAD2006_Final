#include "EnemyMelee.h" 
#include "CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AEnemyMelee::AEnemyMelee()
{
    BaseDamage = 20.0f;
}

void AEnemyMelee::BeginPlay()
{
    Super::BeginPlay();
}


TArray<FMEAttackReturn> AEnemyMelee::EnemyTrace(FVector Fist)
{

    TArray<FHitResult> HitResults;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    
    bool bHit = UKismetSystemLibrary::SphereTraceMulti(
        GetWorld(),
        Fist,
        Fist,
        15.0f,
        UEngineTypes::ConvertToTraceType(ECC_Pawn),
        true,
        ActorsToIgnore,
        EDrawDebugTrace::None, 
        HitResults,
        true
        );


    TArray<FMEAttackReturn> Outputs;

    for (const FHitResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();
        FVector HitLocation = Hit.ImpactPoint;
        FMEAttackReturn Output;
        
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