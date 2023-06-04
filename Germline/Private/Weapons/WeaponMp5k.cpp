
#include "Weapons/WeaponMp5k.h"
#include "Weapons/Projectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/InputComponent.h"

// Sets default values
AWeaponMp5k::AWeaponMp5k(const FObjectInitializer& object_initializer)
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponName = "Heckler & Koch MP-5 Submachine Gun";

	//RedDot d�rb�n�n yolu
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RedDotSight(TEXT("/Game/Main/Assets/Models/Rifles/EQP/Dot/EOTech_Xps2-1"));

	//Scope d�rb�n�n yolu
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RedScopeSight(TEXT("/Game/Main/Assets/Models/Rifles/EQP/Scope/model"));

	//Animasyonbp bulucu
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBPObj(TEXT("/Game/Main/Assets/Models/Rifles/Mp5/NMP5Animbp"));


	//Silah modelinin ayarlanmas�
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));

	WeaponMesh->SetRelativeScale3D(FVector(3.f, 3.f, 3.f)); //b�y�kl�k ayar� default olarak ��t�r.

	WeaponMesh->SetAnimInstanceClass(WeaponAnimBP->GeneratedClass);

	//Silah eklentilerinin ayarlanmas�

	WeaponPlugin = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponPlugin"));

	WeaponPlugin->SetupAttachment(WeaponMesh, "RedDot");

	//Karakter elinin eklentilerinin ayarlanmas�

	ArmPlugin = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmPlugin"));

	ArmPlugin->AttachToComponent(WeaponMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), ("clocksocket"));

	//Namlu lokasyonu

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));

	MuzzleLocation->SetupAttachment(WeaponMesh);

	//Namlu efektleri
	Muzzlefx = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleFX"));

	Muzzlefx->SetupAttachment(WeaponMesh);

	FireFXParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("Fire Effectfx"));

	MuzzleEffectFxTransform = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleEffectfxLocation"));

	//RedDot eklentisi tak�l�ysa
	if (RedDot)
	{
		if (RedDotSight.Succeeded())
		{
			RedDotSight = RedDotSight.Object;
		}

		WeaponPlugin->SetStaticMesh(RedDotSight);
		if (WeaponPlugin)
		{
			WeaponPlugin->AttachToComponent(WeaponMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), ("RedDot"));
		}
	}

	//Scope eklentisi tak�l�ysa
	if (Scope)
	{
		if (RedScopeSight.Succeeded())
		{
			RedScopeSight = RedScopeSight.Object;
		}

		WeaponPlugin->SetStaticMesh(RedScopeSight);
		if (WeaponPlugin)
		{
			WeaponPlugin->AttachToComponent(WeaponMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), ("RedDot"));
		}
	}

	//** Animasyon blueprint **//
	if (AnimBPObj.Succeeded())
	{
		WeaponAnimBP = AnimBPObj.Object;
	}

	GunOffset = FVector(0.0f, 0.0f, -10.0f);
}

// Called when the game starts or when spawned
void AWeaponMp5k::BeginPlay()
{
	Super::BeginPlay();

	EQPAnim();
}

// Called every frame
void AWeaponMp5k::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AWeaponMp5k::Enablefire()
{
	bfireable = true;
}

// Silah� elimize ald���m�zda oynat�lacak animasyon
void AWeaponMp5k::EQPAnimation()
{
	if (EQPAnimation != NULL)
	{
		UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();

		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(EQPAnimation, AnimationSpeedParameter);
		}
	}
}

void AWeaponMp5k::ReloadAnimation() 
{
	if (ReloadAnimation != NULL)
	{

		UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();

		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(ReloadAnimation, AnimationSpeedParameter);
		}
	}
}

void AWeaponMp5k::ReloadEmptyAnimation()
{
	if (ReloadEmptyAnimation != NULL)
	{

		UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();

		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(ReloadEmptyAnimation, AnimationSpeedParameter);
		}
	}
}



//�ste�e ba�l� ek namlu kozmetikleri(duman,sis vs)
void AWeaponMp5k::MuzzleHeatfx()
{
	FVector Scale(1.0f, 1.0f, 1.0f);

	if (FireFXParticle != NULL)
	{
		MuzzleHeatParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireFXParticle, MuzzleLocation->GetComponentLocation());
		MuzzleHeatParticle->SetRelativeScale3D(Scale);
	}

	FTimerHandle StopTime;

	GetWorldTimerManager().SetTimer(StopTime, this, &AWeaponMp5k::StopMuzzlefx, StopFxTime, true);
}

void AWeaponMp5k::StopMuzzlefx()
{
	FVector ZeroScale(0.0f, 0.0f, 0.0f);

	if (MuzzleHeatParticle != NULL)
	{
		MuzzleHeatParticle->SetRelativeScale3D(ZeroScale);
	}
}

//At�� Sesi
void AWeaponMp5k::Firesound()
{
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

//Ate� etme fonksiyonu
void AWeaponMp5k::FireShot()
{
	if (CurrentAmmo >= 1 && FiringShake != NULL && bfireable == true)
	{
		Firesound();
		AGeleceginGunluguCharacter* Fpscam = Cast< AGeleceginGunluguCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		CurrentAmmo -= 1;

		//Ate� ederken kameran�n sallanmas�
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(FiringShake);

		FTimerHandle Time;

		//Oto doldurma denetleyici, mermi s�f�rlan�nca silah otomatik dolar.
		if (CurrentAmmo == 0)
		{
			GetWorldTimerManager().SetTimer(Time, this, &AWeaponMp5k::AutoReload, 1.0f, false); //animasyonun b�l�nmemesi i�in bir saniye ertele
		}

		if (ProjectileClass != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				const FRotator SpawnRotation = Fpscam->GetControlRotation();

				const FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : Fpscam->GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				FActorSpawnParameters ActorSpawnParams;

				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

				Projectile->DamageAmmount = WeaponDamageAmmount;

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticles, MuzzleLocation->GetComponentLocation());

				//Merminin kime ait oldu�unu bul. Ayr�ca hit mark� ekrana basar
				if (Projectile)
				{
					Projectile->FoundOwnerActor(OwnerActor);
				}

				if (FireAnimation != NULL)
				{
					UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();

					if (AnimInstance != NULL)
					{
						AnimInstance->Montage_Play(FireAnimation, AnimationSpeedParameter);

					}
				}
			}
		}
	}

}

//Seri ate� edilecekse
void AWeaponMp5k::StartFire()
{
	GetWorldTimerManager().SetTimer(FireTimer, this, &AWeaponMp5k::FireShot, WeaponFireRate, true);
}

//Seri ate�i durdur
void AWeaponMp5k::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireTimer);
}

//Yeniden doldurma fonksiyonu
void AWeaponMp5k::Reload()
{
	if (CurrentAmmo <= MagazineCapacity-1 && Ammunition != 0)
	{
		bfireable = false;

		FTimerHandle NormalReloadTimerHandle;

		GetWorldTimerManager().SetTimer(NormalReloadTimerHandle, this, &AWeaponMp5k::Enablefire, ReloadTime, false);

		ReloadAnim();

		RequiredAmountOfAmmo = (MagazineCapacity - CurrentAmmo);

		if (RequiredAmountOfAmmo > Ammunition)
		{
	
			CurrentAmmo = CurrentAmmo + Ammunition;
			Ammunition = 0;
		}

		else
		{
			CurrentAmmo += CurrentAmmo;
			Ammunition -= RequiredAmountOfAmmo;
		}
	}
}

//Otomatik Mermi doldurucu
void AWeaponMp5k::AutoReload()
{
	if (CurrentAmmo <= 0 && Ammunition != 0)
	{
		bfireable = false;
		FTimerHandle ReloadTimerHandle;
		GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AWeaponMp5k::Enablefire, AutoReloadTime, false);
		ReloadEmptyAnim();

		if (Ammunition < MagazineCapacity)
		{
			CurrentAmmo += Ammunition;

			Ammunition = 0;
		}

		else
		{
			CurrentAmmo = MagazineCapacity;
			Ammunition = Ammunition - MagazineCapacity;
		}
	}
}


float AWeaponMp5k::SetWeaponAmmo(float Value)
{
	CurrentAmmo += Value;
	return CurrentAmmo;
}

float AWeaponMp5k::SetWeaponHave(float Value)
{
	Ammunition = Ammunition + Value;
	return Ammunition;
}

float AWeaponMp5k::GetWeaponAmmo()
{
	return CurrentAmmo;
}

float AWeaponMp5k::GetWeaponHave()
{
	return Ammunition;
}
