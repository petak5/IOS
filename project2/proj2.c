#include "proj2.h"

int main(int argc, char *argv[])
{
    params_t par = parse_params(argc, argv);

    return 0;
}

params_t parse_params(int argc, char **argv)
{
    if (argc != 6)
    {
        fprintf(stderr, INVALID_ARGUMENTS_MESSAGE);
        exit(1);
    }

    params_t par;

    char *s = NULL;
    par.PI = strtol(argv[1], &s, 10);
    if ((strcmp(s, "") != 0) ||
        (par.PI < 1))
    {
        fprintf(stderr, INVALID_ARGUMENTS_MESSAGE);
        exit(1);
    }

    s = NULL;
    par.IG = strtol(argv[2], &s, 10);
    if ((strcmp(s, "") != 0) ||
        (par.IG < 0) ||
        (par.IG > 2000))
    {
        fprintf(stderr, INVALID_ARGUMENTS_MESSAGE);
        exit(1);
    }

    s = NULL;
    par.JG = strtol(argv[3], &s, 10);
    if ((strcmp(s, "") != 0) ||
        (par.JG < 0) ||
        (par.JG > 2000))
    {
        fprintf(stderr, INVALID_ARGUMENTS_MESSAGE);
        exit(1);
    }

    s = NULL;
    par.IT = strtol(argv[4], &s, 10);
    if ((strcmp(s, "") != 0) ||
        (par.IT < 0) ||
        (par.IT > 2000))
    {
        fprintf(stderr, INVALID_ARGUMENTS_MESSAGE);
        exit(1);
    }

    s = NULL;
    par.JT = strtol(argv[5], &s, 10);
    if ((strcmp(s, "") != 0) ||
        (par.JT < 0) ||
        (par.JT > 2000))
    {
        fprintf(stderr, INVALID_ARGUMENTS_MESSAGE);
        exit(1);
    }

    return par;
}