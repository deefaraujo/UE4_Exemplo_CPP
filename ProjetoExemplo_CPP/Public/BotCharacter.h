// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BotCharacter.generated.h"

UCLASS()
class PROJETOEXEMPLO_CPP_API ABotCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABotCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Definindo uma variavel capaz de receber
	//qualquer tipo de classe Weapon ou SubClasse de Weapon
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AWeapon> BP_Weapon_Rifle;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	class AWeapon *WeaponEnemy;
};
