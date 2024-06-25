// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MyStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyRocket.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(45.f, 65.f, 15.f));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("/Script/Engine.StaticMesh'/Game/P38/Meshes/SM_P38_Body.SM_P38_Body'"));
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_Body(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/P38/Materials/MI_Body.MI_Body'"));
	if (MI_Body.Succeeded())
	{
		Body->SetMaterial(0, MI_Body.Object);
	}

	Left = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);
	Left->SetRelativeLocation(FVector(37.2f, 21.f, 0.5f));
	
	Right = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);
	Right->SetRelativeLocation(FVector(37.2f, -21.f, 0.5f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(TEXT("/Script/Engine.StaticMesh'/Game/P38/Meshes/SM_P38_Propeller.SM_P38_Propeller'"));
	if (SM_Propeller.Succeeded())
	{
		Left->SetStaticMesh(SM_Propeller.Object);
		Right->SetStaticMesh(SM_Propeller.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_Propeller(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/P38/Materials/MI_Propeller.MI_Propeller'"));
	if (MI_Propeller.Succeeded())
	{
		Left->SetMaterial(0, MI_Propeller.Object);
		Right->SetMaterial(0, MI_Propeller.Object);
	}
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);
	Arrow->SetRelativeLocation(FVector(80.f, 0, 0));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Box);
	SpringArm->TargetArmLength = 100.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	
	Boost = 0.5f;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(GetActorForwardVector(), Boost);
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyPawn::Pitch);
	PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AMyPawn::Roll);
	PlayerInputComponent->BindAction(TEXT("Boost"), IE_Pressed, this, &AMyPawn::PressBoost);
	PlayerInputComponent->BindAction(TEXT("Boost"), IE_Released, this, &AMyPawn::ReleaseBoost);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMyPawn::Fire);
}

void AMyPawn::Pitch(float Value)
{
	float Dir = FMath::Clamp(Value, -1.f, 1.f);
	
	AddActorLocalRotation(FRotator(Dir * 60.0f * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0, 0));
}

void AMyPawn::Roll(float Value)
{
	float Dir = FMath::Clamp(Value, -1.f, 1.f);

	AddActorLocalRotation(FRotator(0, 0, Dir * 60.0f * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
}

void AMyPawn::PressBoost()
{
	Boost = 1.0f;
}

void AMyPawn::ReleaseBoost()
{
	Boost = 0.5f;
}

void AMyPawn::Fire()
{
	GetWorld()->SpawnActor<AMyRocket>(Arrow->K2_GetComponentLocation(), Arrow->K2_GetComponentRotation());
}

