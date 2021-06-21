// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterTPS.h"

//Incluir o Componente de Inputs
#include "Components/InputComponent.h"
//Incluir o Componente da Camera
#include "Camera/CameraComponent.h"
//Incluir o Componente da SpringArm
#include "GameFramework/SpringArmComponent.h"
//Incluir o Componente de Movimentação de Pawn
#include "GameFramework/PawnMovementComponent.h"
//Incluir o Componente de Movimentação de Personagem
#include "GameFramework/CharacterMovementComponent.h"
//Incluir o Componente de Movimentação de Personagem
#include "GameFramework/Character.h"
//Incluir a Weapon
#include "Weapon.h"

//Incluir alguns arquivos adicionais
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Engine/Engine.h"

ACharacterTPS::ACharacterTPS()
{

	PrimaryActorTick.bCanEverTick = true;
	//Criar um objeto da classe SpringArm
	//utilizando a template USpringArmComponent
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmCamera"));

	//Definir a distancia da camera para com o personagem
	SpringArm->TargetArmLength = 200.f;

	//Ativar a utilização da rotação do personagem
	SpringArm->bUsePawnControlRotation = true;

	//Definindo a localizacao do SpringArm
	SpringArm->AddRelativeLocation(FVector(0.0f, 40.0f, 50.0f));

	//Ativar o Camera Lag para deixar a camera mais suave
	SpringArm->bEnableCameraLag = true;

	//Configurando o valor da velocidade do Camera Lag
	SpringArm->CameraLagSpeed = 20.f;

	//Ativar o Rotation Camera Lag para deixar a rotaçao da camera mais suave
	SpringArm->bEnableCameraRotationLag = true;

	//Configurando o valor da velocidade do Camera Rotation Lag
	SpringArm->CameraRotationLagSpeed = 10.f;

	//Definindo o SpringArm como o componente raiz
	SpringArm->SetupAttachment(RootComponent);

	//Criando um objeto de classe CameraCharacter
	//utilizando a template UCameraComponent
	CameraCharacter = CreateDefaultSubobject<UCameraComponent>(FName("CameraCharacter"));

	//Definindo a CameraCharacter como "filha" do SpringArm
	CameraCharacter->SetupAttachment(SpringArm);

	//Ativar a possibilidade do personagem agachar
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	//Configurações do Jump
	//Ajustando o controle no ar do pulo
	GetCharacterMovement()->AirControl = 0.05f;
	//Ajustando a velocidade do pulo
	GetCharacterMovement()->JumpZVelocity = 425.f;
	//Ajustando a escala da gravidade
	GetCharacterMovement()->GravityScale = 1.5;

	//Ajustar o tamanho da capsula do personagem quando agacha
	GetCharacterMovement()->CrouchedHalfHeight = 70.f;

	//Ativando a posse automatica do jogador
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ACharacterTPS::BeginPlay()
{
	Super::BeginPlay();

	//TArray = Vector em C++
	TArray<int32> MyArrayUnreal;
	MyArrayUnreal.Add(-20);
	MyArrayUnreal.Add(30);
	MyArrayUnreal.Add(92);
	MyArrayUnreal.Add(5);
	MyArrayUnreal.Add(49);

	for (int32 element : MyArrayUnreal)
	{
		//Mensagem na tela
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
										 FString::Printf(TEXT(" %d "), element));
	}

	//Configurando os parametros do SpawnActor
	//Definindo o struct FActorSpawnParameters como Params
	FActorSpawnParameters Params;
	//Definindo o tipo de colisão do Spawn como AlwaysSpawn
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//Criando e chamando a função SpawnActor do tipo classe AWeapon especificamente
	//a arma BP_Weapon_Rifle, com localização,rotação e escala
	WeaponPlayer = GetWorld()->SpawnActor<AWeapon>(
		BP_Weapon_Rifle, FTransform(), Params);

	//Anexar a arma ao personagem, mais precisamente ao weaponSocket
	//criado no proprio esqueleto do personagem
	WeaponPlayer->AttachToComponent(Cast<USceneComponent>(GetMesh()),
									FAttachmentTransformRules::SnapToTargetIncludingScale,
									FName("weaponSocket"));
}

//Função do Input de Mover para Frente (Teclas já definidas na UE4)
void ACharacterTPS::MoveForward(float Valor)
{
	//Na UE4 foi definido 1.0 para Frente e -1.0 para Trás
	//Assim o personagem poderá mover também para trás
	AddMovementInput(GetActorForwardVector() * Valor);
}

//Função do Input de Mover para Esquerda e Direita (Teclas já definidas na UE4)
void ACharacterTPS::MoveSideways(float Valor)
{
	//Na UE4 foi definido 1.0 para Direita e -1.0 para Esquerda
	//Assim o personagem poderá mover também para a esquerda
	AddMovementInput(GetActorRightVector() * Valor);
}

//Função para Pular
void ACharacterTPS::Jump()
{
	//setando a bool como verdadeira
	bIsJumping = true;
}

//Função para Parar de Pular
void ACharacterTPS::StopJump()
{
	//setando a bool como falsa
	bIsJumping = false;
}

//Função para Agachar
void ACharacterTPS::KCrouch()
{
	Crouch();
}

//Função para Levantar
void ACharacterTPS::KUnCrouch()
{
	UnCrouch();
}

//Função para Atirar
void ACharacterTPS::Fire()
{
	WeaponPlayer->Fire();
}

void ACharacterTPS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterTPS::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Quando o input "Move Forward" for acionado
	//será chamada a função MoveForward
	PlayerInputComponent->BindAxis("Move Forward", this,
								   &ACharacterTPS::MoveForward);

	//Quando o input "Move Sideways" for acionado
	//será chamada a função MoveSideways
	PlayerInputComponent->BindAxis("Move Sideways", this,
								   &ACharacterTPS::MoveSideways);

	//Quando o input "Look Up" for acionado
	//será acionada a função AddControllerPitchInput
	//que vai rotacionar a "visão" no eixo X
	PlayerInputComponent->BindAxis("Look Up", this,
								   &ACharacterTPS::AddControllerPitchInput);

	//Quando o input "Look Sideways" for acionado
	//será acionada a função AddControllerYawInput
	//que vai rotacionar a "visão" no eixo Z
	PlayerInputComponent->BindAxis("Look Sideways", this,
								   &ACharacterTPS::AddControllerYawInput);

	//Quando o input "Crouch" for pressionado
	//será acionada a função KCrouch (Agachar)
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed,
									 this, &ACharacterTPS::KCrouch);
	//Quando o input "Crouch" for soltado
	//será acionada a função KUnCrouch (Levantar)
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Released,
									 this, &ACharacterTPS::KUnCrouch);

	//Quando o input "Jump" for pressionado
	//será acionada a função Jump (Pular)
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed,
									 this, &ACharacterTPS::Jump);

	//Quando o input "Jump" for soltado
	//será acionada a função StopJump (Parar de Pular)
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released,
									 this, &ACharacterTPS::StopJump);

	//Quando o input "Fire" for pressionado
	//será acionada a função Fire (Atirar)
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed,
									 this, &ACharacterTPS::Fire);
}
