// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThePlayer.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "ChasingFlag_AIEnemy.generated.h"

UCLASS()
class CSE389FINAL_API AChasingFlag_AIEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChasingFlag_AIEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool PlayerDetected;
	bool CanAttackPlayer;

	class AThePlayer* Player;

	UPROPERTY(EditAnywhere)
		class USphereComponent* PlayerCollisionDetection;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* PlayerAttackCollision;

	class APatrollingAIController* AIController;

	void OnAIMoveCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);

	UPROPERTY(EditAnywhere)
		float StoppingDistance = 200.f;

	FTimerHandle SeekPlayerTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void MoveToPlayer();

	UFUNCTION()
		void SeekPlayer();

	UFUNCTION()
		void StopSeekingPlayer();

	UFUNCTION()
		void OnPlayerDetectedOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnPlayerDetectedOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnPlayerAttackOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnPlayerAttackOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		bool GetCanAttackPlayer();

};
