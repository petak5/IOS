#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

#define INVALID_ARGUMENTS_MESSAGE "Error: invalid arguments.\n"
#define FAILED_TOP_OPEN_FILE "Error: failed to open file.\n"

#define FILE_PATH "proj2.out"

typedef struct params {
    int PI;
    int IG;
    int JG;
    int IT;
    int JT;
} params_t;

params_t parse_params(int argc, char **argv);