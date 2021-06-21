// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BotAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJETOEXEMPLO_CPP_API ABotAIController : public AAIController
{
	GENERATED_BODY()

protected:
	ABotAIController();

	//Função que dá controle ao Pawn(Enemy) ao Controller
	//BotCharacter será controlado por BotAIController
	//quando for possuido(OnPosses)
	virtual void OnPossess(APawn *InPawn) override;

	//Classe que tem configurações e funcionalidades sensoriais (visão, audição...)
	UPROPERTY(EditDefaultsOnly)
	class UPawnSensingComponent *PawnSensingComp;

	//Classe da Arvore da IA
	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree *BehaviorTree;

	//Classe de Componente da Arvore da IA
	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTreeComponent *BehaviorTreeComp;

	//Classe de Componente da BlackBoard
	UPROPERTY(EditDefaultsOnly)
	class UBlackboardComponent *BlackboardComp;

	//Função "Avistar um Pawn" que determinará outros eventos a partir dela
	UFUNCTION()
	void OnSeePawn(class APawn *SensedPawn);
};
