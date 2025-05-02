// Fill out your copyright notice in the Description page of Project Settings.


#include "ThePlayer.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AThePlayer::AThePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	score = 0;
	health = 100;
}

// Called when the game starts or when spawned
void AThePlayer::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp = FindComponentByClass<UCapsuleComponent>();
	if (CollisionComp)
		CollisionComp->OnComponentHit.AddDynamic(this, &AThePlayer::OnHit);
}

// Called every frame
void AThePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AThePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	APlayerController* PC = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	subsystem->ClearAllMappings();
	subsystem->AddMappingContext(InputMapping, 0);
	UEnhancedInputComponent* pei = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	pei->BindAction(iamove, ETriggerEvent::Triggered, this, &AThePlayer::move);
	pei->BindAction(ialook, ETriggerEvent::Triggered, this, &AThePlayer::look);
	pei->BindAction(iashoot, ETriggerEvent::Triggered, this, &AThePlayer::shoot);
	pei->BindAction(iajump, ETriggerEvent::Triggered, this, &AThePlayer::jump);

}

void AThePlayer::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherComponent->GetCollisionProfileName().ToString() == "Enemy") {
		health -= 10;
		UE_LOG(LogTemp, Warning, TEXT("%d"), health);
	}

}

void AThePlayer::move(const FInputActionValue& value)
{
	if (Controller != nullptr) {
		const FVector2D moveValue = value.Get<FVector2D>();
		const FRotator moveRot(0, Controller->GetControlRotation().Yaw, 0);
		if (moveValue.Y != 0)
			AddMovementInput(moveRot.RotateVector(FVector::ForwardVector), moveValue.Y);
		if (moveValue.X != 0)
			AddMovementInput(moveRot.RotateVector(FVector::RightVector), moveValue.X);
	}
}

void AThePlayer::look(const FInputActionValue& value)
{
	if (Controller != nullptr) {
		const FVector2D lookValue = value.Get<FVector2D>();
		if (lookValue.X != 0.f)
			AddControllerYawInput(lookValue.X);
		if (lookValue.Y != 0.f)
			AddControllerPitchInput(-lookValue.Y);
	}
}

void AThePlayer::shoot()
{
	/*
	* ProjectileSpawn.Set(100.f, 0.f, 0.f);
	FVector SpawnLocation = GetActorLocation() + FTransform(GetActorRotation()).TransformVector(ProjectileSpawn);
	FRotator SpawnRotation = GetActorRotation();
	SpawnRotation.Pitch += 10.f;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	AProjectile* Bullet = GetWorld()->SpawnActor<AProjectile>(SpawnLocation, SpawnRotation, SpawnParams);
	if (Bullet) {
		FVector LaunchDirection = SpawnRotation.Vector();
		Bullet->FireInDirection(LaunchDirection);
	}
	*/
}

void AThePlayer::jump(const FInputActionValue& value)
{
	if (Controller != nullptr) {
		ACharacter* Character = Cast<ACharacter>(this);
		if (Character && !Character->GetCharacterMovement()->IsFalling())
			Character->Jump();
	}
}


void AThePlayer::SetScore(int newScore) {
	score = newScore;
}

int AThePlayer::GetScore() {
	return score;
}

int AThePlayer::GetHealth() {
	return health;
}

void AThePlayer::ChangeHealth(int newHealth) {
	health += newHealth;
}