#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct node {
	struct node * prev;
	int at;			//What time the process will come
	int bt;			//Execution time in CPU
	int v;			//Priority
	int qu;			//queue
	int qm;			//quantum
	int et;			//exit time
	int wt;			//waiting time
	int rt;			//response time
	int tt;			//turnaround time
	uint workDone;
	struct node *next;
	char * name;
};

//Double linked list
struct list {
	struct node head;
	struct node tail;
};

struct node * pop(struct list *lista){
	if(lista->head.next==NULL)
		return NULL;
	else{
		struct node * n = lista->head.next;
		lista->head.next = n->next;
		lista->head.prev = NULL;
		if (lista->head.next != NULL)
		{
			lista->head.next->prev = NULL;
		}
		n->next = NULL;
		printf("%s", n->name);
		return n;
	}
}

struct list cola;
struct list cola_prioridad, cola_r1, cola_r2;
struct node ** processes;
uint Nprocess=0;


void inicialisar_lista( struct list *lista){
	lista->head.prev = NULL;
	lista->head.next = NULL;
	lista->tail.prev = NULL;
	lista->tail.next = NULL;
}
uint length(struct  list *lista){
	uint temp =0;
	struct node * tmp = &lista->head;
	if(lista->head.next == NULL)
			return 0;
	else for (; tmp->next != NULL; temp++)
	{
		/* code */
		tmp=tmp->next;
	}
	return temp;
}




void add_back(struct list *lista, struct node *nodo){
if (lista->head.next == NULL)
	{
		nodo->prev = NULL;
		nodo->next = NULL;
		lista->head.next = nodo;
		lista->tail.prev = nodo;
	}
	else
	{
		lista->tail.prev->next = nodo;

		nodo->prev = lista->tail.prev;
		nodo->next = NULL;
		lista->tail.prev = nodo;
		lista->tail.next = NULL;
	}
}

void crear_proceso(char * str){
	char * copy = (char*)malloc(50*sizeof(char));

	int j = 0;
	for (; str[j] != '\0'; ++j)
	{
		copy[j] = str[j];
	} 
	copy[j] = '\0';
	//printf("%s", copy);

	
	char * ptr = (char*)strtok(copy, "\t");
//	printf("%s", ptr);
	struct node * processs = malloc(sizeof(struct list));
	processs->prev = NULL;
	processs->next = NULL;

	processs->name = malloc(5*sizeof(char));

/*	int i = 0;
	for (; ptr[i] != '\0'; ++i)
	{
		processs->name[i] = ptr[i];
	//	printf("%s \n", &ptr[i]);
	} 


	processs->name[i] = '\0';*/
	processs->name = ptr;

	
	ptr = strtok(NULL, "\t");
	processs->at = atoi(ptr);
	

	ptr = strtok(NULL, "\t");
	processs->bt = atoi(ptr);


	ptr = strtok(NULL, "\t");
	processs->v = atoi(ptr);
	
/*
	printf("%s ", (processs->name));
	printf("%d ",processs->at);
	printf("%d ",processs->bt);
	printf("%d \n",processs->v);
	
*/
	add_back(&cola, processs);
	free(copy);
	
}

void liberar_procesos(){
	for(int i=0; i<Nprocess; ++i)
		free((*(processes+i) )->name );
		
}
void cargar_procesos(){
	inicialisar_lista(&cola);

	FILE * fp = fopen("listProcess.cpu", "r");
	
	char * buffer = (char*)malloc(50 * sizeof(char));
	char ch = fgetc(fp);

	for(int i = 0; i<11; ++i) {
			char ch = fgetc(fp);
		//	printf("%s",&ch);
			}


	
	uint i=0;
	/*while ((ch=fgetc(fp)) != EOF){
		/* code *
		if(ch == '\n'){
			*(buffer+1)='\0';
			printf("%s", buffer);
		//	printf("%s",&ch);
		//	crear_proceso(buffer);
			*buffer = '\0';
			printf("%s", buffer);
			i=0;
			
		}else{
			*(buffer+1)=ch;
		//	printf("%s",&ch);
			i++;
		}


	}*/

	while (fscanf(fp, "%[^\n] ", buffer) != EOF){
	//	printf("%s \n", buffer);
		crear_proceso(buffer);
	}
	

	free(buffer);
	fclose(fp);

}
void reiniciar_procesos(){
	for (int i = 0; i < Nprocess; i++)
	{
		/* code */
		(*(processes+i))->workDone =0;
	}
	
}
int quantum(){
		int qua=0;
	scanf("%d", &qua);

	for (int i = 0; i < Nprocess; ++i)
	{
		(*(processes + i))->qm = qua;
	}
	return qua;
}

void rellenar_Arreglo_Proceso(){
	Nprocess = length(&cola);
	processes = malloc(Nprocess*sizeof( struct list));

	for (int i = 0; i < Nprocess; i++)
		*(processes+i)=pop(&cola);

	struct node * n ;
			for (int i = 0; i < Nprocess; ++i)
		{
			n = *(processes + i);
			printf("|%s\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t| \n",
				n->name, n->at, n->bt, n->et, n->wt, n->rt, n->tt);
			}
		//	printf("%s", separator);
		
}




void output(void *vargp) {
	printf("|-------|-------|-------|-------|-------|-------|-------|-------|-------| \n");
	printf("|PXX\t|QU\t|QM\t|AT\t|BT\t|ET\t|WT\t|RT\t|TT\t|\n");
	printf("|-------|-------|-------|-------|-------|-------|-------|-------|-------| \n\n");
} 

void FCFS(void *vargp) {
	/*
	AT=AT
	BT=BT
	ET= BT + anterior(ET)
	WT= anterior(ET)-AT
	RT=WT
	TT = ET-AT = BT+WT
	*/
	inicialisar_lista(&cola);
	printf(" --> FCFS\n");
	
	output(0);
} 

void PS(void *vargp) {
	/*
	max(V)=3
	PR=V
	AT=AT
	BT=BT
		if is the first Process
			ET= AT+BT
			WT=0
		else
            


		


	ET= ?
	WT= ?
	RT=WT
	TT = ET-AT = BT+WT
	*/
	printf(" --> PS\n");
	output(0);
} 

void RR(void *vargp) {
		/*
	QM=2
	AT=AT
	BT=BT

	ET= ?
	WT= ?

	RT=WT
	TT = ET-AT = BT+WT
	*/
	printf(" --> RR\n");
	output(0);
} 

void MLFQS(void *vargp) {
	printf(" --> MLFQS\n");
	output(0);
} 

int menu(){
	int tmp;
	printf("############\n1 - FCFS\n2 - PS\n3 - RR\n4 - MLFQS\n0 - EXIT\n\nSeleccione Opción: ");
	scanf("%d", &tmp);
	return tmp;
}

// C Concepts 
// https://gist.github.com/Integralist/6c35d4f26c0c7bf5bdce7b95cfa906b3
int main ( int argc , char * argv []) {
	int op = 9;
	cargar_procesos();
	rellenar_Arreglo_Proceso();
	pid_t pid;
	while (op > 0){
		op = menu();
		printf("Procesando opción: %d\n", op);
		if ( op >5 || op<=0 ){
			printf("### END ###\n");
			break;
		}
		printf("### BEGIN ###\n");
		switch(op){
			case 1:
			FCFS(0);
			break;
			case 2:
			PS(0);
			break;
			case 3:
			RR(0);
			break;
			default:
			case 4:
			MLFQS(0);
			break;
		}
	}
	// liberamos todos los MALLOCS
//	liberar_procesos();
	free(processes);
	return 0;
}

