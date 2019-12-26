#include "sched.h"
void init_myprio_rq (struct myprio_rq *myprio_rq)
{
    printk(KERN_INFO "***[MYPRIO] Myprio class is online \n");
    myprio_rq->nr_running = 0;
    INIT_LIST_HEAD(&myprio_rq->queue);
}
void init_task_myprio(struct task_struct *p)
{
    struct sched_myprio_entity *se = &p->myprio;
    
    //stride will be 10~1000(int)
    p->sched_class = &myprio_sched_class;
    if((se->stride % 100) < 50)
        se->aging = (se->stride)/100;
    else      //round aging by stride
        se->aging = (se->stride)/100 + 1 ;
    //any task must be executed before 100 upadates
    se->pass = se->stride;
}

//not only update curr, but also aging the other tasks.
static void update_curr_myprio(struct rq *rq)
{
    struct myprio_rq *myprio_rq = &rq->myprio;
    struct list_head *myprio_queue = &myprio_rq->queue;
    
    struct sched_myprio_entity *se = NULL;
    struct task_struct *p = NULL;
    
    printk(KERN_INFO "      ***[MYPRIO] update_curr: update_curr starting!\n");
    
    ((rq->curr)->myprio).pass += ((rq->curr)->myprio).stride;
    
    int i=0;
    for(i;i<myprio_rq->nr_running;i++) {
        myprio_queue = myprio_queue->next;
        se = container_of(myprio_queue, struct sched_myprio_entity, run_list);
        p = container_of(se, struct task_struct, myprio);
        
        if(p->pid != (rq->curr)->pid) {
            //if se->pass be less than 0, set 0.
            if((se->pass - se->aging) < 0)
                se->pass = 0;
            se->pass -= se->aging;      //decrease other tasks' pass by aging to increase priority
        }
        printk(KERN_INFO "         ***[MYPRIO] update_curr: curr->pid=%d, p->pid=%d, pass=%u, stride=%u\n", (rq->curr)->pid ,p->pid, se->pass, se->stride);
    }
    
    //we don't change the order of rq because sorting every update is more difficult.
    //for every update, we must pick minimum-pass task.
    resched_curr(rq);
    
    printk(KERN_INFO "      ***[MYPRIO] update_curr: update_curr end!\n");
}
static void enqueue_task_myprio(struct  rq *rq, struct task_struct *p, int flags)
{
    struct myprio_rq *myprio_rq = &rq->myprio;
    struct list_head *myprio_queue = &myprio_rq->queue;
    struct sched_myprio_entity *mse = &p->myprio;
    struct list_head *mse_run_list = &mse->run_list;
    
    //just enqueue to last order
    list_add_tail(mse_run_list, myprio_queue);
    
    //increase nr_running and change state
    myprio_rq->nr_running++;
    
    printk(KERN_INFO "***[MYPRIO] enqueue: success cpu=%d, nr_running=%d, p->state=%lu, p->pid=%d, pass=%u,  stride=%u\n", cpu_of(rq), myprio_rq->nr_running, p->state, p->pid, mse->pass, mse->stride);
    
}

static void dequeue_task_myprio(struct rq *rq, struct task_struct *p, int flags)
{
    struct myprio_rq *myprio_rq = &rq->myprio;
    struct list_head *myprio_queue = &myprio_rq->queue;
    struct sched_myprio_entity *mse = &p->myprio;
    struct list_head *mse_run_list = &mse->run_list;
    
    //check whether queue is empty
    if(myprio_rq->nr_running != 0)
    {
        //delete the task and decrease nr_running
        list_del_init(mse_run_list);
        myprio_rq->nr_running--;
        
        printk(KERN_INFO "***[MYPRIO] dequeue: success cpu=%d, nr_running=%d, p->state=%lu, p->pid=%d, pass=%u, stride=%u\n", cpu_of(rq), myprio_rq->nr_running, p->state, p->pid, mse->pass, mse->stride);
    }
    else
    {
    }
}

static void check_preempt_curr_myprio(struct rq *rq, struct task_struct *p, int flags) {}
struct task_struct *pick_next_task_myprio(struct rq *rq, struct task_struct *prev)
{
    struct sched_myprio_entity *se = NULL;
    struct task_struct *next_p = NULL;
    struct sched_myprio_entity *next_se = NULL;
    
    struct myprio_rq *myprio_rq = &rq->myprio;
    struct list_head *myprio_queue = &myprio_rq->queue;
    
    if(myprio_rq->nr_running == 0) {
        return NULL;
    }
    
    if(prev->sched_class != &myprio_sched_class) {
        printk(KERN_INFO "    ***[MYPRIO] pick_next_task: other class came in.. prev->pid=%d\n", prev->pid);
        put_prev_task(rq, prev);
    }
    
    //select the minimum-pass task in rq
    //if the minimum-pass tasks are more than 2,
    //select the one which has the least stride.
    int i;
    unsigned int minimum_pass;
    unsigned int minimum_stride;
    
    next_se = container_of(myprio_queue->next, struct sched_myprio_entity, run_list);
    next_p = container_of(next_se, struct task_struct, myprio);
    
    minimum_pass = next_se->pass;
    minimum_stride = next_se->stride;
    
    for(i = 0; i < myprio_rq->nr_running; i++) {
        myprio_queue = myprio_queue->next;
        se = container_of(myprio_queue, struct sched_myprio_entity, run_list);
        if(se->pass < minimum_pass)
        {
            minimum_pass = se->pass;
            minimum_stride = se->stride;
            next_se = se;
        }
        else if(se->pass == minimum_pass) hihellobye
        {
            if(se->stride < minimum_stride) {
                minimum_stride = se->stride;
                next_se = se;
            }
        }
    }
    next_p = container_of(next_se, struct task_struct, myprio);
    
    printk(KERN_INFO "    ***[MYPRIO] pick_next_task: cpu=%d, prev->pid=%d, next_p->pid=%d, next_pass=%u, next_stride=%u, nr_running=%d\n", cpu_of(rq),prev->pid, next_p->pid, next_se->pass, next_se->stride, myprio_rq->nr_running);
    
    return next_p;
}

static void put_prev_task_myprio(struct rq *rq, struct task_struct *p)
{
    printk(KERN_INFO "    ***[MYPRIO] put_prev_task: do nothing, p->pid=%d\n", p->pid);
}
static int select_task_rq_myprio(struct task_struct *p, int cpu, int sd_flas, int flags) {
    return task_cpu(p);
}
static void set_curr_task_myprio(struct rq *rq) {}
static void task_tick_myprio(struct rq *rq, struct task_struct *p, int queue) {
    update_curr_myprio(rq);
}
static void prio_changed_myprio(struct rq *rq, struct task_struct *p, int oldprio) {}
static void switched_to_myprio(struct rq *rq, struct task_struct *p)
{
    resched_curr(rq);
}
const struct sched_class myprio_sched_class={
    .next=&mysched_sched_class,
    .enqueue_task=enqueue_task_myprio,
    .dequeue_task=dequeue_task_myprio,
    .check_preempt_curr=check_preempt_curr_myprio,
    .pick_next_task=pick_next_task_myprio,
    .put_prev_task=put_prev_task_myprio,
#ifdef CONFIG_SMP
    .select_task_rq=select_task_rq_myprio,
#endif
    .set_curr_task=set_curr_task_myprio,
    .task_tick=task_tick_myprio,
    .prio_changed=prio_changed_myprio,
    .switched_to=switched_to_myprio,
    .update_curr=update_curr_myprio,
};


