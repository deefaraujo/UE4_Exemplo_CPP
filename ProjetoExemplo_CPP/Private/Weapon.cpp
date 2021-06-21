// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundBase.h"
#include "Engine/World.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/EngineTypes.h"
#include "Engine/Public/CollisionQueryParams.h"
#include "Engine/Public/DrawDebugHelpers.h"
#include "Materials/MaterialInterface.h"
#include "Math/UnrealMathUtility.h"

AWeapon::AWeapon()
{

	PrimaryActorTick.bCanEverTick = true;

	//Evitar crash
	MuzzleEffect = nullptr;
	BloodImpact = nullptr;
	GeneralImpact = nullptr;
	MuzzleSound = nullptr;

	//Criando um objeto da classe WeaponMesh
	//utilizando a classe USkeletalMeshComponent
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(
		FName("Weapon Mesh"));

	//Definindo qual é a aparencia da arma (SkeletalMesh)
	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		WeaponVisual(TEXT("SkeletalMesh'/Game/Blueprints/Weapons/Meshes/SK_Wep_Rifle_Assault_05.SK_Wep_Rifle_Assault_05'"));

	//Definindo qual o material da arma (Material)
	ConstructorHelpers::FObjectFinder<UMaterialInterface>
		WeaponSkin(TEXT("MaterialInstanceConstant'/Game/Blueprints/Weapons/Material/Weapon_Skin_Inst2.Weapon_Skin_Inst2'"));

	//Com tudo certo irá definir o visual padrão da arma
	if (WeaponVisual.Succeeded() && WeaponSkin.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(WeaponVisual.Object);
		WeaponMesh->SetMaterial(0, WeaponSkin.Object);
	}

	//Definindo o componente raiz como WeaponMesh
	RootComponent = WeaponMesh;

	//Criando um componente ArrowWeapon
	//utilizando a classe UArrowComponent
	ArrowWeapon = CreateDefaultSubobject<UArrowComponent>(FName("Arrow Weapon"));

	//Anexando o ArrowWeapon ao WeaponMesh
	//mais precisamente ao socket MuzzleFlashSocket do WeaponMesh
	ArrowWeapon->SetupAttachment(WeaponMesh, FName("MuzzleFlashSocket"));

	//Localização do ArrowWeapon
	ArrowWeapon->SetRelativeLocation(FVector(-1.0f, 0.f, -2.0f));

	//Escala do ArrowWeapon
	ArrowWeapon->SetRelativeScale3D(FVector(0.3f, 0.8f, 0.7f));
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::Fire()
{
	//Encontrar o componente de classe UArrowComponent
	if (ArrowWeapon)
	{
		//Pegando a localizacao e rotação do ArrowWeapon
		FVector Start = ArrowWeapon->GetComponentLocation();
		FVector Direction = ArrowWeapon->GetComponentRotation().Vector();
		//Calculo do raio
		FVector End = Start + (Direction * 1000);

		//Configurando a colisao e o Hit para o raio
		FHitResult InfoImpact;
		FCollisionQueryParams Params;

		//Configuração da colisão do raio
		//Impedir que o raio colida com o proprio ArrowWeapon
		Params.AddIgnoredActor(this);
		//Impedir que o raio colida com a malha da arma
		Params.AddIgnoredActor(GetOwner());
		//Ativando o trace
		Params.bTraceComplex = true;

		//Variavel para definir se acertou ou não
		bool hitSomething = GetWorld()->LineTraceSingleByChannel(
			InfoImpact, Start, End, ECollisionChannel::ECC_Visibility, Params);

		if (MuzzleSound)
		{
			UGameplayStatics::PlaySoundAtLocation(ArrowWeapon, MuzzleSound, Start);
		}

		if (hitSomething)
		{
			//Mensagem na tela
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString(("Acertou em algo")));

			//Identificar qual o Actor que o raio impactou
			AActor *Actor = InfoImpact.GetActor();

			//Se a classe do Actor que foi atingida for do tipo ASkeletalMeshActor
			//ou qualquer subclasse dela entra no If. BloodImpact -> True.
			if (Actor->GetClass()->IsChildOf(ASkeletalMeshActor::StaticClass()) &&
				BloodImpact)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(), BloodImpact, InfoImpact.Location,
					InfoImpact.ImpactNormal.Rotation(), true);
			}
			//Se não for um Actor de outro tipo será um impacto geral. GeneralImpact -> True.
			else if (GeneralImpact)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(), GeneralImpact, InfoImpact.Location,
					InfoImpact.ImpactNormal.Rotation(), true);

				//Definindo um furo para o tiro utilizando um Decal com tempo de duração
				FVector VariableScale = FVector(FMath::RandRange(10.f, 25.f));
				UGameplayStatics::SpawnDecalAttached(GeneralDecalImpact,
													 VariableScale, InfoImpact.GetComponent(), NAME_None,
													 InfoImpact.Location, InfoImpact.ImpactNormal.Rotation(),
													 EAttachLocation::KeepWorldPosition, 60.f);
			}
		}

		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f, (uint8)0, 1.0f);

		//Se tiver um efeito na arma será verdadeiro
		//Se MuzzleEffect tiver valor 0 é falso
		//Qualquer outro valor é verdadeiro
		if (MuzzleEffect)
		{
			//Pegando a localização e a rotação do ArrowWeapon
			FVector Location = ArrowWeapon->GetComponentLocation();
			FRotator Rotation = ArrowWeapon->GetComponentRotation();
			//Sendo valores iguais ao invez de definir os 3 valores
			//Define um valor para os X,Y,Z
			FVector Scale = FVector(0.9f);

			//Configurações da Particula
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(), MuzzleEffect, Location, Rotation, Scale, true);
		}
	}
}
