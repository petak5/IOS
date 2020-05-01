#ifndef __PROJ2_H__
#define __PROJ2_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>

#define INVALID_ARGUMENTS_MESSAGE "Error: invalid arguments.\n"
#define FAILED_TO_OPEN_FILE "Error: failed to open file.\n"

#define SEM_NAME(n) "xurgos00.ios.proj2."#n

#define FILE_PATH "proj2.out"

typedef struct params
{
    long PI;
    int IG;
    int JG;
    int IT;
    int JT;
} params_t;

typedef struct semaphores
{
    sem_t *write;
    sem_t *checked;
    sem_t *confirmed;
    sem_t *noJudge;
    sem_t *allSignedIn;
} semaphores_t;

typedef struct sm
{
    int judge_entered;
    int A;
    int I;
    int NE;
    int NC;
    int NB;
} sm_t;

params_t parse_params(int argc, char **argv);

void init_resources();

void free_resources();

void judge(params_t par);

void imm_gen(params_t par);

void immigrant(params_t par, int number);

#endif // __PROJ2_H__