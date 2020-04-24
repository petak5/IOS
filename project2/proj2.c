#include "proj2.h"

FILE *f;
semaphores_t sem;
sm_t *sm;

int main(int argc, char *argv[])
{
    params_t par = parse_params(argc, argv);

    init_resources();

    // Judge
    pid_t judge_pid = fork();
    if (judge_pid == 0)
    {
        judge(par);
    }
    else if (judge_pid < 0)
    {
        fprintf(stderr, "Error: failed to fork this process.\n");
        return 1;
    }

    // Immigrants generator
    pid_t imm_gen_pid = fork();
    if (imm_gen_pid == 0)
    {
        imm_gen(par);
    }
    else if (imm_gen_pid < 0)
    {
        fprintf(stderr, "Error: failed to fork this process.\n");
        return 1;
    }

    waitpid(judge_pid, NULL, 0);
    waitpid(imm_gen_pid, NULL, 0);

    sem_wait(sem.write);
    fprintf(f, "Main process finishing...\n");
    sem_post(sem.write);

    free_resources();

    return 0;
}

void judge(params_t par)
{
    usleep((random() % (par.JG)) * 1000);

    sem_wait(sem.write);
    fprintf(f, "%d  JUDGE   wants to enter.\n", sm->message_count);
    sm->message_count++;
    sem_post(sem.write);

    exit(0);
}

void imm_gen(params_t par)
{
    sem_wait(sem.write);
    fprintf(f, "%d  Generating immigrants.\n", sm->message_count);
    sm->message_count++;
    sem_post(sem.write);

    if (par.PI == 0) exit(0);

    exit(0);
}

void immigrant(params_t par)
{
    if (par.PI == 0) exit(0);

    exit(0);
}

params_t parse_params(int argc, char **argv)
{
    if (argc != 6)
    {
        fprintf(stderr, INVALID_ARGUMENTS_MESSAGE);
        exit(1);
    }

    params_t par;

    long temp;

    char *s = NULL;
    par.PI = strtol(argv[1], &s, 10);
    if ((strcmp(s, "") != 0) ||
        (par.PI < 1))
    {
        fprintf(stderr, INVALID_ARGUMENTS_MESSAGE);
        exit(1);
    }

    s = NULL;
    temp = strtol(argv[2], &s, 10);
    if ((strcmp(s, "") != 0) ||
        (temp < 0) ||
        (temp > 2000))
    {
        fprintf(stderr, INVALID_ARGUMENTS_MESSAGE);
        exit(1);
    }
    par.IG = temp;

    s = NULL;
    temp = strtol(argv[3], &s, 10);
    if ((strcmp(s, "") != 0) ||
        (temp < 0) ||
        (temp > 2000))
    {
        fprintf(stderr, INVALID_ARGUMENTS_MESSAGE);
        exit(1);
    }
    par.JG = temp;

    s = NULL;
    temp = strtol(argv[4], &s, 10);
    if ((strcmp(s, "") != 0) ||
        (temp < 0) ||
        (temp > 2000))
    {
        fprintf(stderr, INVALID_ARGUMENTS_MESSAGE);
        exit(1);
    }
    par.IT = temp;

    s = NULL;
    temp = strtol(argv[5], &s, 10);
    if ((strcmp(s, "") != 0) ||
        (temp < 0) ||
        (temp > 2000))
    {
        fprintf(stderr, INVALID_ARGUMENTS_MESSAGE);
        exit(1);
    }
    par.JT = temp;

    return par;
}

void init_resources()
{
    // Output file
    f = fopen(FILE_PATH, "w");
    if (f == NULL)
    {
        fprintf(stderr, FAILED_TO_OPEN_FILE);
        exit(1);
    }

    // Semaphores
    /* is this needed? */sem_unlink(SEM_NAME("write"));
    if ((sem.write = sem_open(SEM_NAME("write"), O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
    {
        fprintf(stderr, "Error: failed to open semaphore.\n");
        exit(1);
    }

    // Shared memory
    if ((sm = mmap(NULL, sizeof(sm_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
    {
        fprintf(stderr, "Error: failed to allocate shared memory.\n");
        exit(1);
    }
    sm->message_count = 1;
}

void free_resources()
{
    // Output file
    fclose(f);

    // Semaphores
    sem_unlink(SEM_NAME("write"));
    sem.write = NULL;

    // Shared Memory
    munmap(sm, sizeof(sm_t));
}