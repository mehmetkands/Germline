// Fill out your copyright notice in the Description page of Project Settings.


#include "GpObjects/Health.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/GeleceginGunluguCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AHealth::AHealth()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Character"));
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("TheBox"));
	Box->SetupAttachment(Mesh);

}

// Called when the game starts or when spawned
void AHealth::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &AHealth::AddHealth);
	
}

//Karakter temas ederse
void AHealth::AddHealth(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGeleceginGunluguCharacter* GGC = Cast <AGeleceginGunluguCharacter>(OtherActor))
	{
		if (GGC->PlayerHealth < 100)
		{
			if (HealthPickupSfx != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, HealthPickupSfx, GetActorLocation());
			}

			GGC->AddHealth(HealValue);

			Destroy();
		}
	}
}

