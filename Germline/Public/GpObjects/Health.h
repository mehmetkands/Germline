// Ekinezya 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Widgets/HealthStatusWidget.h"
#include "Health.generated.h"

UCLASS()
class GELECEGINGUNLUGU_API AHealth : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UBoxComponent* Box;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AHealth();

	UPROPERTY(Editanywhere)
		int HealValue = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* HealthPickupSfx;

	UFUNCTION()
		void AddHealth(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
