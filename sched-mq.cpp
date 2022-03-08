/*
 * The Priority Task Scheduler
 * SKELETON IMPLEMENTATION TO BE FILLED IN FOR TASK 1
 */

#include <infos/kernel/sched.h>
#include <infos/kernel/sched-entity.h>
#include <infos/kernel/thread.h>
#include <infos/kernel/log.h>
#include <infos/util/list.h>
#include <infos/util/map.h>
#include <infos/util/lock.h>


using namespace infos::kernel;
using namespace infos::util;

/**
 * A Multiple Queue priority scheduling algorithm
 */
class MultipleQueuePriorityScheduler : public SchedulingAlgorithm
{
public:
    // List<SchedulingEntity *> runqueue;
    /**
     * Returns the friendly name of the algorithm, for debugging and selection purposes.
     */
    const char* name() const override { return "mq"; }

    /**
     * Called during scheduler initialisation.
     */
    void init()
    {
        // TODO: Implement me!
    }

    /**
	 * Called when a scheduling entity becomes eligible for running.
	 * @param entity
	 */
	void add_to_runqueue(SchedulingEntity& entity) override
	{
		UniqueIRQLock l;
		runqueue.enqueue(&entity);
        // initiates current queue modification state
        begin = true;
	}

	/**
	 * Called when a scheduling entity is no longer eligible for running.
	 * @param entity
	 */
	void remove_from_runqueue(SchedulingEntity& entity) override
	{
		UniqueIRQLock l;

        // removes from any of the priority queues if present
        if (MultipleQueuePriorityScheduler::in_list(entity, p1)){
            p1.remove(&entity);
        }
        if (MultipleQueuePriorityScheduler::in_list(entity, p2)){
            p2.remove(&entity);
        }
        if (MultipleQueuePriorityScheduler::in_list(entity, p3)){
            p3.remove(&entity);
        }
        if (MultipleQueuePriorityScheduler::in_list(entity, p4)){
            p4.remove(&entity);
        }

        // remove from current queue if present
        if (MultipleQueuePriorityScheduler::in_list(entity, q)){
            q.remove(&entity);
            if (q.empty()){
                q_pointer = 0;
            } else {
                q_pointer = (q_pointer - 1) % q.count();
            }
        }

        // remove from runtimes map if present
        if (runtimes.contains_key(&entity)){
            MultipleQueuePriorityScheduler::rm_from_map(&entity);
        }

        // remove from runqueue
		runqueue.remove(&entity);
	}

    bool in_list(SchedulingEntity& entity, List<SchedulingEntity *> lst) {
        // takes an entity& and lst
        // returns true if entity in lst, else false
        for (auto ent : lst){
            if (ent == &entity){
                return true;
            }
        }
        return false;
    }

    bool in_list2(SchedulingEntity* entity, List<SchedulingEntity *> lst) {
        // takes an entity* and lst
        // returns true if entity in lst, else false
        for (auto ent : lst){
            if (ent == entity){
                return true;
            }
        }
        return false;
    }

    void add_to_list(SchedulingEntity* entity, int n) {
        // takes entity and integer n
        // adds entity to priority queue n if not already in
        if (n == 1) {
            if (!MultipleQueuePriorityScheduler::in_list2(entity, p1)){
                UniqueIRQLock l;
                p1.append(entity);
            }
        }
        if (n == 2) {
            if (!MultipleQueuePriorityScheduler::in_list2(entity, p2 )){
                UniqueIRQLock l;
                p2.append(entity);
            }
        }
        if (n == 3) {
            if (!MultipleQueuePriorityScheduler::in_list2(entity, p3)){
                UniqueIRQLock l;
                p3.append(entity);
            }
        }
        if (n == 4) {
            if (!MultipleQueuePriorityScheduler::in_list2(entity, p4)){
                UniqueIRQLock l;
                p4.append(entity);
            }
        }
    }

    void rm_from_list(SchedulingEntity* entity, int n) {
        // takes entity and integer n
        // removes entity from priority queue n if present
        if (n == 1) {
            if (!MultipleQueuePriorityScheduler::in_list2(entity, p1)){
                UniqueIRQLock l;
                p1.remove(entity);
            }
        }
        if (n == 2) {
            if (!MultipleQueuePriorityScheduler::in_list2(entity, p2 )){
                UniqueIRQLock l;
                p2.remove(entity);
            }
        }
        if (n == 3) {
            if (!MultipleQueuePriorityScheduler::in_list2(entity, p3)){
                UniqueIRQLock l;
                p3.remove(entity);
            }
        }
        if (n == 4) {
            if (!MultipleQueuePriorityScheduler::in_list2(entity, p4)){
                UniqueIRQLock l;
                p4.remove(entity);
            }
        }
    }

    SchedulingEntity* get_at(int ptr) {
        // takes int ptr
        // returns entity at index ptr from current queue
        UniqueIRQLock l;
        if (ptr < q.count()){
            return q.at(ptr);
        }
    }

    void add_to_map(SchedulingEntity* entity, int val) {
        // adds entity with value val to runtimes  map
        UniqueIRQLock l;
        runtimes.add(entity, val);
    }

    void rm_from_map(SchedulingEntity* entity) {
        // removes entity from runtimes map
        UniqueIRQLock l;

        // create temp queue
        Map<SchedulingEntity *, int> temp;

        // add all entities not equal to entity in runtimes to temp
        for (const auto& node : runtimes) {
            if (node.key != entity){
                temp.add(node.key, node.value);
            } 
        }

        // clear runtimes
        runtimes.clear();

        // add all entities from temp to runtimes
        for (const auto& node : temp) {
            runtimes.add(node.key, node.value);
        }

        // clear temp
        temp.clear();
    }




    void set_current_queue() {
        // finds highest priority occupied queue
        // transfer all entities from priority queue to current queue to be scheduled

        // if no occupied priority queues, return
        if (p1.empty() && p2.empty() && p3.empty() && p4.empty()) {
            return;
        }

        UniqueIRQLock l;

        // if priority queue is not empty
        if (!p1.empty()) {
            // transfer all entities from priority queue 1 to current queue q
            transfer(1);
            // clear priority queue 1
            p1.clear();
            // set quantum
            quantum = 2;
        }
        else if (!p2.empty()) {
            transfer(2);
            p2.clear();
            quantum = 4;
        }
        else if (!p3.empty()) {
            transfer(3);
            p3.clear();
            quantum = 8;
        }
        else if (!p4.empty()) {
            transfer(4);
            p4.clear();
            quantum = 16;
        }
    }

    void transfer(int n){
        // all entities from priority queue n are added to current queue q
        UniqueIRQLock l;

        if (n==1){
            for (const auto& ent : p1) {
                if (!MultipleQueuePriorityScheduler::in_list2(ent, q)){
                    q.append(ent);
                }
            }
        }
        if (n==2){
            for (const auto& ent : p2) {
                if (!MultipleQueuePriorityScheduler::in_list2(ent, q)){
                    q.append(ent);
                }
            }
        }
        if (n==3){
            for (const auto& ent : p3) {
                if (!MultipleQueuePriorityScheduler::in_list2(ent, q)){
                    q.append(ent);
                }
            }
        }
        if (n==4){
            for (const auto& ent : p4) {
                if (!MultipleQueuePriorityScheduler::in_list2(ent, q)){
                    q.append(ent);
                }
            }
        }
        
    }


    /**
     * Called every time a scheduling event occurs, to cause the next eligible entity
     * to be chosen.  The next eligible entity might actually be the same entity, if
     * e.g. its timeslice has not expired.
     */
    SchedulingEntity *pick_next_entity() override
    {
        // current queue modification state
        if (begin) {
            // assigns all entities in runqueue to a priority queue based on priority
            for (const auto& entity : runqueue) {

                // only if runnable
                if (entity->state() == 2) {

                    if (entity->priority() == 0){
                        MultipleQueuePriorityScheduler::add_to_list(entity, 1);
                    }
                    if (entity->priority() == 1){
                        MultipleQueuePriorityScheduler::add_to_list(entity, 2);
                    }
                    if (entity->priority() == 2){
                        MultipleQueuePriorityScheduler::add_to_list(entity, 3);
                    }
                    if (entity->priority() == 3){
                        MultipleQueuePriorityScheduler::add_to_list(entity, 4);
                    }

                }

                
            }

            // set current queue q
            set_current_queue();
            // end current queue modification
            begin = false;
        }

        // check for runnable entities that have changed state (previously not runnable)
        if (runqueue.count() != q.count()) {
            for (const auto& entity : runqueue){
                if (!MultipleQueuePriorityScheduler::in_list2(entity, q) && entity->state() == 2){
                    // initiate current queue modification
                    begin = true;
                }
            }
        }

        // =======================================
        // Round-Robin                           |
        // =======================================

        // while current queue q is occupied
        while (!q.empty()) {
            
            // if only one entity is present, return that entity
            if (q.count() == 1) {return q.first();}

            // get current entity
            auto ent = MultipleQueuePriorityScheduler::get_at(q_pointer);

            // get number of quantums executed for if present in runtimes map
            int iters;
            if (runtimes.try_get_value(ent, iters)){
                // entity present in map

                // if number of iterations exceeds the quantum
                if (iters >= quantum){
                    
                    // remove entity from runtime map
                    MultipleQueuePriorityScheduler::rm_from_map(ent);
        
                    // increment current queue pointer if queue has more than one entity
                    if (q.count() != 1) {
                        q_pointer = (q_pointer + 1) % q.count();
                    } 


                }
                 else {
                    // if quantum is not exceeded, increment iters in runtime map
                    MultipleQueuePriorityScheduler::rm_from_map(ent);
                    MultipleQueuePriorityScheduler::add_to_map(ent, iters + 1);
                    // return entity as quantum not exceeded
                    return ent;
                }

            } else {
                // if entity not in runtimes map

                // add entity to map with iters=1, as this is the entities first execution iteration
                iters = 1;
                MultipleQueuePriorityScheduler::add_to_map(ent, iters);

                // return entity 
                return ent;

            }           

        }

        // if all queues are empty, return null
        if (q.empty() && p1.empty() && p2.empty() && p3.empty() && p4.empty()){
            return NULL;
        } else {
            // otherwise begin current queue modification state
            begin = true;
        }
        
    }

private:
    // runqueue
    List<SchedulingEntity *> runqueue;

    // if true, begin current queue modification state
    bool begin = true;
    // points to scheduled entity in current queue
    int q_pointer = 0;
    // number of executions entity can perform, dependent on priority; values in {2,4,8,16}; initiated to -1
    int quantum = -1;

    // runtimes map
    // key: entity
    // val: number of iterations executed for; values range from 0-quantum 
    Map<SchedulingEntity *, int> runtimes;

    // priority queues, where p1 is highest priority and p4 is lowest
    List<SchedulingEntity *> p1; // REALTIME
    List<SchedulingEntity *> p2; // INTERACTIVE
    List<SchedulingEntity *> p3; // NORMAL
    List<SchedulingEntity *> p4; // DAEMON
      
    // current queue, the queue for scheduling of highest priority entities
    List<SchedulingEntity *> q;
};

/* --- DO NOT CHANGE ANYTHING BELOW THIS LINE --- */

RegisterScheduler(MultipleQueuePriorityScheduler);