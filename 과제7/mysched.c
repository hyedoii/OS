#include "sched.h"
void init_mysched_rq (struct mysched_rq *mysched_rq)
{
                printk(KERN_INFO "***[MYSCHED] Mysched class is online \n");
                mysched_rq->nr_running = 0;
                INIT_LIST_HEAD(&mysched_rq->queue);
}
static void update_curr_mysched(struct rq *rq){}
static void enqueue_task_mysched(struct rq *rq, struct task_struct *p, int flags)
{
    struct mysched_rq *mysched_rq = &rq->mysched;
    struct sched_mysched_entity *task = &p->mysched;
    
    list_add_tail(&task->run_list, &mysched_rq->queue);
    mysched_rq->nr_running++;
    
    //TODO
    //1. enqueue the task in mysched_rq
    //2. increment nr_running value of mysched_rq by 1
    printk(KERN_INFO "***[MYSCHED] enqueue: success cpu=%d,nr_running=%d,pid=%d\n", cpu_of(rq),mysched_rq->nr_running,p->pid);

}
static void dequeue_task_mysched(struct rq *rq, struct task_struct *p, int flags)
{
    struct mysched_rq *mysched_rq = &rq->mysched;
    //struct sched_mysched_entity *task = &p->mysched;
    
    //TODO
    if(/*1. is queue empty?*/ mysched_rq->nr_running > 0)
    {
        list_del_init(&p->mysched.run_list);
        mysched_rq->nr_running--;
        //2. dequeue the task from mysched_rq
        //3. decrement nr_running value of mysched_rq by 1
        printk(KERN_INFO "\t***[MYSCHED] dequeue: success cpu=%d,nr_running=%d,pid=%d\n", cpu_of(rq),mysched_rq->nr_running, p->pid);
        if(/*4. Are current task and picked task same task?*/ rq->curr == p)
        {
            resched_curr(rq);
        }
    }
    else
    {
    }

}
static void check_preempt_curr_mysched(struct rq *rq, struct task_struct *p, int flags) { }

struct task_struct *pick_next_task_mysched(struct rq *rq, struct task_struct *prev)
{
    struct task_struct *next_p = NULL;
    struct sched_mysched_entity *next_se = NULL;
    struct mysched_rq *mysched_rq = &rq->mysched;
    struct list_head *pick = mysched_rq->queue.next;
    //TODO
    if(/*1. is queue empty?*/ rq->mysched.nr_running == 0){
        return NULL;
    }
    
    next_se = container_of(pick, struct sched_mysched_entity, run_list);
    next_p = container_of(next_se, struct task_struct, mysched);
    
    //2. pick one sched_mysched_entity from mysched_rq and assign it to next_se()
    //3. find parent task_struct of picked sched_mysched_entity and assign it to next_p
    printk(KERN_INFO "\t***[MYSCHED] pick_next_task: cpu=%d,prev->pid=%d,next_p->pid=%d,nr_running=%d\n",cpu_of(rq),prev->pid,next_p->pid,mysched_rq->nr_running);
    return next_p;
}

static void put_prev_task_mysched(struct rq *rq, struct task_struct *p) { }
static int select_task_rq_mysched(struct task_struct *p, int cpu, int sd_flag, int flags) {     return task_cpu(p);
}
static void set_curr_task_mysched(struct rq *rq) { }
static void task_tick_mysched(struct rq *rq, struct task_struct *p, int queued) { }
static void prio_changed_mysched(struct rq *rq, struct task_struct *p, int oldprio) { }
/*This routine is called when a task migrates between classes*/
static void switched_to_mysched(struct rq *rq, struct task_struct *p)
{
        resched_curr(rq);
}
const struct sched_class mysched_sched_class={
        .next=&fair_sched_class,
        .enqueue_task=enqueue_task_mysched,
        .dequeue_task=dequeue_task_mysched,
        .check_preempt_curr=check_preempt_curr_mysched,
        .pick_next_task=pick_next_task_mysched,
        .put_prev_task=put_prev_task_mysched,
#ifdef CONFIG_SMP
        .select_task_rq=select_task_rq_mysched,
#endif
        .set_curr_task=set_curr_task_mysched,
        .task_tick=task_tick_mysched,
        .prio_changed=prio_changed_mysched,
        .switched_to=switched_to_mysched,
        .update_curr=update_curr_mysched,
                        //rear %= 100;
};
