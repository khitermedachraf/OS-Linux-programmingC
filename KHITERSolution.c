#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int fils_AB = -100;
int fils_BA = -100;
int direction = 0;
int gestionParPolice = 0;
int ctrl_c_clicked = 0;

void fils_actif(int sig)
{
    if (gestionParPolice == 1)
    {
        alarm(0);
        return;
    }
    if (fils_AB != -100)
        kill(fils_AB, SIGUSR1);
    if (fils_BA != -100)
        kill(fils_BA, SIGUSR1);
    alarm(3);
}

void policier_actif(int sig)
{
    printf("\nRecived CTRL Z.\n");
    alarm(0);
    ctrl_c_clicked = 0;
    gestionParPolice = 1;
    if (direction == 0)
    {
        printf("\nPolicier : Direction AB = FEU VERT [Vous pouvez passer... ].\n");
        direction = 1;
    }
    else
    {
        printf("\nPolicier : Direction BA = FEU VERT [Vous pouvez passer... 	].\n\n");
        direction = 0;
    }
}

void ctrl_c(int sig)
{
    printf("\nRecived CTRL C.\n");
    ctrl_c_clicked += 1;
    if (ctrl_c_clicked == 1)
    {
        printf("Premier click.\n");
        gestionParPolice = 0;
        signal(SIGALRM, fils_actif);
        alarm(3);
    }
    if (ctrl_c_clicked == 2)
    {
        // stop the program
        printf("Deuxieme click.\n\n");
        printf("**************************************\n");
        printf("Le pont est ferme pour les travaux -_- \n");
        printf("**************************************\n\n");
        kill(getpid(),SIGTERM);
    }
}

void fils1_p(int sig)
{

    if (direction == 0)
    {
        printf("Direction AB : FEU VERT [Vous pouvez passer... ].\n");
        direction = 1;
    }
    else
    {
        printf("Direction AB : FEU ROUGE [Vous ne pouvez pas passer ...].\n");
        direction = 0;
    }
}

void fils2_p(int sig)
{

    if (direction == 0)
    {
        printf("Direction BA : FEU ROUGE [Vous ne pouvez pas passer ...].\n");
        direction = 1;
    }
    else
    {
        printf("direction BA : FEU VERT [Vous pouvez passer... ].\n");
        direction = 0;
    }
}


int main()
{
    fils_AB = fork();
    if (fils_AB == 0)
    {
        signal(SIGUSR1, fils1_p);
        fils1_p(0);
    }
    else
    {
        fils_BA = fork();
    }
    if (fils_BA == 0)
    {
        signal(SIGUSR1, fils2_p);
        fils2_p(0);
    }
    if (fils_AB != 0 && fils_BA != 0)
    {
        signal(SIGTSTP, policier_actif);
        signal(SIGALRM, fils_actif);
        signal(SIGINT, ctrl_c);
        alarm(3);
    }
    while (1)
    {
    	pause();
    }
    
    return 0;
}
