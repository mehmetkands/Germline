// Ekinezya 2022

#pragma once


#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h" 
#include "Camera/CameraShake.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "Weapons/WeaponsEQP.h"
#include "WeaponMp5k.generated.h"


UCLASS(Blueprintable)
class GELECEGINGUNLUGU_API AWeaponMp5k : public AActor
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
		class UStaticMeshComponent* WeaponPlugin;

	UPROPERTY(EditAnywhere, Category = Mesh)
		class UStaticMeshComponent* ArmPlugin;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		class UParticleSystem* MuzzleParticles;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		class UParticleSystem* MuzzleSmoke;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		class USceneComponent* MuzzleLocation;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* Muzzlefx;

	UPROPERTY()
		UAnimBlueprint* WeaponAnimBP;

	UPROPERTY()
		UStaticMesh* RedDotSight;

	UPROPERTY()
		UStaticMesh* RedScopeSight;

public:
	// Sets default values for this actor's properties
	AWeaponMp5k(const FObjectInitializer& object_initializer);

	UPROPERTY(EditAnywhere)
		bool RedDot = false;

	UPROPERTY(EditAnywhere)
		bool Scope = false;

	UPROPERTY(EditAnywhere)
		float CurrentAmmo=30.0f;

	UPROPERTY(EditAnywhere)
		float Ammunition =300.0f;

	UPROPERTY(EditAnywhere)
		float MagazineCapacity=30.0f;

	UPROPERTY(EditAnywhere)
		float WeaponDamageAmmount = 20.0f;

	UPROPERTY(EditAnywhere)
		FString WeaponName;

	//Animasyon montajlarý
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* ReloadEmptyAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* EQPAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WeaponFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ReloadTime = 1.70f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AutoReloadTime = 2.07f;

	UPROPERTY(EditAnywhere)
		float AnimationSpeedParameter = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StopFxTime = 3.0f;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* MuzzleEffectFxTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

	//Parçacýk Sistemi
	UPROPERTY(Category = Mesh, EditAnywhere)
		class UParticleSystem* FireFXParticle;

	UParticleSystemComponent* MuzzleHeatParticle;

	UParticleSystemComponent* Firefx;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	//Alt sýnýflar
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UMatineeCameraShake> FiringShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bfireable=true;


	//Sayýsal fonksiyonlar
	float SetWeaponAmmo(float Value);

	float SetWeaponHave(float Value);

	float GetWeaponAmmo();

	float GetWeaponHave();

	//UFonksiyonlar

	UFUNCTION(BlueprintCallable)
		void StartFire();

	UFUNCTION(BlueprintCallable)
		void StopFire();

	UFUNCTION(BlueprintCallable)
		void Reload();

	UFUNCTION(BlueprintCallable)
		void AutoReload();

	//Fonksiyonlar
	void ReloadAnim();

	void ReloadEmptyAnimation();

	void EQPAnimation();

	void FireSound();

	void FireShot();

	void EnableFire();

	void MuzzleHeatfx();

	void StopMuzzlefx();

	FTimerHandle FireTimer;

	ACharacter* OwnerActor;
};