// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTPS.generated.h"

UCLASS()
class PROJETOEXEMPLO_CPP_API ACharacterTPS : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterTPS();

protected:
	virtual void BeginPlay() override;

	//Definindo a função do input "Move Forward"
	//Movimentar o personagem para frente e para trás
	void MoveForward(float Valor);

	//Definindo a função do input "Move Sideways"
	//Movimentar o personagem para direita e para esquerda
	void MoveSideways(float Valor);

	//Em UPROPERTY (Visibilidade,Permissão De Acesso,Categoria)
	//Pode chamar funções, variaveis...

	//Definindo um componente chamado "CameraCharacter" do tipo UCameraComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent *CameraCharacter;

	//Definindo um componente chamado "SpringArm" do tipo USpringArmComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent *SpringArm;

	//Definindo uma variavel capaz de receber
	//qualquer tipo de classe Weapon ou SubClasse de Weapon
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AWeapon> BP_Weapon_Rifle;

	//Definindo uma variavel do tipo bool para verificar
	//se o personagem esta pulando ou não
	UPROPERTY(BlueprintReadOnly, Category = "Jump")
	bool bIsJumping;

	//Função para o personagem pular
	void Jump();

	//Função para o personagem parar de pular
	void StopJump();

	//Função para o personagem agachar
	void KCrouch();

	//Função para o personagem levantar
	void KUnCrouch();

	//Função para o personagem atirar
	void Fire();

public:
	class AWeapon* WeaponPlayer;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

};
