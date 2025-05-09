// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "Components/CapsuleComponent.h"
#include "ThePlayer.generated.h"

UCLASS()
class CSE389FINAL_API AThePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AThePlayer();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int currLasersInMag;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
		FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* iamove;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* ialook;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* iashoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* iajump;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* iareload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ProjectileSpawn;

	UCapsuleComponent* CollisionComp;

	int score;
	float health;
	int lasersInMag = 15;

	FTimerHandle ReloadTimerHandle;
	bool isReloading = false;
	bool canShoot = true;
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* reloadSound;
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* shootSound;

	UFUNCTION(BlueprintCallable)
	int GetLasersInMag();

	void SetScore(int score);

	UFUNCTION(BlueprintCallable)
	int GetScore();

	UFUNCTION(BlueprintCallable)
	float GetHealth();

	void ChangeHealth(float health);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void move(const FInputActionValue& value);
	void look(const FInputActionValue& value);
	void shoot();
	void jump(const FInputActionValue& value);
	void reload();
	void finishReloading();
};
