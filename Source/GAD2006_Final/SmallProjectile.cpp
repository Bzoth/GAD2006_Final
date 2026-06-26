#include "SmallProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ASmallProjectile::ASmallProjectile()
{
    // Mermi için her kare çalışacak bir mantığa gerek yok, performans için Tick kapalı.
    PrimaryActorTick.bCanEverTick = false; 

    // Çarpışma Küresi Kurulumu
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComponent->InitSphereRadius(15.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
    
    // Çarpma fonksiyonunu fizik olayına bağlıyoruz
    CollisionComponent->OnComponentHit.AddDynamic(this, &ASmallProjectile::OnHit);
    RootComponent = CollisionComponent;

    // Görsel Efekt Bileşeni
    ProjectileEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileEffect"));
    ProjectileEffect->SetupAttachment(RootComponent);

    // Mermi Hareketi Ayarları (Düz gitmesi ve yerçekimsiz olması için)
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
    ProjectileMovement->UpdatedComponent = CollisionComponent;
    ProjectileMovement->InitialSpeed = InitialSpeed;
    ProjectileMovement->MaxSpeed = MaxSpeed;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.0f; // Souls büyüleri havada süzülür, aşağı düşmez

    // Boşluğa sıkılırsa 5 saniye sonra hafızadan otomatik silinsin
    InitialLifeSpan = 5.0f;
}

void ASmallProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void ASmallProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    AActor* MyOwner = GetOwner();
    
    // Çarptığımız obje geçerliyse, kendimiz veya sahibimiz değilse hasar ver
    if (OtherActor && OtherActor != this && OtherActor != MyOwner)
    {
        UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, UDamageType::StaticClass());

        // Patlama efekti oluştur
        if (ImpactEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, GetActorLocation(), GetActorRotation());
        }

        // Mermiyi yok et
        Destroy();
    }
}