#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <ncurses.h>

#define N 6 //Numero de filósofos

#define IZQUIERDA(x)	((x) !=0 ? (x)-1 : (N-1)) //Calcula el filosofo de la izqda
#define DERECHA(x)		(((x)+1)%N) //Calcula el filosofo de la dcha

typedef enum{
	PENSANDO,HAMBRIENTO,COMIENDO
}status;

status estado[N]; //Estado en el que esta cada proceso en cada momento (pensando, hambriento o comiendo)
sem_t mutex,s[N]; //Mutex: Solo un filosofo puede coger o soltar palillos; s[N]: Controlamos si he podido coger palillos o no, si no he podido, me bloqueo hasta que nos despierte el filosofo de la izqda o dcha

void pensar(int id);
void comer(int id);
void coger_palillos(int id);
void Comprobar(int id);
void soltar_palillos(int id);

int main()
{
	extern status estado[N]; //No es necesario
	extern sem_t mutex,s[N]; //No es necesario

	int v[N]/*para decirle a cada hilo quien es*/,value,i;
	sem_init(&mutex,0,1);
	pthread_t filosofo[N]; //Nombre de los hilos correspondiente a los filosofos
	pthread_t imp;        //Nombre del hilo de impresion
	void *Filosofo(void* ); //Funcion filosofo
	void *imprimir(void*); //Funcion de impresion

	for(i=0;i<N;i++)				//NO SE PUEDEN LANZAR LOS HILOS HASTA QUE LOS SEMAFOROS NO ESTEN INICIALIZADOS
	{
		sem_init(&s[i],0,0);
		estado[i]=PENSANDO;

	}

	pthread_create(&imp,NULL,imprimir,(void *) &v[i]);

	for(i=0;i<N;i++)
	{
		v[i]=i;
		if(value=pthread_create(&filosofo[i],NULL,Filosofo, (void *) &v[i]))
			exit(value);

	}
	
	for(i=0;i<N;i++)
	{
		pthread_join(filosofo[i],NULL);
	}

	pthread_join(imp,NULL);
	return 0;
}

void *imprimir(void* i){

	initscr();			//Inicio de biblioteca ncurses 		
	cbreak();	
	keypad(stdscr, TRUE);
	scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);	
	noecho();
	int varmenu=1;
	while(varmenu==1){

		if (getch() =='s'){	
			varmenu=0;
		}

		mvprintw(1,0,"Presionar s para salir");


		//Imprimir filosofo 0
		if (estado[0]==PENSANDO)
		{	
			if(estado[1]!=COMIENDO){mvprintw(4,23,"| ");}
			mvprintw(2,30,"PENSANDO   ");
			if(estado[5]!=COMIENDO){mvprintw(4,44,"| ");}
		}

		if (estado[0]==HAMBRIENTO)
		{
			mvprintw(2,30,"HAMBRIENTO");
		}

		if (estado[0]==COMIENDO)
		{
			mvprintw(4,23,"  ");
			mvprintw(2,30,"COMIENDO   ");
			mvprintw(4,44,"  ");
		}

		//Imprimir filosofo 1
		if (estado[1]==PENSANDO)
		{	
			if(estado[0]!=COMIENDO)mvprintw(4,23,"| ");
			mvprintw(6,18,"PENSANDO   ");
			if(estado[2]!=COMIENDO)mvprintw(8,23,"| ");
		}

		if (estado[1]==HAMBRIENTO)
		{
			mvprintw(6,18,"HAMBRIENTO");
		}

		if (estado[1]==COMIENDO)
		{
			mvprintw(4,23,"  ");
			mvprintw(6,18,"COMIENDO   ");
			mvprintw(8,23,"  ");
		}


		//Imprimir filosofo 2
		if (estado[2]==PENSANDO)
		{	
			if(estado[1]!=COMIENDO)mvprintw(8,23,"| ");
			mvprintw(10,18,"PENSANDO   ");
			if(estado[3]!=COMIENDO)mvprintw(12,23,"| ");
		}

		if (estado[2]==HAMBRIENTO)
		{
			mvprintw(10,18,"HAMBRIENTO");
		}

		if (estado[2]==COMIENDO)
		{
			mvprintw(8,23,"  ");
			mvprintw(10,18,"COMIENDO   ");
			mvprintw(12,23,"  ");
		}

		
		//Imprimir filosofo 3
		if (estado[3]==PENSANDO)
		{	
			if(estado[2]!=COMIENDO){mvprintw(12,23,"| ");}
			mvprintw(14,30,"PENSANDO   ");
			if(estado[4]!=COMIENDO){mvprintw(12,44,"| ");}
		}

		if (estado[3]==HAMBRIENTO)
		{
			mvprintw(14,30,"HAMBRIENTO");
		}

		if (estado[3]==COMIENDO)
		{
			mvprintw(12,23,"  ");
			mvprintw(14,30,"COMIENDO   ");
			mvprintw(12,44,"  ");
		}

		//Imprimir filosofo 4
		if (estado[4]==PENSANDO)
		{	
			if(estado[3]!=COMIENDO)mvprintw(12,44,"| ");
			mvprintw(10,44,"PENSANDO   ");
			if(estado[5]!=COMIENDO)mvprintw(8,44,"| ");
		}

		if (estado[4]==HAMBRIENTO)
		{
			mvprintw(10,44,"HAMBRIENTO");
		}

		if (estado[4]==COMIENDO)
		{
			mvprintw(8,44,"  ");
			mvprintw(10,44,"COMIENDO   ");
			mvprintw(12,44,"  ");
		}

		//Imprimir filosofo 5
		if (estado[5]==PENSANDO)
		{	
			if(estado[0]!=COMIENDO)mvprintw(4,44,"| ");
			mvprintw(6,44,"PENSANDO   ");
			if(estado[4]!=COMIENDO)mvprintw(8,44,"| ");
		}

		if (estado[5]==HAMBRIENTO)
		{
			mvprintw(6,44,"HAMBRIENTO");
		}

		if (estado[5]==COMIENDO)
		{
			mvprintw(4,44,"  ");
			mvprintw(6,44,"COMIENDO   ");
			mvprintw(8,44,"  ");
		}

		refresh();	
	}
	endwin();

}


void *Filosofo(void* i)
{
	int id,j;
	id=*(int *) i;

	while(true){ 
		pensar(id);
		coger_palillos(id);
		comer(id);
		soltar_palillos(id);
	}
}

void pensar(int id)
{
//	printf("Filosofo %d pensando\n",id);
	sleep (random() % 15);
}

void comer(int id)
{
//	printf("Filosofo %d comiendo\n",id);
	sleep (random() % 15);
}

void coger_palillos(int id)
{
	sem_wait(&mutex); //Si un filosofo esta cogiendo tenedores, no podre coger y me quedare bloqueado. Si no hay nadie cogiendo tenedores, cogere yo
	estado[id]=HAMBRIENTO;
	Comprobar(id);
	sem_post(&mutex); //Permito que otro filosofo coja tenedores
	sem_wait(&s[id]); //Si no hace el post de la funcion Comprobar(), se quedaria pillado aqui
}

void Comprobar(int id)
{
	if(estado[id]==HAMBRIENTO&&estado[IZQUIERDA(id)]!=COMIENDO&&estado[DERECHA(id)]!=COMIENDO)
	{
		estado[id]=COMIENDO;
		sem_post(&s[id]); //Sirve para liberar al filosofo para que no se quede pillado mas adelante, puesto que ha podido coger tenedores
	}
}

void soltar_palillos(int id)
{
	sem_wait(&mutex); //Si hay alguien cogiendo tenedores, nadie mas puede coger. Si estoy cogiendo tenedores, bloqueo para que nadie mas pueda hacerlo 
	estado[id]=PENSANDO;
	Comprobar(IZQUIERDA(id));
	Comprobar(DERECHA(id));
	sem_post(&mutex);
}