// Ekinezya 2022


#include "Characters/ZombieCharacter.h"
#include "Characters/GeleceginGunluguCharacter.h"
#include "AI/ZombieAIController.h"

// Sets default values
AZombieCharacter::AZombieCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	DamageBox=CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));

	DamageBox->SetupAttachment(GetCapsuleComponent());

	DamageBox->OnComponentBeginOverlap.AddDynamic(this, &AZombieCharacter::BeginOverlap);

}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AZombieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Killer();
}


float AZombieCharacter::TakeDamage(float TakeDamageAmmount)
{
	if(Health>0)
	{
		Health -= TakeDamageAmmount;
	}

	return Health;
}

//Karakteri öldürür
void AZombieCharacter::Killer()
{
	if (Health <= 0)
	{
		AZombieAIController* AIPC = Cast<AZombieAIController>(GetController());

		if (AIPC)
		{
			//Yapay zekayý durdur
			AIPC->SysStopLogic();
		}

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); //Kapsül komponenti kaldýr

		GetMesh()->SetSimulatePhysics(true); //Ragdoll fizigini aktif et

		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly); 

		GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);

		FTimerHandle DestroyTimer;

		GetWorldTimerManager().SetTimer(DestroyTimer,this, &AZombieCharacter::Destroyer, CharacterDestroyOnWorldTime, false); //Belirlenen zamanda, karakteri dünyadan kaldýr.
	
	}
}

//Yok edici fonksiyon
void AZombieCharacter::Destroyer()
{
	Destroy();
}

//Hasar verici trigger
void AZombieCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGeleceginGunluguCharacter* GGCAct = Cast< AGeleceginGunluguCharacter>(OtherActor))
	{
		if (Health > 0)
		{
			FPointDamageEvent DamageEvent;

			GGCAct->TakeDamage(ZombieDamageAmmount, DamageEvent, GetInstigatorController(), this);
		}
	}
}
