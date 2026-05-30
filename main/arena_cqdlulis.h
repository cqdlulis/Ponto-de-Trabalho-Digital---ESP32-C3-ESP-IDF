#ifndef ARENA_CQDLULIS_H
#define ARENA_CQDLULIS_H

#include <stdlib.h>
#include <stdio.h>

typedef struct{
    size_t size;
    size_t offset;
    unsigned char *memory;
}arena;

static arena setArena(size_t size); // seta arena
static void *arenaAlloc(arena *arena, size_t size); // aloca espaço na arena
static void freeArena(arena *arena); // libera espaço da arena
static void resetArena(arena *arena); // reseta o offset para zero sem limpar a arena

static arena setArena(size_t size){

    arena a;

    a.size = size;
    a.offset = 0;
    a.memory = malloc(size);
    if(a.memory == NULL) {
        printf("Erro ao setar memória.");
        a.size = 0;
        a.offset = 0;
    }

    return a;
    
}

static void *arenaAlloc(arena *arena, size_t size){
    if(size > arena->size - arena->offset){
        printf("Erro ao alocar memória.");
        return NULL;
    }

    unsigned char *ptr = arena->memory + arena->offset;
    arena->offset += size;

    return ptr;

}

static void freeArena(arena *arena){
    free(arena->memory);    
    arena->offset = 0;
    arena->size = 0;
    arena->memory = NULL;
}

static void resetArena(arena *arena){
    arena->offset = 0;
}

#endif