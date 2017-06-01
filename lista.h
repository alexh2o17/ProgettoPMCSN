
struct task{
		double arrival; //Istante di arrivo del task
		double completion; //Istante di completamento del task
		struct task *next_task; //Puntatore al prossimo task
		int perc_rim;
	};



struct task*alloc_task(void);
void free_task(struct task*);
void insert_after_task(struct task*, struct task**);
void remove_first_task(struct task**);
void insert_sorted_list( struct task*, struct task**);
void print_task_list(struct task*);
void remove_central_task(struct task *r,struct task **pnext);
struct task *find_tail(struct task **pnext);

