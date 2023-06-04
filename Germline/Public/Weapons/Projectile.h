// Ekinezya 2022

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Characters/GeleceginGunluguCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h" 
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Projectile.generated.h"

UCLASS(config = Game)
class AProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		class UStaticMeshComponent* Bullet;

	//Projectile movement component
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	//Mermi ýþýðý
	UPROPERTY(EditAnywhere, Category = Projectile)
		UPointLightComponent* Light;

public:
	AProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DamageAmmount = 50; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BulletLife = 10.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BulletGravityValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDamageValid;

	//Fizik Etkisi
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BulletPhysicalVelocityEffect = 0.2f; //Default fizik etki deðeri

	//Görsel fiziksel efektler
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* Blood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* StoneSplash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* WoodSplash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* IronSplash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* WaterSplash;

	//Fiziksel sesler
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* StoneImpactSfx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* WoodImpactSfx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* MetalImpactSfx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* WaterImpactSfx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* BulletFlybySfx;

	//Merminin flyby ses efekti için trigger
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* LeftBox;

	//Merminin flyby ses efekti için trigger
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* RightBox;

	UPROPERTY(BlueprintReadOnly)
		AGeleceginGunluguCharacter* GGCPlayer;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		AActor* Causer;

	//Mermi çarptýðýnda
	UFUNCTION(BlueprintCallable)
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void LeftBoxBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void RightBoxBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		void FoundOwnerActor(AActor* Actor);
};

