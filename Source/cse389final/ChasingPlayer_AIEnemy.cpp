// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasingPlayer_AIEnemy.h"
#include "PatrollingAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"

// Sets default values
AChasingPlayer_AIEnemy::AChasingPlayer_AIEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCollisionDetection = CreateDefaultSubobject<USphereComponent>(TEXT("Player Collision Detection"));
	PlayerCollisionDetection->SetupAttachment(RootComponent);

	PlayerAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Player Attack Collision"));
	PlayerAttackCollision->SetupAttachment(GetMesh(), TEXT("Fist"));

	PlayerDetected = false;
	CanAttackPlayer = false;
}

// Called when the game starts or when spawned
void AChasingPlayer_AIEnemy::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<APatrollingAIController>(GetController());

	if (AIController == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("No AIController found!"));
		return;
	}


	AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AChasingPlayer_AIEnemy::OnAIMoveCompleted);

	PlayerCollisionDetection->OnComponentBeginOverlap.AddDynamic(this,
		&AChasingPlayer_AIEnemy::OnPlayerDetectedOverlapBegin);

	PlayerCollisionDetection->OnComponentEndOverlap.AddDynamic(this,
		&AChasingPlayer_AIEnemy::OnPlayerDetectedOverlapEnd);

	PlayerAttackCollision->OnComponentBeginOverlap.AddDynamic(this,
		&AChasingPlayer_AIEnemy::OnPlayerAttackOverlapBegin);

	PlayerAttackCollision->OnComponentEndOverlap.AddDynamic(this,
		&AChasingPlayer_AIEnemy::OnPlayerAttackOverlapEnd);
}

// Called every frame
void AChasingPlayer_AIEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChasingPlayer_AIEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChasingPlayer_AIEnemy::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!PlayerDetected) {
		AIController->RandomPatrol();
	}
	else {
		MoveToPlayer();
	}
}

void AChasingPlayer_AIEnemy::MoveToPlayer()
{
	if (Player) {
		AIController->MoveToLocation(Player->GetActorLocation(), StoppingDistance, true);
	}
}

void AChasingPlayer_AIEnemy::SeekPlayer()
{
	MoveToPlayer();
	GetWorld()->GetTimerManager().SetTimer(SeekPlayerTimerHandle, this,
		&AChasingPlayer_AIEnemy::SeekPlayer, 0.25f, true);
}

void AChasingPlayer_AIEnemy::StopSeekingPlayer()
{
	GetWorld()->GetTimerManager().ClearTimer(SeekPlayerTimerHandle);
}

void AChasingPlayer_AIEnemy::OnPlayerDetectedOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<AThePlayer>(OtherActor);
	if (Player) {
		PlayerDetected = true;
		SeekPlayer();
	}
}

void AChasingPlayer_AIEnemy::OnPlayerDetectedOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Player = Cast<AThePlayer>(OtherActor);
	if (Player) {
		PlayerDetected = false;
		StopSeekingPlayer();
		AIController->RandomPatrol();
	}
}

void AChasingPlayer_AIEnemy::OnPlayerAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<AThePlayer>(OtherActor);
	if (Player) {
		PlayerDetected = true;
		CanAttackPlayer = true;
	}
}

void AChasingPlayer_AIEnemy::OnPlayerAttackOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Player = Cast<AThePlayer>(OtherActor);
	if (Player) {
		CanAttackPlayer = false;

		SeekPlayer();
	}
}

bool AChasingPlayer_AIEnemy::GetCanAttackPlayer()
{
	return CanAttackPlayer;
}

