#include "EnemyRanged.h"
#include "SmallProjectile.h"
#include "CharacterBase.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CollisionQueryParams.h"
#include "Engine/World.h"

AEnemyRanged::AEnemyRanged()
{
    BaseDamage = 15.0f; 
}

bool AEnemyRanged::CanSeePlayer(AActor* Player)
{
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

    FHitResult HitResult;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    
    bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(
        GetWorld(),
        GetActorLocation() + FVector(0.0f, 0.0f, 50.0f),
        Player->GetActorLocation() + FVector(0.0f, 0.0f, 50.0f),
        ObjectTypes,
        true,
        ActorsToIgnore,
        EDrawDebugTrace::ForOneFrame, 
        HitResult,
        true
    );
    
    return !bHit;
}


void AEnemyRanged::FireProjectile(FVector Head)
{
    if (!ProjectileClass) return;

    AActor* PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!PlayerActor) return;
    
    FVector TargetLocation = PlayerActor->GetActorLocation();
    
    
    FRotator SpawnRotation = (TargetLocation - GetActorLocation()).Rotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();

    
    GetWorld()->SpawnActor<ASmallProjectile>(ProjectileClass, Head + GetActorForwardVector() * 45, SpawnRotation, SpawnParams);
}
