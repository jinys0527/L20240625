// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRocket.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMyRocket::AMyRocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetGenerateOverlapEvents(true);
	Box->SetBoxExtent(FVector(50.f, 10.f, 10.f));

	Rocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket"));
	Rocket->SetupAttachment(Box);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Rocket(TEXT("/Script/Engine.StaticMesh'/Game/Rocket/Meshes/SM_Rocket.SM_Rocket'"));
	if (SM_Rocket.Succeeded())
	{
		Rocket->SetStaticMesh(SM_Rocket.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_Rocket(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Rocket/Materials/MI_Rocket.MI_Rocket'"));
	if (MI_Rocket.Succeeded())
	{
		Rocket->SetMaterial(0, MI_Rocket.Object);
	}

	Rocket->SetRelativeRotation(FRotator(-90.f, 0, 0));

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 3000.0f;
	Movement->MaxSpeed = 3000.0f;
	Movement->ProjectileGravityScale = 0.0f;
}

void AMyRocket::ProcessActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
}

// Called when the game starts or when spawned
void AMyRocket::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AMyRocket::ProcessActorBeginOverlap);
	OnActorBeginOverlap.RemoveDynamic(this, &AMyRocket::ProcessActorBeginOverlap);
	/*OnActorBeginOverlap.RemoveAll(this);*/

	SetLifeSpan(3.0f);
}

// Called every frame
void AMyRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

