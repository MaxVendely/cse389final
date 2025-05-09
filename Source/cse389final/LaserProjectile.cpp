// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserProjectile.h"

// Sets default values
ALaserProjectile::ALaserProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	collComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	collComp->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	collComp->OnComponentHit.AddDynamic(this, &ALaserProjectile::OnHit);
	collComp->InitSphereRadius(15.f);
	RootComponent = collComp;
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Justin/Assets/laser/Isk.Isk"));
	if (Mesh.Succeeded())
		meshComp->SetStaticMesh(Mesh.Object);
	meshComp->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));
	meshComp->SetupAttachment(RootComponent);

	PMComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	PMComp->SetUpdatedComponent(collComp);
	PMComp->InitialSpeed = 1000.f;
	PMComp->MaxSpeed = 1000.f;
	PMComp->bRotationFollowsVelocity = true;
	PMComp->ProjectileGravityScale = 0.f;
	InitialLifeSpan = 5.f;
}

// Called when the game starts or when spawned
void ALaserProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaserProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserProjectile::FireInDirection(const FVector& ShootDirection) {
	PMComp->Velocity = ShootDirection * PMComp->InitialSpeed;
}

void ALaserProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
}

