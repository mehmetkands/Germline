// Ekinezya 2022

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "ZombieCharacter.generated.h"


UENUM(BlueprintType)
enum class ETeamType : uint8 //Takým sistemi
{
	Roman		UMETA(DisplayName = "Roman"),
	SECU		UMETA(DisplayName = "SECU"),
	Creature	UMETA(DisplayName = "Creature"),
	Pirate		UMETA(DisplayName = "Pirate"),
	Neutral		UMETA(DisplayName = "Neutral"),
};

UCLASS()
class GELECEGINGUNLUGU_API AZombieCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties
	AZombieCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* DamageBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ETeamType Team;

	UPROPERTY(EditAnywhere)
		float ZombieDamageAmmount;

	UPROPERTY(EditAnywhere)
		float CharacterDestroyOnWorldTime=10.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		float Health=100.0f;

	//Trigger için etkileþim algýlayýcý
	UFUNCTION(BlueprintCallable)
		void BeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Killer();

	void Destroyer();

	float TakeDamage(float TakeDamageAmmount);


};
