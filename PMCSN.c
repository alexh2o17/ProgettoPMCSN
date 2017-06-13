#include "basic.h"


int main(int argc, char* argv[])
{
	/*if(argc != 0)
	{
		fprintf(stderr, "Usage of %s: no arguments\n", argv[0]);
		exit(EXIT_FAILURE);
	}*/

    srand((unsigned int) time(NULL)); //Ogni simulazione usa un seed differente
    PlantSeeds(rand());
    //PlantSeeds(123456789);
    struct {
	double node;
	double node_cloudlet;
	double node_cloud;
	double service;
	double service_cloudlet;
	double service_cloud;
	}area = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};


    int num_task1_cloud = 0; //Conta il numero di task 1 nel cloud
    int num_task2_cloud = 0; //Conta il numero di task 2 nel cloud
    int num_task1_cloudlet = 0; //Conta il numero di task 1 nella cloudlet
    int num_task2_cloudlet = 0; //Conta il numero di task 2 nella cloudlet
    int task_rimanenti = 0;     //Conta i task rimanenti nel sistema
    int task1_rim_cloud = 0;    //Conta i task 1 rimanenti sul cloud
    int task2_rim_cloud = 0;    //Conta i task 2 rimanenti sul cloud
    int task1_rim_cloudlet= 0;  //Conta i task 1 rimanenti sul cloudlet
    int task2_rim_cloudlet = 0; //Conta i task 2 rimanenti sul cloudlet
    int num_task_cloud = 0;     //Conta i task che hanno attraversato il cloud
    int num_task_cloudlet = 0;  //Conta i task che hanno attraversato la cloudlet

    
    //Indici relativi ai completamenti
    double num_task1_compl_cloud = 0; //Numero di task 1 completati sul cloud
    double num_task2_compl_cloud = 0; //Numero di task 2 completati sul cloud
    double num_task1_compl_cloudlet = 0; //Numero di task 1 completati sulla cloudlet
    double num_task2_compl_cloudlet = 0; //Numero di task 2 completati sulla cloudlet

    double t_current = 0; //Istante di accadimento dell'evento corrente 
    double t_next = 0;  //Istante di accadimento del prossimo evento
    double diff = 0;    //Differenza tra l'istante di accadimento del prossimo evento e di quello corrente
    double area_cloudlet = 0;
    double area_cloud = 0;
    
    //Istanti di completamento. Inizializzandoli a infinito si forza un arrivo come primo evento
    double task1_completion_cloud = INFINITO;
    double task2_completion_cloud = INFINITO;
    double task1_completion_cloudlet = INFINITO;
    double task2_completion_cloudlet = INFINITO;

    //Inizializzazione arrivi
    double task1_arrival = GetArrival1(); 
    double task2_arrival = GetArrival2();

    double percentuale_elab_task = 0; //Percentuale di job elaborato sulla cloudlet prima di essere spostato sul cloud 

    struct task *list_head_cloud_1 = NULL; //Lista task 1 sul cloud
    struct task *list_head_cloud_2 = NULL; //Lista task 2 sul cloud
    struct task *list_head_cloudlet_1 =  NULL; //Lista task 1 sulla cloudlet
    struct task *list_head_cloudlet_2 =  NULL; //LIsta task 2 sulla cloudlet

    double thrSistema;  //Throguput del Sistema
    double thrcloudlet; //Throughput della cloudlet
    double thrcloud;    //Throughput del cloud
    double t_risp_sis;  //Tempo di risposta medio del Sistema
    double t_risp_cloudlet; //Tempo di risposta medio della cloudlet
    double t_risp_cloud;    //Tempo di rispost medio del cloud
    double util_sistema; //Utilizzazione del sistema
    double util_cloudlet; //Utilizzazione cloudlet
    double util_cloud; //Utiizzazione cloud
    double t_old;
    
    FILE *file_Throughput = openFile("Throughput.txt", "a+");;
    FILE *file_T_Sistema = openFile("T_Sistema.txt", "a+");

    
    while(task_rimanenti > 0 || task1_arrival < STOP ||task2_arrival < STOP) //Il sistema elabora task finchè ne restano al suo interno e la simulazione non termina
    {
        if(task1_arrival >= STOP) //Scaduto il tempo della simulazione si forzano i completamenti dei task restanti nel sistema come unico evento possibile
        {
            task1_arrival = INFINITO;
        }

        if(task2_arrival >= STOP)
        {
            task2_arrival = INFINITO;
        }

        t_next = Min(task1_arrival, task2_arrival, task1_completion_cloud, task2_completion_cloud, task1_completion_cloudlet, task2_completion_cloudlet); //Devo capire qual � il prossimo evento
        diff = t_next - t_current;
        if(task_rimanenti > 0)
        {
        	area.node += diff * task_rimanenti;

        	area.service += diff;
        	if(task1_rim_cloudlet + task2_rim_cloudlet > 0)
        	{
        		area.service_cloudlet += diff;
        		area.node_cloudlet += diff * (task1_rim_cloudlet + task2_rim_cloudlet);
        	}
        	if(task1_rim_cloud + task2_rim_cloud > 0)
        	{
        		area.service_cloud += diff;
        		area.node_cloud += diff * (task1_rim_cloud + task2_rim_cloud);
        	}
        }
        t_current = t_next; //Il prossimo evento diventa quello da gestire
        
        if(t_current == task1_arrival) //Arriva un task 1
        {
            struct task *t1 = alloc_task(); //Viene allocata e inizializzata la struttura rappresentante il task
            t1->perc_rim = 1; 
            t1->arrival = task1_arrival;
            if(task1_rim_cloudlet == N) //Il task 1 va sul cloud
            {
                t1->completion = t1->arrival + GetService1_cloud();; //Viene assegnato l'istante di completamento al task
                if(task1_completion_cloud > t1->completion) //Il task 1 appena entrato nel cloud è quello di più prossimo completamento
                {
                    task1_completion_cloud = t1->completion;
                }
                insert_sorted_list(t1, &list_head_cloud_1); //Aggiungo il task alla lista
                num_task1_cloud++; //Viene incrementato il numero di task 1 che hanno attraversato il cloud
                task1_rim_cloud++; //Viene incrementato il numero di task 1 rimanenti nel cloud
            }
            else if((task1_rim_cloudlet + task2_rim_cloudlet) < S) //Il task 1 viene accettato sulla cloudlet
            {
                t1->completion = t1->arrival + GetService1_cloudlet();
                if(task1_completion_cloudlet > t1->completion)
                {
                    task1_completion_cloudlet = t1->completion;
                }
                insert_sorted_list(t1, &list_head_cloudlet_1); //Aggiungo il task alla lista
                num_task1_cloudlet++; //Viene incrementato il numero di task 1 che hanno attraversato la cloudlet
                task1_rim_cloudlet++; //Viene incrementato il numero di task 1 rimanenti nella cloudlet
            }
            else if(task2_rim_cloudlet > 0) //Il task 1 viene accettato sulla cloudlet e un task 2 viene mandato sul cloud
            {
                t1->completion = t1->arrival + GetService1_cloudlet();
                if(task1_completion_cloudlet > t1->completion)
                {
                    task1_completion_cloudlet = t1->completion;
                }
                struct task *p;
                p = find_tail(&list_head_cloudlet_2); //Cerco nella lista il task 2, presente sulla cloudlet, con istante di completamento maggiore 
                if(task2_rim_cloudlet == 1)
                {
                    task2_completion_cloudlet = INFINITO; //Se dopo la rimozione del task dalla cloudlet rimangono 0 task 2 allora viene forzata l'impossibilità di completamento di un task 2 sulla cloudlet
                } 

                percentuale_elab_task = (t_current - p->arrival)/(p->completion - p->arrival); 
                num_task2_compl_cloudlet += percentuale_elab_task; //In questo caso l'incremento non è unitario visto che il task non viene elaborato completamente sulla cloudlet
                p->perc_rim = 1 - percentuale_elab_task;
                p->completion = t_current + (GetService2_cloud()*p->perc_rim) + SETUP;
                if(task2_completion_cloud > p->completion)
                {
                    task2_completion_cloud = p->completion;
                }
                p->arrival = t_current; //Istante di arrivo del task 2 sul cloud

                struct task *new = alloc_task(); //Viene allocata la struttura per il task trasferito sul cloud
                new->arrival = p->arrival;
                new->completion = p->completion;
                new->perc_rim = p->perc_rim;

                //I task vengono inseriti nelle opportune liste
                insert_sorted_list(t1, &list_head_cloudlet_1);
                insert_sorted_list(new, &list_head_cloud_2);
                remove_central_task(p,&list_head_cloudlet_2);

                num_task1_cloudlet++; //Viene incrementato il numero di task 1 che hanno attraversato la cloudlet
                num_task2_cloud++; //Viene incrementato il numero di task 2 che hanno attraversato il cloud
                task1_rim_cloudlet++; //Viene incrementato il numero di task 1 rimanenti sulla cloudlet
                task2_rim_cloud++; //Viene incrementato il numero di task 2 rimanenti sul cloud
                task2_rim_cloudlet--; //Viene decrementato il numero di task 2 rimanenti sulla cloudlet
            }
            else //Il task 1 viene accettato sulla cloudlet
            {
                t1->completion = t1->arrival + GetService1_cloudlet();
                if(task1_completion_cloudlet > t1->completion)
                {
                    task1_completion_cloudlet= t1->completion;
                }
                insert_sorted_list(t1, &list_head_cloudlet_1); 
                task1_rim_cloudlet++; //Viene incrementato il numero di task 1 rimanenti sulla cloudlet
            }
            task1_arrival = GetArrival1(); //Viene impostato l'istante di arrivo del prossimo task 1
        }

        else if(t_current == task2_arrival) //Arriva un task 2
        {
            struct task *t2 = alloc_task(); //Viene allocata e inizializzata la struttura rappresentante il task
            t2->perc_rim = 1;
            t2->arrival= task2_arrival;

            if(task1_rim_cloudlet + task2_rim_cloudlet >= S) //Il task 2 viene mandato sul cloud
            {
                t2->completion = t2->arrival + GetService2_cloud();
                if(task2_completion_cloud > t2->completion)
                {
                    task2_completion_cloud= t2->completion;
                }
                insert_sorted_list(t2, &list_head_cloud_2); 
                num_task2_cloud++; //Viene incrementato il numero di task 2 che hanno attraversato il cloud
                task2_rim_cloud++; //Viene incrementato il numero di task 2 rimanenti sul cloud
            }
            else //Il task 2 viene accettato sulla cloudlet
            {
                t2->completion = t2->arrival + GetService2_cloudlet();
                if(task2_completion_cloudlet == INFINITO || task2_completion_cloudlet > t2->completion)
                {
                    task2_completion_cloudlet= t2->completion;
                }
                insert_sorted_list(t2, &list_head_cloudlet_2); //Aggiungo il task 2 alla lista
                num_task2_cloudlet++; //Viene incrementato il numero di task 2 che hanno attraversato il cloud
                task2_rim_cloudlet++; //Viene incrementato il numero di task 2 rimanenti sul cloud

            }
            task2_arrival = GetArrival2(); //Viene impostato l'istante di arrivo del prossimo task 2
        }
        else if(t_current == task1_completion_cloudlet) //Viene completato un task 1 sulla cloudlet
        {
            num_task1_compl_cloudlet++; //Viene incrementato il numero di task 1 completati sulla cloudlet
            task1_rim_cloudlet--; //Viene decrementato il numero di task 1 rimanenti sulla cloudlet
            if(list_head_cloudlet_1->next_task == NULL)
            {

                task1_completion_cloudlet = INFINITO; //Se il numero di task 1 rimanenti sulla cloudlet è 0 allora si forza l'impossibiltà del completamento degli stessi sulla cloudlet
            }
            else
            {
                task1_completion_cloudlet = list_head_cloudlet_1->next_task->completion;
            }
            remove_first_task(&list_head_cloudlet_1);
        }
        else if(t_current == task1_completion_cloud) //Il task 1 viene completato sul cloud
        {
            num_task1_compl_cloud++; //Viene incrementato il numero di task 1 completati sul cloud
            task1_rim_cloud--; //Viene decrementato il numero di task 1 rimanenti sul cloud
            if(list_head_cloud_1->next_task == NULL)
            {
                task1_completion_cloud = INFINITO;
                t_old=t_current;
            }
            else
            {
                task1_completion_cloud = list_head_cloud_1->next_task->completion;
            }
            remove_first_task(&list_head_cloud_1);
        }
        else if(t_current == task2_completion_cloudlet) //Il task 2 viene completato sulla cloudlet
        {
            num_task2_compl_cloudlet++;
            task2_rim_cloudlet--;

            if(list_head_cloudlet_2->next_task == NULL)
            {
                task2_completion_cloudlet = INFINITO;
            }
            else
            {
                task2_completion_cloudlet=list_head_cloudlet_2->next_task->completion;
            }
            remove_first_task(&list_head_cloudlet_2);
        }
        else if(t_current == task2_completion_cloud) //Il task 2 viene completato sul cloud
        {
            num_task2_compl_cloud += list_head_cloud_2->perc_rim;
            task2_rim_cloud--;
            if(list_head_cloud_2->next_task == NULL)
            {
                task2_completion_cloud=INFINITO;
                t_old=t_current;
            }
            else
            {
                task2_completion_cloud=list_head_cloud_2->next_task->completion;
            }
            remove_first_task(&list_head_cloud_2);
        }
        task_rimanenti = task1_rim_cloudlet + task1_rim_cloud + task2_rim_cloudlet + task2_rim_cloud;
    }
    num_task_cloud = num_task1_cloud + num_task2_cloud; //Numero di task 1 e 2 che hanno attraversato il cloud
    num_task_cloudlet = num_task1_cloudlet + num_task2_cloudlet; //Numero di task 1 e 2 che hanno attraversato la cloudlet
    
    thrSistema = (num_task1_compl_cloud + num_task2_compl_cloud + num_task1_compl_cloudlet + num_task2_compl_cloudlet)/t_current;
    thrcloudlet = (num_task1_compl_cloudlet + num_task2_compl_cloudlet)/t_current;
    thrcloud = (num_task1_compl_cloud + num_task2_compl_cloud)/t_current;
    printf("Throughput: %f\n", thrSistema);
    printf("Throughput cloudlet: %f\n", thrcloudlet);
    printf("Throughput cloud: %f\n", thrcloud);
    
    double P1 = ((double) num_task_cloud/(num_task_cloud + num_task_cloudlet)); //Probabilità che un task vada sul cloud 	
    double P2 = ((double) num_task_cloudlet/(num_task_cloud + num_task_cloudlet)); //Probabilità che un task vada sulla cloudlet

    if((area_cloudlet/t_current) > 20)
    {
        printf("\n\n\n non va bene \n\n\n");
    }
	printf("Numero medio task nel sistema: %f\n", area.node/t_current);
	printf("Numero medio task nel cloud: %f\n", area.node_cloud/t_current);
	printf("Numero medio task nella cloudlet: %f\n", area.node_cloudlet/t_current);

    t_risp_sis = (area.node/t_current)/thrSistema;
    t_risp_cloudlet = (area.node_cloudlet/t_current)/thrcloudlet;
    t_risp_cloud = (area.node_cloud/t_current)/thrcloud;
	printf("Tempo di risposta del sistema: %f\n", t_risp_sis);
	printf("Tempo di risposta della cloudlet: %f\n", t_risp_cloudlet);
	printf("Tempo di risposta del cloud: %f\n", t_risp_cloud);

	printf("num_task_cloud: %d\n", num_task_cloud);
	printf("T_current: %f\n", t_current);
	util_sistema = (area.service/t_current);
	util_cloud = (area.service_cloud/t_current);
	util_cloudlet = (area.service_cloudlet/t_current);
	printf("Utilizzazione del sistema: %f\n", util_sistema);
	printf("Utilizzazione del cloud: %f\n", util_cloud);
	printf("Utilizzazione della cloudlet: %f\n", util_cloudlet);

	printf("\n\n**********Nuova esecuzione**********\n");

    fprintf(file_Throughput, "%f\n", thrSistema);
    fprintf(file_T_Sistema, "%f\n", t_risp_sis);
    fclose(file_Throughput);
    fclose(file_T_Sistema);
}