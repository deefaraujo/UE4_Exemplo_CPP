// Fill out your copyright notice in the Description page of Project Settings.

#include "BotCharacter.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Weapon.h"

// Sets default values
ABotCharacter::ABotCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABotCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Configurando os parametros do SpawnActor
	//Definindo o struct FActorSpawnParameters como Params
	FActorSpawnParameters Params;
	//Definindo o tipo de colisão do Spawn como AlwaysSpawn
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//Criando e chamando a função SpawnActor do tipo classe AWeapon especificamente
	//a arma BP_Weapon_Rifle, com localização,rotação e escala
	WeaponEnemy = GetWorld()->SpawnActor<AWeapon>(
		BP_Weapon_Rifle, FTransform(), Params);

	//Anexar a arma ao inimigo, mais precisamente ao weaponSocket
	//criado no proprio esqueleto do inimigo
	WeaponEnemy->AttachToComponent(Cast<USceneComponent>(GetMesh()),
								   FAttachmentTransformRules::SnapToTargetIncludingScale,
								   FName("weaponSocket"));
}

// Called every frame
void ABotCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABotCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
