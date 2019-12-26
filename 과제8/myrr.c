#include "sched.h"
void init_myrr_rq(struct myrr_rq *myrr_rq)
{
    printk(KERN_INFO "***[MYRR] Mysched class is online \n");
    myrr_rq->nr_running = 0;
    INIT_LIST_HEAD(&myrr_rq->queue);
}
/*static void resched(struct rq *rq) {
 struct myrr_rq *myrr_rq = &rq->myrr;
 struct task_struct *curr = &rq->curr;
 struct sched_myrr_entity *myrr_ = &curr->myrr;
 }*/

static void enqueue_task_myrr(struct rq *rq, struct task_struct *p, int flags)
{
    struct myrr_rq *myrr_rq = &rq->myrr;
    struct sched_myrr_entity *task = &p->myrr;
    
    list_add_tail(&task->run_list, &myrr_rq->queue);
    myrr_rq->nr_running++;
    
    //TODO
    //1. enqueue the task in mysched_rq
    //2. increment nr_running value of mysched_rq by 1
    printk(KERN_INFO "***[MYRR] enqueue: success cpu=%d,nr_running=%d,pid=%d\n", cpu_of(rq),myrr_rq->nr_running,p->pid);
}
static void dequeue_task_myrr(struct rq *rq, struct task_struct *p, int flags)
{
    struct myrr_rq *myrr_rq = &rq->myrr;
    //struct sched_mysched_entity *task = &p->mysched;
    
    //TODO
    if(/*1. is queue empty?*/ myrr_rq->nr_running > 0)
    {
        list_del_init(&p->myrr.run_list);
        myrr_rq->nr_running--;
        //2. dequeue the task from mysched_rq
        //3. decrement nr_running value of mysched_rq by 1
        printk(KERN_INFO "\t***[MYRR] dequeue: success cpu=%d,nr_running=%d,pid=%d\n", cpu_of(rq),myrr_rq->nr_running, p->pid);
        if(/*4. Are current task and picked task same task?*/ rq->curr == p)
        {
            resched_curr(rq);
        }
    }
    else
    {
    }
}
static void check_preempt_curr_myrr(struct rq *rq, struct task_struct *p, int flags) { }

struct task_struct *pick_next_task_myrr(struct rq *rq, struct task_struct *prev)
{
    struct task_struct *next_p = NULL;
    struct sched_myrr_entity *next_se = NULL;
    struct myrr_rq *myrr_rq = &rq->myrr;
    struct list_head *pick = myrr_rq->queue.next;
    //TODO
    if(/*1. is queue empty?*/ rq->myrr.nr_running == 0){
        return NULL;
    }
    
    next_se = container_of(pick, struct sched_myrr_entity, run_list);
    next_p = container_of(next_se, struct task_struct, myrr);
    
    //2. pick one sched_mysched_entity from mysched_rq and assign it to next_se()
    //3. find parent task_struct of picked sched_mysched_entity and assign it to next_p
    printk(KERN_INFO "\t***[MYRR] pick_next_task: cpu=%d,prev->pid=%d,next_p->pid=%d,nr_running=%d\n",cpu_of(rq),prev->pid,next_p->pid,myrr_rq->nr_running);
    return next_p;
}

static void update_curr_myrr(struct rq *rq){
    //TODO
    
    struct task_struct *next_p = NULL;
    struct sched_myrr_entity *myrr_en = NULL;
    struct myrr_rq *myrr_rq = &rq->myrr;
    struct list_head *task = myrr_rq->queue.next;
    
    myrr_en = container_of(task, struct sched_myrr_entity, run_list);
    next_p = container_of(myrr_en, struct task_struct, myrr);
    //struct myrr_rq *myrr_rq = &rq->myrr;
    //struct task_struct *task = &myrr_rq->queue.next;
    //struct sched_myrr_entity *myrr_en = &task->myrr;
    //int shced_rr_timeslice = RR_TIMESLICE;
    
    myrr_en->update_num++;
    printk(KERN_INFO "\t***[MYRR] update_curr_myrr pid=%d update_num=%d\n", next_p->pid, myrr_en->update_num)
    if( myrr_en->update_num > 3) {
        myrr_en->update_num = 0;
        dequeue_task_myrr(rq, next_p, 0);
        enqueue_task_myrr(rq, next_p, 0);
        resched_curr(rq);
        //resched(rq);
    }
    //1. update update_num
    //2. if update_num > time slice
    //      do something;
    //      resched(rq);
    
}

static void task_tick_myrr(struct rq *rq, struct task_struct *p, int queued){
    //TODO
    //1. call update_curr_myrr method
    update_curr_myrr(rq);
}

static void put_prev_task_myrr(struct rq *rq, struct task_struct *p) {
    printk(KERN_INFO"\tput_prev_task: do nothing, p->pid=%d\n", p->pid);
}
static int select_task_rq_myrr(struct task_struct *p, int cpu, int sd_flag, int flags) {        return task_cpu(p);
}
static void set_curr_task_myrr(struct rq *rq) { }
//static void task_tick_mysched(struct rq *rq, struct task_struct *p, int queued) { }
static void prio_changed_myrr(struct rq *rq, struct task_struct *p, int oldprio) { }
/*This routine is called when a task migrates between classes*/
static void switched_to_myrr(struct rq *rq, struct task_struct *p)
{
    resched_curr(rq);
}
const struct sched_class myrr_sched_class={
    .next=&fair_sched_class,
    .enqueue_task=enqueue_task_myrr,
    .dequeue_task=dequeue_task_myrr,
    .check_preempt_curr=check_preempt_curr_myrr,
    .pick_next_task=pick_next_task_myrr,
    .put_prev_task=put_prev_task_myrr,
#ifdef CONFIG_SMP
    .select_task_rq=select_task_rq_myrr,
#endif
    .set_curr_task=set_curr_task_myrr,
    .task_tick=task_tick_myrr,
    .prio_changed=prio_changed_myrr,
    .switched_to=switched_to_myrr,
    .update_curr=update_curr_myrr,
    //rear %= 100;
};
