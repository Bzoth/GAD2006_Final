#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmallProjectile.generated.h"

UCLASS()
class GAD2006_FINAL_API ASmallProjectile : public AActor
{
	GENERATED_BODY()
    
public:    
	ASmallProjectile();

protected:
	virtual void BeginPlay() override;

public:    
	// --- BİLEŞENLER ---
	// Çarpışma algılayıcı küre
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class USphereComponent* CollisionComponent;

	// Fırlatma/Uçma fiziğini yöneten bileşen
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovement;

	// Uçarken arkasından çıkan büyü efekti
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visuals")
	class UParticleSystemComponent* ProjectileEffect;

	// --- AYARLAR & HASAR ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float InitialSpeed = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxSpeed = 2000.0f;

	// Hedefe çarptığında oluşacak patlama efekti
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	class UParticleSystem* ImpactEffect;

protected:
	// Çarpışma tetiklendiğinde çalışacak fonksiyon
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};