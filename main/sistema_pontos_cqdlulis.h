#ifndef SISTEMA_PONTOS_CQDLULIS_H
#define SISTEMA_PONTOS_CQDLULIS_H

#include "arena_cqdlulis.h"
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
    char username[50];
    char id;
    char entradahora[9];
    char saidahora[9];
    bool veiohoje;
    char atrasos;
    char faltas;
    char credencial[5];
}user;

void criarusuario(arena *a, char name[], char entrada[], char saida[], char credencial[]); // função para criar usuários
struct tm *horario_atual(); // função para facilitar a chamada do horário 
int segundos_hora(char horario[9]); // função para facilitar a chamada do horário
int segundos_horario_atual(); // função para chamar os segundos do dia baseado no horário presente
struct tm atraso_usuario(user *u); // função para calcular o tempo de atraso em h:m:s
int atraso_usuario_segundos(user *u); // função para calcular o tempo de atraso em s
void fim_do_dia(user *u); // função para contar faltas e fechar o dia
void chegou(user *u); // função quando o usuário chega no trabalho

//variáveis globais
int qt_max_de_usuarios = 13;
int id = 0;
int atraso_sec_tolerado = 600;
user *usuario[13];

void criarusuario(arena *a, char name[], char entrada[], char saida[], char credencial[]){
    usuario[id] = arenaAlloc(a, sizeof(user));
    usuario[id]->id = id;
    strcpy(usuario[id]->username, name);
    strcpy(usuario[id]->entradahora, entrada);
    strcpy(usuario[id]->saidahora, saida);
    strcpy(usuario[id]->credencial, credencial);
    usuario[id]->veiohoje = false;
    usuario[id]->atrasos = 0;
    usuario[id]->faltas = 0;
    id++;
}
struct tm *horario_atual(){
    time_t now = time(NULL);
    struct tm *temporeal =localtime(&now);
    return temporeal;
}
int segundos_horario_atual(){
    struct tm *horario = horario_atual();
    return horario->tm_hour *3600 +
        horario->tm_min *60 +
        horario->tm_sec;
}
int segundos_hora(char horario[9]){
    int hora, minuto, segundo;

    sscanf(horario, "%d:%d:%d/0", 
    &hora, &minuto, &segundo);
    return hora*3600 + 
        minuto*60 + 
        segundo;
}
struct tm atraso_usuario(user *u){
    struct tm atraso;

    int segundo_restantes = segundos_hora(u->entradahora) - segundos_horario_atual();

    atraso.tm_hour = segundo_restantes / 3600;
    atraso.tm_min = abs((segundo_restantes % 3600) / 60);
    atraso.tm_sec = abs((segundo_restantes % 3600) % 60);

    return atraso;
}

int atraso_usuario_segundos(user *u){
    return segundos_horario_atual() - segundos_hora(u->entradahora);
}

void fim_do_dia(user *u){ 
    if(u->veiohoje){
        u->veiohoje = false;
    }
    else{
        u->faltas += 1;
    }
}
void chegou(user *u){
    
    struct tm checkhorario = atraso_usuario(u);
    int atraso_segundos = atraso_usuario_segundos(u);

    u->veiohoje = true;
    
    if (atraso_segundos < 0){
      printf("Voce se adiantou e chegou%+02d:%02d:%02d antes do horario!\n",
      checkhorario.tm_hour,
      checkhorario.tm_min,
      checkhorario.tm_sec);
    }
    else{
      printf("Voce se atrasou e chegou %+02d:%02d:%02d apos o horario!\n",
      checkhorario.tm_hour,
      checkhorario.tm_min,
      checkhorario.tm_sec);

      if(atraso_segundos > atraso_sec_tolerado){
        u->atrasos += 1;
      }
    }
}

#endif