#include "Weapons/Projectile.h"
#include "Characters/SecuCharacter.h"
#include "Characters/Ghost.h"
#include "Characters/CreatureScorpion.h"
#include "Characters/AssaultRobot.h"
#include "Characters/WormCharacter.h"
#include "Characters/SquidCharacter.h"
#include "Characters/ZombieCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AProjectile::AProjectile()
{
	bReplicates = true; 

	//Mermi Mesh ve ayarlarý
	Bullet = CreateDefaultSubobject<UStaticMeshComponent>(FName("BulletMesh"));

	Bullet->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
			
	Bullet->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));

	Bullet->CanCharacterStepUpOn = ECB_No;

	//Sol flyby trigger ayarlarý
	BoxLeft = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftBox"));

	BoxLeft->SetupAttachment(Bullet);

	BoxLeft->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::LeftBoxBeginOverlap);

	//Sað flyby trigger ayarlarý
	BoxRight= CreateDefaultSubobject<UBoxComponent>(TEXT("RightBox"));

	BoxRight->SetupAttachment(Bullet);

	BoxRight->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::RightBoxBeginOverlap);


	//ProjectileMovement ayarlarý(Component, hýz vb)
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileComp"));

	ProjectileMovement->UpdatedComponent = Bullet;

	ProjectileMovement->InitialSpeed = 20000.f;

	ProjectileMovement->bRotationFollowsVelocity = true;

	ProjectileMovement->bShouldBounce = false;

	ProjectileMovement->ProjectileGravityScale = BulletGravityValue;

	InitialLifeSpan = BulletLife;

	RootComponent = Bullet;
}

//Mermi isabet ettiðinde
void AProjectile::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
		//Çarpýlan obje tipini seç
		if (AZombieCharacter* Zombox = Cast< AZombieCharacter>(Hit.GetActor()))
		{
			if (Blood)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Blood, Hit.Location);
			}

			if (Zombox->Health > 0)
			{
				FPointDamageEvent DamageEvent;
				DamageEvent.HitInfo = Hit;
				Zombox->TakeDamage(DamageAmmount);

				if (GGCPlayer)
				{

					GGCPlayer->HitMarkShower(); //isabet olduðu için niþangaha isabet sembolü basar

				}
			}
		}

		if (AGhost* EnemyGhost = Cast<AGhost>(Hit.GetActor()))
		{
			FPointDamageEvent DamageEvent;
			DamageEvent.HitInfo = Hit;
			EnemyGhost->TakeDamage(DamageAmmount, DamageEvent, GetInstigatorController(), this);

			if (GGCPlayer)
			{
				GGCPlayer->HitMarkShower();
			}
		}

		if (AWormCharacter* EnemyWorm = Cast< AWormCharacter>(Hit.GetActor()))
		{
			FPointDamageEvent DamageEvent;
			DamageEvent.HitInfo = Hit;
			EnemyWorm->TakeDamage(DamageAmmount, DamageEvent, GetInstigatorController(), this);

			if (Blood)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Blood, Hit.Location);
			}

			if (GGCPlayer)
			{
				GGCPlayer->HitMarkShower();
			}

		}

		if (ASquidCharacter* EnemySquid = Cast< ASquidCharacter>(Hit.GetActor()))
		{
			FPointDamageEvent DamageEvent;
			DamageEvent.HitInfo = Hit;
			EnemySquid->TakeDamage(DamageAmmount);

			if (Blood)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Blood, Hit.Location);
			}

			if (GGCPlayer)
			{
				GGCPlayer->HitMarkShower();
			}

		}

		if (ACreatureScorpion* EnemyScorpion = Cast< ACreatureScorpion>(Hit.GetActor()))
		{
			FPointDamageEvent DamageEvent;
			DamageEvent.HitInfo = Hit;
			EnemyScorpion->TakeDamage(DamageAmmount, DamageEvent, GetInstigatorController(), this);

			if (GGCPlayer)
			{
				GGCPlayer->HitMarkShower();
			}
		}

		if (ASecuCharacter* EnemySecu = Cast< ASecuCharacter>(Hit.GetActor()))
		{
			ASecuCharacter* Secu = Cast< ASecuCharacter>(Causer); //Dost ateþi önleme

			if (Secu)
			{
				if (EnemySecu->Health && EnemySecu->TeamType != Secu->TeamType)
				{
					FPointDamageEvent DamageEvent;
					DamageEvent.HitInfo = Hit;
					EnemySecu->TakeDamage(DamageAmmount, DamageEvent, GetInstigatorController(), this);

					if (GGCPlayer)
					{
						GGCPlayer->HitMarkShower();
					}
				}
			}

			else
			{
				FPointDamageEvent DamageEvent;
				DamageEvent.HitInfo = Hit;
				EnemySecu->TakeDamage(DamageAmmount, DamageEvent, GetInstigatorController(), this);

				if (Blood)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Blood, Hit.Location);
				}

				if (GGCPlayer)
				{
					GGCPlayer->HitMarkShower();
				}
				
			}
		}

		if (AAssaultRobot* EnemyAssaultRobot = Cast< AAssaultRobot>(Hit.GetActor()))
		{
			AAssaultRobot* AssaultRobotCharacter = Cast< AAssaultRobot>(Causer); //Dost ateþi önleme

			if (AssaultRobotCharacter)
			{
				if (EnemyAssaultRobot->Health && EnemyAssaultRobot->Team != AssaultRobotCharacter->Team)
				{
					FPointDamageEvent DamageEvent;
					DamageEvent.HitInfo = Hit;
					SecuCharacter->TakeDamage(DamageAmmount, DamageEvent, GetInstigatorController(), this);

					if (GGCPlayer)
					{
						GGCPlayer->HitMarkShower();
					}
				}
			}

			else
			{
				if (EnemyAssaultRobot->Health)
				{
					FPointDamageEvent DamageEvent;
					DamageEvent.HitInfo = Hit;
					EnemyAssaultRobot->TakeDamage(DamageAmmount,true,Causer);

					if (GGCPlayer)
					{
						GGCPlayer->HitMarkShower();
					}
				}
			}
		}

		//Oyuncu karakterinin hasar almasý
		if (AGeleceginGunluguCharacter* GGCharacter = Cast< AGeleceginGunluguCharacter>(Hit.GetActor())) 
		{
				FPointDamageEvent DamageEvent;

				DamageEvent.HitInfo = Hit;

				GGCharacter->TakeDamage(DamageAmmount, DamageEvent, GetInstigatorController(), Causer);
		}
		

		// Çarptýðýn fizik objesine etki oluþtur
		if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * BulletPhysicalVelocityEffect, GetActorLocation());
		}

		if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
		{
			OtherActor->OnActorHit;
		}


		//Fizik Materyal Görsel ve Ses Efektleri
		if (Hit.PhysMaterial != NULL)
		{
			//Taþ
			if (Hit.PhysMaterial->SurfaceType == EPhysicalSurface::SurfaceType5)
			{
				if (StoneSplash)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), StoneSplash, Hit.Location);
				}

				if (StoneImpactSfx)
				{
					UGameplayStatics::PlaySoundAtLocation(this, StoneImpactSfx, GetActorLocation());
				}
			
			}

			if (Hit.PhysMaterial->SurfaceType == EPhysicalSurface::SurfaceType6)
			{
				//Su
				if (WaterSplash)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WaterSplash, Hit.Location);
				}

				if (WaterImpactSfx)
				{
					UGameplayStatics::PlaySoundAtLocation(this, WaterImpactSfx, GetActorLocation());
				}
		
			}

			if (Hit.PhysMaterial->SurfaceType == EPhysicalSurface::SurfaceType8)
			{
				//Tahta
				if (WoodSplash)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WoodSplash, Hit.Location);
				}

				if (WoodImpactSfx)
				{
					UGameplayStatics::PlaySoundAtLocation(this, WoodImpactSfx, GetActorLocation());
				}
		
			}

			if (Hit.PhysMaterial->SurfaceType == EPhysicalSurface::SurfaceType4)
			{
				//Demir & metal
				if (IronSplash)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), IronSplash, Hit.Location);
				}

				if (MetalImpactSfx)
				{
					UGameplayStatics::PlaySoundAtLocation(this, MetalImpactSfx, GetActorLocation());
				}
			
			}

		}

		Destroy();
}

//Mermi flyby sfx
void AProjectile::LeftBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGeleceginGunluguCharacter* GGC=Cast<AGeleceginGunluguCharacter>(OtherActor))
	{
		if (GGCPlayer != NULL)
		{
			if (GGC != GGCPlayer)
			{
				if (BulletFlybySfx)
				{
					UGameplayStatics::PlaySoundAtLocation(this, BulletFlybySfx, GetActorLocation());
				}
			}
		}
	}
}

//Mermi flyby sfx
void AProjectile::RightBoxBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGeleceginGunluguCharacter* GGC = Cast<AGeleceginGunluguCharacter>(OtherActor))
	{
		if (GGCPlayer != NULL)
		{
			if (GGC != GGCPlayer)
			{
				if (BulletFlybySfx)
				{
					UGameplayStatics::PlaySoundAtLocation(this, BulletFlybySfx, GetActorLocation());
				}
			}
		}
	}
}

void AProjectile::FoundOwnerActor(AActor* Actor)
{
	if (IsBullet == true)
	{
		GGCPlayer = Cast< AGeleceginGunluguCharacter>(Actor); //Ateþleyen oyuncu ise

		Causer = Actor;
	}
}
