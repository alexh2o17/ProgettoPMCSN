#include "basic.h"

struct task *alloc_task(void)
{
    struct task *p;

    p = malloc(sizeof(struct task));

    if(p == NULL)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    p->next_task= NULL;
    return p;
}

void free_task(struct task *p)
{
        free(p);
}

void insert_after_task(struct task *new, struct task **pnext)
{
    new->next_task = *pnext;
    *pnext = new;
}

void remove_first_task(struct task**ppos)
{
    struct task *d = *ppos;
    *ppos = d-> next_task;
}

void insert_sorted_list( struct task *new, struct task **pnext)
{
    struct task *p;

    for( p = *pnext; p!=NULL; pnext = &p->next_task, p=p->next_task)
        if(p->completion > new->completion)
        {
            /*inserisci new dopo pnext, ossia prima di p*/
            insert_after_task(new, pnext);
            return;
        }
    insert_after_task(new,pnext);
}


void print_task_list(struct task*h)
{
    struct task *p;

    for(p=h; p!=NULL; p=p->next_task)
        printf("arrival %f\n", p->arrival);
        
    for(p=h; p!=NULL; p=p->next_task)
        printf("completion %f\n", p->completion);

}

void remove_central_task(struct task *r,struct task **pnext)
{
    struct task *p;

    for( p = *pnext; p!=NULL; pnext = &p->next_task, p=p->next_task) {
        if (p->next_task == r) {
            p->next_task = r->next_task;
            return;
        } else if (p == r) {
            remove_first_task(pnext);
            return;
        }
    }
}
struct task *find_tail(struct task **pnext)
{
    struct task *p;

    for (p = *pnext; p->next_task != NULL; p = p->next_task){
    }

    return p;
}