#include "proj2.h"

FILE *f;
semaphores_t sem;
sm_t *sm;

int main(int argc, char *argv[])
{
    params_t par = parse_params(argc, argv);

    init_resources();

    setvbuf(f, NULL, _IONBF, 0);

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

    waitpid(imm_gen_pid, NULL, 0);
    waitpid(judge_pid, NULL, 0);

    free_resources();

    return 0;
}

void judge(params_t par)
{
    long count_of_confirmed = 0;
    while (count_of_confirmed < par.PI)
    {
        if (par.JG != 0)
        {
            usleep((random() % (par.JG)) * 1000);
        }

        // want to enter
        sem_wait(sem.write);
        fprintf(f, "%d\t: JUDGE\t\t: wants to enter\n", sm->A);
        sm->A++;
        sem_post(sem.write);

        // enter
        sem_wait(sem.noJudge);
        sem_wait(sem.checked);
        sem_wait(sem.write);
        fprintf(f, "%d\t: JUDGE\t\t: enters\t\t\t\t\t\t: %d\t\t: %d\t\t: %d\n", sm->A, sm->NE, sm->NC, sm->NB);
        sm->A++;
        sm->judge_entered = 1;
        sem_post(sem.write);

        if (sm->NE > sm->NC)
        {
            sem_wait(sem.write);
            fprintf(f, "%d\t: JUDGE\t\t: waits for imm\t: %d\t\t\t\t\t\t: %d\t\t: %d\t\t: %d\n", sm->A, 0, sm->NE, sm->NC, sm->NB);
            sm->A++;
            sem_post(sem.write);

            sem_post(sem.checked);
            sem_wait(sem.allSignedIn);
        }

        // start confirmation
        sem_wait(sem.write);
        fprintf(f, "%d\t: JUDGE\t\t: starts confirmation\t\t\t: %d\t\t: %d\t\t: %d\n", sm->A, sm->NE, sm->NC, sm->NB);
        sm->A++;
        sem_post(sem.write);

        if (par.JT != 0)
        {
            usleep((random() % (par.JT)) * 1000);
        }

        // end confirmation
        sem_wait(sem.write);
        for (int i = 0; i < sm->NC; i++)
        {
            sem_post(sem.confirmed);
        }
        count_of_confirmed += sm->NC;
        sm->NE = 0;
        sm->NC = 0;
        fprintf(f, "%d\t: JUDGE\t\t: ends confirmation\t\t\t\t: %d\t\t: %d\t\t: %d\n", sm->A, sm->NE, sm->NC, sm->NB);
        sm->A++;
        sem_post(sem.write);

        if (par.JT != 0)
        {
            usleep((random() % (par.JT)) * 1000);
        }
        
        // leave
        sem_wait(sem.write);
        fprintf(f, "%d\t: JUDGE\t\t: leaves\t\t\t\t\t\t: %d\t\t: %d\t\t: %d\n", sm->A, sm->NE, sm->NC, sm->NB);
        sm->A++;
        sm->judge_entered = 0;
        sem_post(sem.checked);
        sem_post(sem.noJudge);
        sem_post(sem.write);
    }

    // finish
    sem_wait(sem.write);
    fprintf(f, "%d\t: JUDGE\t\t: finishes\n", sm->A);
    sm->A++;
    sem_post(sem.write);

    exit(0);
}

void imm_gen(params_t par)
{
    pid_t pids[par.PI];

    for (long i = 0; i < par.PI; i++)
    {
        if (par.IG != 0)
        {
            usleep((random() % (par.IG)) * 1000);
        }

        pid_t new_pid = fork();
        if (new_pid == 0)
        {
            immigrant(par, i);
        }
        else if (new_pid < 0)
        {
            fprintf(stderr, "Error: failed to fork this process.\n");
            exit(1);
        }

        pids[i] = new_pid;
    }
    
    for (long i = 0; i < par.PI; i++)
    {
        waitpid(pids[i], NULL, 0);
    }

    exit(0);
}

void immigrant(params_t par, int number)
{
    // start
    sem_wait(sem.write);
    fprintf(f, "%d\t: IMM %d\t\t: starts\n", sm->A, number);
    sm->A++;
    sem_post(sem.write);

    // enter
    sem_wait(sem.noJudge);
    sem_wait(sem.write);
    sm->NE++;
    sm->NB++;
    fprintf(f, "%d\t: IMM %d\t\t: enters\t\t\t\t\t\t: %d\t\t: %d\t\t: %d\n", sm->A, number, sm->NE, sm->NC, sm->NB);
    sm->A++;
    sem_post(sem.write);
    sem_post(sem.noJudge);

    // check
    sem_wait(sem.checked);
    sem_wait(sem.write);
    sm->NC++;
    fprintf(f, "%d\t: IMM %d\t\t: checks\t\t\t\t\t\t: %d\t\t: %d\t\t: %d\n", sm->A, number, sm->NE, sm->NC, sm->NB);
    sm->A++;
    sem_post(sem.write);

    if ((sm->judge_entered == 1) && (sm->NE == sm->NC))
    {
        sem_post(sem.allSignedIn);
    }
    else
    {
        sem_post(sem.checked);
    }

    // wait for confirmation
    sem_wait(sem.confirmed);

    // want certificate
    sem_wait(sem.write);
    fprintf(f, "%d\t: IMM %d\t\t: wants certificate\t\t\t\t: %d\t\t: %d\t\t: %d\n", sm->A, number, sm->NE, sm->NC, sm->NB);
    sm->A++;
    sem_post(sem.write);

    if (par.IT != 0)
    {
        usleep((random() % (par.IT)) * 1000);
    }

    // get certificate
    sem_wait(sem.write);
    fprintf(f, "%d\t: IMM %d\t\t: got certificate\t\t\t\t: %d\t\t: %d\t\t: %d\n", sm->A, number, sm->NE, sm->NC, sm->NB);
    sm->A++;
    sem_post(sem.write);

    // leave
    sem_wait(sem.noJudge);
    sem_wait(sem.write);
    sm->NB--;
    fprintf(f, "%d\t: IMM %d\t\t: leaves\t\t\t\t\t\t: %d\t\t: %d\t\t: %d\n", sm->A, number, sm->NE, sm->NC, sm->NB);
    sm->A++;
    sem_post(sem.write);
    sem_post(sem.noJudge);

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
    /* is this needed? */sem_unlink(SEM_NAME(write));
    if ((sem.write = sem_open(SEM_NAME(write), O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
    {
        fprintf(stderr, "Error: failed to open semaphore.\n");
        exit(1);
    }
    /* is this needed? */sem_unlink(SEM_NAME(checked));
    if ((sem.checked = sem_open(SEM_NAME(checked), O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
    {
        fprintf(stderr, "Error: failed to open semaphore.\n");
        exit(1);
    }
    /* is this needed? */sem_unlink(SEM_NAME(confirmed));
    if ((sem.confirmed = sem_open(SEM_NAME(confirmed), O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
        fprintf(stderr, "Error: failed to open semaphore.\n");
        exit(1);
    }
    /* is this needed? */sem_unlink(SEM_NAME(noJudge));
    if ((sem.noJudge = sem_open(SEM_NAME(noJudge), O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
    {
        fprintf(stderr, "Error: failed to open semaphore.\n");
        exit(1);
    }
    /* is this needed? */sem_unlink(SEM_NAME(allSignedIn));
    if ((sem.allSignedIn = sem_open(SEM_NAME(allSignedIn), O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
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
    sm->judge_entered = 0;
    sm->A = 1;
    sm->I = 0;
    sm->NE = 0;
    sm->NC = 0;
    sm->NB = 0;
}

void free_resources()
{
    // Output file
    fclose(f);

    // Semaphores
    sem_unlink(SEM_NAME(write));
    sem.write = NULL;
    sem_unlink(SEM_NAME(checked));
    sem.checked = NULL;
    sem_unlink(SEM_NAME(confirmed));
    sem.confirmed = NULL;
    sem_unlink(SEM_NAME(noJudge));
    sem.noJudge = NULL;
    sem_unlink(SEM_NAME(allSignedIn));
    sem.allSignedIn = NULL;

    // Shared Memory
    munmap(sm, sizeof(sm_t));
}