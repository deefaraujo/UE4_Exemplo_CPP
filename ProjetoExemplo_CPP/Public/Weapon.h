// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PROJETOEXEMPLO_CPP_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	//Definindo a função de atirar
	UFUNCTION()
	void Fire();

protected:
	virtual void BeginPlay() override;

	//Criando uma classe USkeletalMeshComponent chamada WeaponMesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAcess = "true"))
	class USkeletalMeshComponent *WeaponMesh;

	//Criando uma classe UArrowComponent chamada ArrowWeapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAcess = "true"))
	class UArrowComponent *ArrowWeapon;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	class USoundBase *MuzzleSound;

	//Criando uma classe do tipo UParticleSystem chamada MuzzleEffect
	UPROPERTY(EditAnywhere, Category = "WeaponEffects")
	class UParticleSystem *MuzzleEffect;

	//Criando uma classe do tipo UParticleSystem chamada BloodImpact
	UPROPERTY(EditAnywhere, Category = "ImpactEffects")
	class UParticleSystem *BloodImpact;

	//Criando uma classe do tipo UParticleSystem chamada GeneralImpact
	UPROPERTY(EditAnywhere, Category = "ImpactEffects")
	class UParticleSystem *GeneralImpact;

	UPROPERTY(EditAnywhere, Category = "ImpactEffects")
	class UMaterialInterface *GeneralDecalImpact;



public:
	virtual void Tick(float DeltaTime) override;
};
