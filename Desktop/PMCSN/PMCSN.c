#include "basic.h"

int main()
{
    srand((unsigned int) time(NULL));
    PlantSeeds(rand());

    int num_task1_cloud = 0; //Conta il numero di task di tipo 1 nel cloud
    int num_task2_cloud = 0; //Conta il numero di task di tipo 2 nel cloud
    int num_task1_cloudlet = 0; //Conta il numero di task di tipo 1 nella cloudlet
    int num_task2_cloudlet = 0; //Conta il numero di task di tipo 2 nella cloudlet
    int task_rimanenti = 0;     //Conta i task rimanenti nel sistema
    int task1_rim_cloud = 0;    //Conta i task di tipo 1 rimanenti sul Cloud
    int task2_rim_cloud = 0;    //Conta i task di tipo 2 rimanenti sul Cloud
    int task1_rim_cloudlet= 0;  //Conta i task di tipo 1 rimanenti sul Cloudlet
    int task2_rim_cloudlet = 0; //Conta i task di tipo 2 rimanenti sul Cloudlet
    int num_task_cloud = 0;     //Conta i task che hanno attraversato il Cloud
    int num_task_cloudlet = 0;  //Conta i task che hanno attraversato la Cloudlet

    //Indici relativi ai completamenti
    double num_task1_compl_cloud = 0; //Numero di task 1 completati sul cloud
    double num_task2_compl_cloud = 0; //Numero di task 2 completati sul cloud
    double num_task1_compl_cloudlet = 0; //Numero di task 1 completati sulla cloudlet
    double num_task2_compl_cloudlet = 0; //Numero di task 2 completati sulla cloudlet

    double t_current=0; //Istante di accadimento dell'evento corrente (da gestire)
    double t_next = 0;  //Istante di accadimento dell'evento prossimo
    double diff = 0;    //Differenza tra l'istante prossimo e quello corrente
    double area_cloudlet =0;
    double area_cloud=0;
    double area_tot=0;

    //Istanti di completamento. Inizializzando a infinito forzo il fatto che il primo evento sia un arrivo
    double task1_completion_cloud = INFINITO;
    double task2_completion_cloud = INFINITO;
    double task1_completion_cloudlet = INFINITO;
    double task2_completion_cloudlet = INFINITO;

    //Inizializzo gli arrivi per il motivo di cui sopra
    double task1_arrival = GetArrival1(); //tempo di arrivo per il primo job di classe1
    double task2_arrival = GetArrival2(); //tempo di arrivo per il primo job di classe2

    struct task *list_head_cloud_1 = NULL; //Lista task 1 sul Cloud
    struct task *list_head_cloud_2 = NULL; //Lista task 2 sul Cloud
    struct task *list_head_cloudlet_1 =  NULL; //Lista task 1 sulla Cloudlet
    struct task *list_head_cloudlet_2 =  NULL; //LIsta task 2 sulla Cloudlet


//    struct task *next_compl_cloud_1 = alloc_task(); //Prossimo task 1 sul cloud
//    struct task *next_compl_cloud_2 = alloc_task(); //Prossimo task 2 sul cloud
//    struct task *next_compl_cloudlet_1 = alloc_task(); //Prossimo task 1 sulla cloudlet
//    struct task *next_compl_cloudlet_2 = alloc_task(); //Prossimo task 2 sulla cloudlet

    double thrSistema;  //throguput del Sistema
    double thrCloudlet; //Througput della Cloudlet
    double thrCloud;    //Througput del Cloud
    double t_Risp_Sis;  //Tempo di risposta medio del Sistema
    double t_Risp_Cloudlet; //Tempo di risposta medio della Cloudlet
    double t_Risp_Cloud;    //Tempo di rispost medio del Cloud
    double t_old;


    double percentuale_elab_task = 0; //Percentuale di job elaborato sulla cloudlet prima di essere spostato sul cloud

    while(task_rimanenti > 0 || task1_arrival < STOP ||task2_arrival < STOP) //Ogni iterazione ha la stessa durata. Accetto task in ingresso finch� � possibile
    {
        if(task1_arrival >= STOP)
        {
            task1_arrival = INFINITO;
        }
        if(task2_arrival >= STOP)
        {
            task2_arrival = INFINITO;
        }

        t_next = Min(task1_arrival, task2_arrival, task1_completion_cloud, task2_completion_cloud, task1_completion_cloudlet, task2_completion_cloudlet); //Devo capire qual � il prossimo evento

        diff= t_next-t_current;
        t_current= t_next;
        area_tot += diff*task_rimanenti;
        area_cloud += diff*(task1_rim_cloud+task2_rim_cloud);
        area_cloudlet += diff*(task1_rim_cloudlet+task2_rim_cloudlet);

        if(t_current == task1_arrival) //Arriva un task di tipo 1
        {
            struct task *t1 = alloc_task();
            t1->perc_rim = 1; //Non posso inizializzare il campo della struttura nella struttura stessa
            t1->arrival = task1_arrival;
//                    printf("\ntask1 arrival\n");

            if(task1_rim_cloudlet == N) //Manda il task 1 sul cloud
            {
                //printf("%d\n",task1_rim_cloud+task2_rim_cloud);
//                area_cloud += diff*(task1_rim_cloud+task2_rim_cloud);

                t1->completion = t1->arrival + GetService1_cloud();
                if(task1_completion_cloud == INFINITO || task1_completion_cloud > t1->completion)
                {
                    task1_completion_cloud = t1->completion;
                }
                insert_sorted_list(t1, &list_head_cloud_1); //Aggiungo il task alla lista
//                        printf("task1 nel cloud\n");
                num_task1_cloud++; //Incremento il numero di task 1 che hanno attraversato il cloud
                task1_rim_cloud++; //Task 1 rimanenti sul cloud
            }
            else if((task1_rim_cloudlet + task2_rim_cloudlet) < S) //Il task 1 viene accettato sulla cloudlet
            {
                t1->completion = t1->arrival + GetService1_cloudlet();
//                area_cloudlet += diff*(task1_rim_cloudlet+task2_rim_cloudlet);
                if(task1_completion_cloudlet == INFINITO || task1_completion_cloudlet > t1->completion)
                {
                    task1_completion_cloudlet = t1->completion;
                }
                insert_sorted_list(t1, &list_head_cloudlet_1); //Aggiungo il task alla lista
                num_task1_cloudlet++; //Incremento il numero di task1 che hanno attraversato la cloudlet
                task1_rim_cloudlet++; //Task 1 rimanenti sulla cloudlet
            }
            else if(task2_rim_cloudlet > 0) //Accetta il task 1 sulla cloudlet e manda un task 2 sul cloud
            {
//                area_cloudlet += diff*(task1_rim_cloudlet+task2_rim_cloudlet);
//              area_cloud += diff*(task1_rim_cloud+task2_rim_cloud);
//                printf("%d\n",task1_rim_cloud+task2_rim_cloud);

                t1->completion = t1->arrival + GetService1_cloudlet();
                if(task1_completion_cloudlet == INFINITO || task1_completion_cloudlet > t1->completion)
                {
                    task1_completion_cloudlet = t1->completion;
                }
                struct task *p;

                p=find_tail(&list_head_cloudlet_2);
//                        printf("arrival: %f\n", p->arrival);
//                        fflush(stdout);
                if(task2_rim_cloudlet == 1){
                    task2_completion_cloudlet=INFINITO;
                }


                percentuale_elab_task = (t_current - p->arrival)/(p->completion - p->arrival); //Tempo per cui il task � stato gi� elaborato diviso il tempo di servizio
                if(percentuale_elab_task>1){
                    printf("peleb: %f\n", percentuale_elab_task);
                    printf("current: %f\n",t_current);
                    printf("arrival: %f\n",p->arrival);
                    printf("completion: %f\n",p->completion);
                }

                num_task2_compl_cloudlet += percentuale_elab_task;
                p->perc_rim = 1 - percentuale_elab_task;
                p->completion = t_current + (GetService2_cloud()*p->perc_rim) + SETUP;
                if(task2_completion_cloud == INFINITO || task2_completion_cloud > p->completion)
                {
                    task2_completion_cloud = p->completion;
                }
                p->arrival = t_current; //Tempo di arrivo sul cloud


                struct task *new = alloc_task();

                new->arrival=p->arrival;
                new->completion=p->completion;
                new->perc_rim = p->perc_rim;

                insert_sorted_list(t1, &list_head_cloudlet_1);
                insert_sorted_list(new, &list_head_cloud_2);
                remove_central_task(p,&list_head_cloudlet_2);
//
//                        if(list_head_cloudlet_2==NULL){
//                            task2_completion_cloudlet=INFINITO;
//                        }

                num_task1_cloudlet++;
                num_task2_cloud++;
                task1_rim_cloudlet++; //Incremento il numero di task 1 rimanente alla cloudlet
                task2_rim_cloud++; //Incremento il numero di task 2 rimanenti sul cloud
                task2_rim_cloudlet--; // Decremeto il numero di task2 rimanenti sulla clet
            }
            else //Accetta il task 1 sulla cloudlet
            {
//                area_cloudlet += diff*(task1_rim_cloudlet+task2_rim_cloudlet);
                t1->completion = t1->arrival + GetService1_cloudlet();
                if(task1_completion_cloudlet == INFINITO || task1_completion_cloudlet > t1->completion)
                {
                    task1_completion_cloudlet= t1->completion;
                }
                insert_sorted_list(t1, &list_head_cloudlet_1); //Aggiungo il task alla lista
                num_task1_cloudlet++;
                task1_rim_cloudlet++;
            }
//                    printf("task1 completion: %f\n", t1->completion);
            task1_arrival = GetArrival1(); //Se si verifica l'arrivo di un task di tipo 1 tale evento va ricreato
//                    printf("\n\ntask1_arrival: %f\n\n",task1_arrival);
        }

        else if(t_current == task2_arrival) //Arriva un task di tipo 2
        {
            struct task *t2 = alloc_task();
            t2->perc_rim = 1;
            t2->arrival= task2_arrival;

            if(task1_rim_cloudlet + task2_rim_cloudlet >= S)//Il task 2 viene mandato sul cloud
            {
//                printf("%d\n",task1_rim_cloud+task2_rim_cloud);
//                area_cloud += diff*(task1_rim_cloud+task2_rim_cloud);
//                        printf("cloud\n");
                t2->completion = t2->arrival + GetService2_cloud();
                if(task2_completion_cloud == INFINITO   || task2_completion_cloud > t2->completion)
                {
                    task2_completion_cloud= t2->completion;
                }
//                          printf("Task2 sul cloud\n");
//                        printf(" %f\n", task2_completion_cloud);
                insert_sorted_list(t2, &list_head_cloud_2); //Aggiungo il task alla lista
                num_task2_cloud++; //Manda il task2 sul cloud
                task2_rim_cloud++;
            }
            else //Il task 2 viene accettato sulla cloudlet
            {
//                        printf("cloudlet\n");
//                area_cloudlet += diff*(task1_rim_cloudlet+task2_rim_cloudlet);
                t2->completion = t2->arrival + GetService2_cloudlet();
                if(task2_completion_cloudlet == INFINITO || task2_completion_cloudlet > t2->completion)
                {
                    task2_completion_cloudlet= t2->completion;
                }
//                        printf(" %f\n", task2_completion_cloudlet);
                insert_sorted_list(t2, &list_head_cloudlet_2); //Aggiungo il task alla lista
                num_task2_cloudlet++; //Accetta il task2 sulla cloudlet
                task2_rim_cloudlet++;

            }
//                    printf("task2 completion: %f\n", t2->completion);
            task2_arrival = GetArrival2();
//                    printf("\n\ntask2_arrival: %f\n\n",task2_arrival);

        }

        else if(t_current == task1_completion_cloudlet) //Il task 1 viene completato sulla cloudlet
        {
//                    printf("task1 cloudlet \n");
//            area_cloudlet += diff*(task1_rim_cloudlet+task2_rim_cloudlet);
            num_task1_compl_cloudlet++;

            task1_rim_cloudlet--;
            if(list_head_cloudlet_1->next_task == NULL){
//                        printf("rim %i\n",task1_rim_cloudlet);
                task1_completion_cloudlet=INFINITO;
            }else{
                task1_completion_cloudlet=list_head_cloudlet_1->next_task->completion;
            }
            remove_first_task(&list_head_cloudlet_1);

        }
        else if(t_current == task1_completion_cloud) //Il task 1 viene completato sul cloud
        {
//            printf("%d\n",task1_rim_cloud+task2_rim_cloud);
//            area_cloud += diff*(task1_rim_cloud+task2_rim_cloud);
//                    printf("task1 cloud\n");
            num_task1_compl_cloud++;
            task1_rim_cloud--;
            if(list_head_cloud_1->next_task == NULL){
                task1_completion_cloud=INFINITO;
                t_old=t_current;
            }else{
                task1_completion_cloud=list_head_cloud_1->next_task->completion;
            }
            remove_first_task(&list_head_cloud_1);
        }
        else if(t_current == task2_completion_cloudlet) //Il task 2 viene completato sulla cloudlet
        {
//                        printf("task2 cloudlet\n");
//            area_cloudlet += diff*(task1_rim_cloudlet+task2_rim_cloudlet);
            num_task2_compl_cloudlet++;
            task2_rim_cloudlet--;

            if(list_head_cloudlet_2->next_task == NULL) {
                task2_completion_cloudlet = INFINITO;
            }else{
                task2_completion_cloudlet=list_head_cloudlet_2->next_task->completion;
            }
            remove_first_task(&list_head_cloudlet_2);
        }
        else if(t_current == task2_completion_cloud) //Il task 2 viene completato sul cloud
        {
//            printf("%d\n",task1_rim_cloud+task2_rim_cloud);
//            area_cloud += diff*(task1_rim_cloud+task2_rim_cloud);
//                    printf("task2 cloud\n");
            num_task2_compl_cloud += list_head_cloud_2->perc_rim;
            task2_rim_cloud--;
            if(list_head_cloud_2->next_task == NULL){
                task2_completion_cloud=INFINITO;
                t_old=t_current;
            }else{
                task2_completion_cloud=list_head_cloud_2->next_task->completion;
            }
            remove_first_task(&list_head_cloud_2);
        }
        task_rimanenti= task1_rim_cloudlet+task1_rim_cloud+task2_rim_cloudlet+task2_rim_cloud;//controllare bene
        thrSistema = (num_task1_compl_cloud + num_task2_compl_cloud + num_task1_compl_cloudlet + num_task2_compl_cloudlet)/t_current;
        printf("%f\n",thrSistema);
    }
    printf("task rimanenti: %d\n",task_rimanenti);
    //Task che hanno attraversato il colud e la cloudlet
    num_task_cloud = num_task1_cloud + num_task2_cloud;
    num_task_cloudlet = num_task1_cloudlet + num_task2_cloudlet;
    thrSistema = (num_task1_compl_cloud + num_task2_compl_cloud + num_task1_compl_cloudlet + num_task2_compl_cloudlet)/t_current;
    printf("Throughput: %f\n", thrSistema);
    printf("Num task cloud: %d\n", num_task1_cloud);
    printf("Num task cloudlet: %d\n", num_task_cloudlet);
    printf("tempo finale: %f\n", t_current);
//        double response_time=t_current/(num_task1_compl_cloud + num_task2_compl_cloud + num_task1_compl_cloudlet + num_task2_compl_cloudlet);
//        printf("tempo di risposta: %f", response_time);
    thrCloudlet=(num_task1_compl_cloudlet+num_task2_compl_cloudlet)/t_current;
//        printf("througCLet: %f\n", thrClet);
//        double P1=  ((double)num_task1_cloudlet/num_task_cloudlet); //Probabilità che arrivi un job di tipo 1 in clet
//        double P2= ((double) num_task2_cloudlet/num_task_cloudlet); //Probabilità che arrivi un job di tipo 2 in clet
//        double Px= ((double) num_task_cloudlet/(num_task_cloudlet+num_task_cloud));
//        double ro1= (Px*P1*3.25)/0.45;
//        double ro2= (Px*P1*6.25)/0.30;
//        double n1=ro1/(1-ro1);
//        double n2=ro2/(1-ro2);
//        double nx=n1+n2;
//        double trispclet= 20/thrClet;
//        double trispclet1= P1*2.2222+P2*3.3333;
//        printf("risposta: %f\n rispostainP: %f\n", trispclet,trispclet1);
//        printf("P1: %f\n P2: %d\n", n1,num_task_cloudlet);
//
    double P1= ((double) num_task_cloud/(num_task_cloud+num_task_cloudlet));
    double P2= ((double) num_task_cloudlet/(num_task_cloud+num_task_cloudlet));

    printf("P1: %f\n", P1);
    printf("P2: %f\n", P2);
    printf("# job medi clet: %f\n", area_cloudlet/t_current);
    if((area_cloudlet/t_current)>20){
        printf("\n\n\n non va bene \n\n\n");
    }
    printf("# job medi cloud: %f\n", area_cloud/t_current);
    printf("# job medi Sistema: %f\n", area_tot/t_current);

//       printf("# job medi tot: %f\n", area_tot/t_current);
    thrCloud=(num_task1_compl_cloud+num_task2_compl_cloud)/t_current;
    t_Risp_Sis= (area_tot/t_current)/thrSistema;
    t_Risp_Cloudlet=(area_cloudlet/t_current)/thrCloudlet;
    t_Risp_Cloud=(area_cloud/t_current)/thrCloud;
    printf("Tempo di risposta: %f\n\n", t_Risp_Sis);
    printf("Tempo di risposta della Cloudlet: %f\n",t_Risp_Cloudlet);
    printf("Tempo di risposta della Cloud: %f\n",t_Risp_Cloud);
}