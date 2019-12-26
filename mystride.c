#include "sched.h"
void init_mystride_rq (struct mystride_rq *mystride_rq)
{
        printk(KERN_INFO "***[MYSTRD] Mystride class is online \n");
        mystride_rq->nr_running = 0;
        INIT_LIST_HEAD(&mystride_rq->queue);
}

void init_task_mystride(struct task_struct *p)
{
        struct sched_mystride_entity *se = &p->mystride;

        p->sched_class = &mystride_sched_class;
        se->stride = 1000/(se->ticket);
        se->pass = se->stride;

        printk(KERN_INFO "***[MYSTRD] init_task_mystrd: ticket=%d,stride=%d,pass=%d,p->pid=%d\n", se->ticket, se->stride, se->pass, p->pid);
}
static void enqueue_task_mystride(struct rq *rq, struct task_struct *p, int flags)
{
        struct mystride_rq *mystride_rq = &rq->mystride;
        struct sched_mystride_entity *task = &p->mystride;
        struct list_head *head = &mystride_rq->queue;
        struct list_head *pick = &task->run_list;
        struct sched_mystride_entity *next = container_of(head, struct sched_mystride_entity, run_list);
        int i = 0;

        if( mystride_rq->nr_running == 0 ) {
                list_add(pick, &mystride_rq->queue);
                mystride_rq->nr_running++;
                printk(KERN_INFO " ***[MYSTRD] enqueue | nr_running = %d\n", mystride_rq->nr_running);
        }
        else {
                head = head->next;
                next = container_of(head, struct sched_mystride_entity, run_list);
                //printk(KERN_INFO, "***[MYSTRD] enqueue for : next ticket 1111t = %d\n", next->ticket);
                for( i = 0 ; i < mystride_rq->nr_running; i++ ) {
                        printk(KERN_INFO "***[MYSTRD] enqueue for\n");
                        if(next->pass >= task->pass ) break;
                        head = head->next;
                        next = container_of(head, struct sched_mystride_entity, run_list);
                        //printk(KERN_INFO, "***[MYSTRD] enqueue for : next ticket 2222  = %d\n", next->ticket);
                }
                list_add_tail(&task->run_list, head);
                mystride_rq->nr_running++;
                printk(KERN_INFO "***[MYSTRD] enqueue: success cpu=%d,nr_running=%d,p->state=%lu,p->pid=%d\n", cpu_of(rq),mystride_rq->nr_running,p->state,p->pid);

                        //pick = next->run_list.next;
                        //next = container_of(pick, struct sched_mystride_entity, run_list);

                //TODO
                // make sure the new task is enqueueing in right position
                //printk(KERN_INFO "***[MYSTRD] enqueue: success cpu=%d,nr_running=%d,pid=%d\n", cpu_of(rq),mystride_rq->nr_running,p->pid);
        }
}
static void dequeue_task_mystride(struct rq *rq, struct task_struct *p, int flags)
{
        struct mystride_rq *mystride_rq = &rq->mystride;
        //struct sched_mysched_entity *task = &p->mysched;
        //printk(KERN_INFO "***[MYSTRD] dequeue: start\n");

        //TODO
        if(/*1. is queue empty?*/ mystride_rq->nr_running > 0)
        {
                printk(KERN_INFO "***[MYSTRD dequeue: start\n");
                list_del_init(&p->mystride.run_list);
                mystride_rq->nr_running--;
                //2. dequeue the task from mysched_rq
                //3. decrement nr_running value of mysched_rq by 1
                printk(KERN_INFO "\t***[MYSTRD] dequeue: success cpu=%d,nr_running=%d,p->state=%lu,p->pid=%d\n", cpu_of(rq),mystride_rq->nr_running,p->state,  p->pid);
                if(/*4. Are current task and picked task same task?*/ rq->curr == p)
                {
                        resched_curr(rq);
                }
        }
        else
        {
        }
        printk(KERN_INFO "***[MYSTRD] duqueue: end\n");
}
static void update_curr_mystride(struct rq *rq){
        struct task_struct *task = NULL;
        struct sched_mystride_entity *mystride = NULL;
        struct mystride_rq *mystride_rq = &rq->mystride;
        struct list_head *pick = mystride_rq->queue.next;

        struct task_struct *compare_struct = NULL;
        struct sched_mystride_entity *compare = NULL;
        struct list_head *compare_pick = NULL;

        int i = 0;

        mystride = container_of(pick, struct sched_mystride_entity, run_list);

        task = container_of(mystride, struct task_struct, mystride);

        compare_struct = task;
        //update the curr task's pass
        pick = mystride->run_list.next;

        compare = container_of(pick, struct sched_mystride_entity, run_list);
        compare_struct = container_of(compare, struct task_struct, mystride);
        compare_pick = compare->run_list.next;

        compare->pass += compare->stride;
        //task = container_of(next_se, struct task_struct, mystride);
        printk(KERN_INFO "***[MYSTRD] update_curr: update_curr starting!\n");

        //make mystride_rq sorted by pass
        for( i = 0; i < mystride_rq->nr_running ; i++) {
                printk(KERN_INFO "/t/t***[MYSTRD] update_curr: curr->pid=%d,p->pid=%d,pass=%d\n", task->pid, compare_struct->pid, compare->pass);
                compare_pick = compare_pick->next;
                compare = container_of(compare_pick, struct sched_mystride_entity, run_list);
                compare_struct = container_of(compare, struct task_struct, mystride);
        }
        dequeue_task_mystride(rq, task, 0);
        enqueue_task_mystride(rq, task, 0);
        // if any task has smaller pass than curr task's pass resched_curr(rq)
        resched_curr(rq);
        printk(KERN_INFO "***[MYSTRD] update_curr: update_curr end!\n");

        //TODO
        //update the curr task's pass and make mystride_rq sorted by pass
        // if any task has smaller pass than curr task's pass
        //      resched_curr(rq)

}
static void check_preempt_curr_mystride(struct rq *rq, struct task_struct *p, int flags) { }

struct task_struct *pick_next_task_mystride(struct rq *rq, struct task_struct *prev)
{
        struct task_struct *next_p = NULL;
        struct sched_mystride_entity *next_se = NULL;
        struct mystride_rq *mystride_rq = &rq->mystride;
        struct list_head *pick = mystride_rq->queue.next;

        //printk(KERN_INFO "***[MYSTRD] pick_next_task: start\n");
        //TODO
        if(/*1. is queue empty?*/ rq->mystride.nr_running == 0){
                return NULL;
        }
        if(prev->sched_class != &mystride_sched_class)
        {
                printk(KERN_INFO "***[MYSTRD] pick_next_task: other class came in.. prev->pid=%d\n", prev->pid);
                put_prev_task(rq, prev);
        }
        printk(KERN_INFO "***[MYSTRD] pick_next_task: start\n");

        next_se = container_of(pick, struct sched_mystride_entity, run_list);
        next_p = container_of(next_se, struct task_struct, mystride);

        //2. pick one sched_mysched_entity from mysched_rq and assign it to next_se()
        //3. find parent task_struct of picked sched_mysched_entity and assign it to next_p
        printk(KERN_INFO "\t***[MYSTRD] pick_next_task: cpu=%d,prev->pid=%d,next_p->pid=%d,nr_running=%d\n",cpu_of(rq),prev->pid,next_p->pid,mystride_rq->nr_running);
        printk(KERN_INFO "***[MYSTRD] pick_next_task: end\n");
        return next_p;
}

static void put_prev_task_mystride(struct rq *rq, struct task_struct *p) {
        printk(KERN_INFO "***[MYSTRD] put_prev_task: do nothing, p->pid=%d\n", p->pid);
}
static int select_task_rq_mystride(struct task_struct *p, int cpu, int sd_flag, int flags) {    return task_cpu(p);
}
static void set_curr_task_mystride(struct rq *rq) { }
static void task_tick_mystride(struct rq *rq, struct task_struct *p, int queued) {
        printk(KERN_INFO"***[MYSTRD] task_tick\n");
        update_curr_mystride(rq);
}
static void prio_changed_mystride(struct rq *rq, struct task_struct *p, int oldprio) { }
/*This routine is called when a task migrates between classes*/
static void switched_to_mystride(struct rq *rq, struct task_struct *p)
{
        resched_curr(rq);
}
const struct sched_class mystride_sched_class={
        .next=&fair_sched_class,
        .enqueue_task=enqueue_task_mystride,
        .dequeue_task=dequeue_task_mystride,
        .check_preempt_curr=check_preempt_curr_mystride,
        .pick_next_task=pick_next_task_mystride,
        .put_prev_task=put_prev_task_mystride,
#ifdef CONFIG_SMP
        .select_task_rq=select_task_rq_mystride,
#endif
        .set_curr_task=set_curr_task_mystride,
        .task_tick=task_tick_mystride,
        .prio_changed=prio_changed_mystride,
        .switched_to=switched_to_mystride,
        .update_curr=update_curr_mystride,
        //rear %= 100;
};
