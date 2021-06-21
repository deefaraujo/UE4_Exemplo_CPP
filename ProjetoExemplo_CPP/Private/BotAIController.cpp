// Fill out your copyright notice in the Description page of Project Settings.

#include "BotAIController.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "BotCharacter.h"
#include "Weapon.h"

ABotAIController::ABotAIController()
{
    //Componente de sensores do BotCharacter
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(
        FName("PawnSensing"));
    //Configuração dos sensores do BotCharacter
    //Varredura sensorial a cada 1/4 de segundo (.25f)
    PawnSensingComp->SensingInterval = .25f;
    //Detecção sensivel apenas de Player
    PawnSensingComp->bOnlySensePlayers = true;
    //Angulo da visão do BotCharacter será de 90 graus
    PawnSensingComp->SetPeripheralVisionAngle(90.f);
    //O raio de visão foi setado em 3000
    PawnSensingComp->SightRadius = 3000.f;

    //Criação da BT para manipular a BT criada dentro da UE4
    BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(
        FName("BehaviorTreeComponent"));
    //Criação da BB para manipular a BB (incluir variaveis e etc) criada dentro da UE4
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(
        FName("BlackBoardComp"));
}

void ABotAIController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);
    //OnSeePawn -> Ao ver o jogador irá disparar essa função
    PawnSensingComp->OnSeePawn.AddDynamic(this,
                                          &ABotAIController::OnSeePawn);

    if (BehaviorTree)
    {
        //Inicializar o Blackboard e a BehaviorTree
        BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
        BehaviorTreeComp->StartTree(*BehaviorTree);
        //Se não ver o jogador vai definir a patrulha como verdadeiro
        BlackboardComp->SetValueAsBool("Patrol", true);
    }
}

void ABotAIController::OnSeePawn(APawn *SensedPawn)
{
    if (BlackboardComp && SensedPawn)
    {
        //Mensagem na tela quando o jogador for detectado
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
                                         TEXT("Eu vejo você!"));

        //Definir Enemy como Object no BB
        BlackboardComp->SetValueAsObject("Enemy", SensedPawn);
        //Função acionada quando o inimigo VER o jogador
        BlackboardComp->SetValueAsBool("Patrol", false);
        //Verifica se o BotCharacter esta sendo possuindo por este Controller
        ABotCharacter *Bot = Cast<ABotCharacter>(GetPawn());
        //Chama a função Atirar neste BotCharacter
        Bot->WeaponEnemy->Fire();
    }
}