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

/*	struct node * n ;
			for (int i = 0; i < Nprocess; ++i)
		{
			n = *(processes + i);
			printf("|%s\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t| \n",
				n->name, n->at, n->bt, n->et, n->wt, n->rt, n->tt);
			}*/
		
}




void output(void *vargp) {
	printf("|-------|-------|-------|-------|-------|-------|-------|-------|-------| \n");
	printf("|PXX\t|QU\t|QM\t|AT\t|BT\t|ET\t|WT\t|RT\t|TT\t|\n");
	printf("|-------|-------|-------|-------|-------|-------|-------|-------|-------| \n\n");
} 

void fila_fsfc(struct list *queue, struct node ** array, int length){
	for (int i = 0; i < length; ++i)
	{
		add_back(queue, *(array + i));
	}
}

void fcfs_pepare(struct list *l){
	int at = 0;
	struct node * original = &l->head;
	struct node * n;
	uint kernel_time = 0;

	int numberProcesses = length(l);

	int dispatched = 0;

	for (int at = 0; dispatched != numberProcesses; ++at)
	{
		n = original;
		while((n = n->next) != NULL){
			if (n->at == at)
			{
				kernel_time = kernel_time + n->bt;
				n->et = kernel_time;
				n->wt = kernel_time - n->at - n->bt;
				n->rt = n->wt;
				n->tt = n->et - at;
				++dispatched;
			}
			else
			{
				continue;
			}
		}
	}
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
	//***********************
	fila_fsfc(&cola, processes, Nprocess);
	fcfs_pepare(&cola);
	//***********************
	printf(" --> FCFS\n");
	printf("|-------|-------|-------|-------|-------|-------|-------| \n");
	printf("|PXX\t|AT\t|BT\t|ET\t|WT\t|RT\t|TT\t|\n" );
	printf("|-------|-------|-------|-------|-------|-------|-------| \n");
		struct node * n ;
			for (int i = 0; i < Nprocess; ++i)
		{
			n = *(processes + i);
			printf("|P%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t| \n",
				i, n->at, n->bt, n->et, n->wt, n->rt, n->tt);
			}
			printf("|-------|-------|-------|-------|-------|-------|-------| \n");
	


} 
void insert(struct list * pq, struct node * newnode, struct node * reference){
	newnode->next = reference;
	newnode->prev = reference->prev;

	if (reference->prev == NULL)
	{
		pq->head.next = newnode;
	}
	else
	{
		reference->prev->next = newnode;
	}

	reference->prev = newnode;

}
void agregarPrioridad(struct list *pq, struct node *n){
	
	struct node * var1 = &pq->head;
	if (pq->head.next == NULL)
	{
		add_back(pq, n);
	}
	else
	{	
		while((var1 = var1->next) != NULL){
			if (var1->v > n->v)
			{

			if (var1)
				insert(pq, n, var1);
			else
				add_back(pq, n);
				break;
			}
			else
			{
				if (var1->next == NULL)
				{
					add_back(pq, n);
							
					break;
				}
				else
				{
					continue;
				}
			}
		}
	}	
}

void creandoPs(struct list * queue, struct node ** array, int length){
	uint kernel_time = 0;
	uint executing = 0;
	struct node * inExecution;
	uint estimated_finish_time = 0;

	int dispatched = 0;

	for (; dispatched != length; ++kernel_time)
	{
		for (int i = 0; i < length; ++i)
		{
			if ((*(array + i))->at == kernel_time)
			{
				
				agregarPrioridad(queue, *(array + i));
			}
			else
			{
				continue;
			}
		}
		

		if (executing && estimated_finish_time == kernel_time)
		{
			executing = 0;
			dispatched++;
			inExecution = NULL;
		}

		if (executing == 0)
		{
			if (queue->head.next != NULL)
			{
				inExecution = pop(queue);
				executing = 1;
				estimated_finish_time = kernel_time + inExecution->bt;
				
				inExecution->et = estimated_finish_time;
				inExecution->wt = estimated_finish_time - inExecution->at - inExecution->bt;
				inExecution->rt = inExecution->wt;
				inExecution->tt = inExecution->et - inExecution->at;
			}
		}			
	}	
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
	//output(0);
	inicialisar_lista(&cola);
	//***********************
	creandoPs(&cola, processes, Nprocess);
	//***********************
	printf(" --> PS\n");
	printf("|-------|-------|-------|-------|-------|-------|-------|-------| \n");
	printf("|PXX\t|AT\t|BT\t|PR\t|ET\t|WT\t|RT\t|TT\t|\n" );
	printf("|-------|-------|-------|-------|-------|-------|-------|-------| \n");
		struct node * n ;
			for (int i = 0; i < Nprocess; ++i)
		{
			n = *(processes + i);
			printf("|P%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t| \n",
				i, n->at, n->bt, n->v, n->et, n->wt, n->rt, n->tt);
			}
			printf("|-------|-------|-------|-------|-------|-------|-------|-------| \n");


} 

void crearRR(struct list * queue, struct node ** array, int length, int quantum){
	char str[5];
	uint kernel_time = 0;
	uint executing = 0;
	struct node * inExecution;

	uint qm_counter = 0;

	int dispatched = 0;

	char * str1 = "No process in execution";

	for (; dispatched != length; ++kernel_time)
	{
		//printf("At kernel time: %d\n", kernel_time);
		for (int i = 0; i < length; ++i)
		{
			if ((*(array + i))->at == kernel_time)
			{
				add_back(queue, *(array + i));
			}
			else
			{
				continue;
			}
		}
		//printf("Processes in Queue: \n");
		//print_list(queue);
		

		if (executing)
		{
			inExecution->workDone++;
			qm_counter++;
			if (inExecution->workDone == inExecution->bt)
			{
				//printf("Termino un proceso\n");
				executing = 0;
				dispatched++;
				
				qm_counter = 0;

				/* Setting Process Properties */
				inExecution->et = kernel_time;
				inExecution->wt = kernel_time - inExecution->at - inExecution->bt;
				//inExecution->rt = inExecution->wt;
				inExecution->tt = inExecution->et - inExecution->at;
				inExecution = NULL;
			}

			if (qm_counter == quantum)
			{
				//printf("/*Context Switch*/\n");
				qm_counter = 0;
				executing = 0;
				if (inExecution != NULL)
				{
					add_back(queue, inExecution);
					inExecution = NULL;
				}
				/* Switch context */

			}
		}

		if (executing == 0)
		{
			if (queue->head.next != NULL)
			{
				inExecution = pop(queue);
				if (inExecution->workDone == 0)
				{
					inExecution->rt = kernel_time - inExecution->at;
				}
				executing = 1;

			}
		}			


	}
}

void RR(void *vargp) {
		/*
	QM=?
	AT=AT
	BT=BT

	ET= ?
	WT= ?

	RT=WT
	TT = ET-AT = BT+WT
	*/
	printf(" --> RR\n");
	printf("Quantum:");
	int qua= quantum();
	reiniciar_procesos();
	crearRR(&cola, processes, Nprocess, qua);
	printf("|-------|-------|-------|-------|-------|-------|-------|-------|-------| \n");
	printf("|PXX\t|QU\t|QM\t|AT\t|BT\t|ET\t|WT\t|RT\t|TT\t|\n" );
	printf("|-------|-------|-------|-------|-------|-------|-------|-------|-------| \n");
		struct node * n ;
			for (int i = 0; i < Nprocess; ++i)
		{
			n = *(processes + i);
			printf("|P%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t| \n",
				i, n->qu, n->qm, n->at, n->bt, n->et, n->wt, n->rt, n->tt);
			}
			printf("|-------|-------|-------|-------|-------|-------|-------|-------|-------| \n");

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

