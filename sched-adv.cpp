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
class MultilevelFeedbackQueuePriorityScheduler : public SchedulingAlgorithm
{
public:
    // List<SchedulingEntity *> runqueue;
    /**
     * Returns the friendly name of the algorithm, for debugging and selection purposes.
     */
    const char* name() const override { return "adv"; }

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
        add_to_list(&entity, 1);
	}

	/**
	 * Called when a scheduling entity is no longer eligible for running.
	 * @param entity
	 */
	void remove_from_runqueue(SchedulingEntity& entity) override
	{
		UniqueIRQLock l;

        // removes from any of the priority queues if present
        if (MultilevelFeedbackQueuePriorityScheduler::in_list(entity, q1)){
            q1.remove(&entity);
            if (q1.empty()){
                q1_pointer = 0;
            } else {
                q1_pointer = (q1_pointer - 1) % q1.count();
            }
        }
        if (MultilevelFeedbackQueuePriorityScheduler::in_list(entity, q2)){
            q2.remove(&entity);
            if (q2.empty()){
                q2_pointer = 0;
            } else {
                q2_pointer = (q2_pointer - 1) % q2.count();
            }
        }
        if (MultilevelFeedbackQueuePriorityScheduler::in_list(entity, q3)){
            q3.remove(&entity);
        }

        // remove from runtimes map if present
        if (runtimes.contains_key(&entity)){
            MultilevelFeedbackQueuePriorityScheduler::rm_from_map(&entity);
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
            if (!MultilevelFeedbackQueuePriorityScheduler::in_list2(entity, q1)){
                UniqueIRQLock l;
                q1.append(entity);
            }
        }
        if (n == 2) {
            if (!MultilevelFeedbackQueuePriorityScheduler::in_list2(entity, q2)){
                UniqueIRQLock l;
                q2.append(entity);
            }
        }
        if (n == 3) {
            if (!MultilevelFeedbackQueuePriorityScheduler::in_list2(entity, q3)){
                UniqueIRQLock l;
                q3.append(entity);
            }
        }
    }

    void rm_from_list(SchedulingEntity* entity, int n) {
        // takes entity and integer n
        // removes entity from priority queue n if present
        if (n == 1) {
            if (MultilevelFeedbackQueuePriorityScheduler::in_list2(entity, q1)){
                UniqueIRQLock l;
                q1.remove(entity);
                if (q1.empty()){
                    q1_pointer = 0;
                } else {
                    q1_pointer = (q1_pointer - 1) % q1.count();
                }
            }
        }
        if (n == 2) {
            if (MultilevelFeedbackQueuePriorityScheduler::in_list2(entity, q2)){
                UniqueIRQLock l;
                q2.remove(entity);
                if (q2.empty()){
                    q2_pointer = 0;
                } else {
                    q2_pointer = (q2_pointer - 1) % q2.count();
                }
            }
        }
        if (n == 3) {
            if (MultilevelFeedbackQueuePriorityScheduler::in_list2(entity, q3)){
                UniqueIRQLock l;
                q3.remove(entity);
            }
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



    /**
     * Called every time a scheduling event occurs, to cause the next eligible entity
     * to be chosen.  The next eligible entity might actually be the same entity, if
     * e.g. its timeslice has not expired.
     */
    SchedulingEntity *pick_next_entity() override
    {
       

        // =======================================
        // Round-Robin                           |
        // =======================================

        while (!q1.empty()){
            
            // if only one entity is present, return that entity
            if (q1.count() == 1) {return q1.first();}

            // get current entity
            auto ent = q1.at(q1_pointer);

            // get number of quantums executed for if present in runtimes map
            int iters;
            if (runtimes.try_get_value(ent, iters)){
                // entity present in map

                // if number of iterations exceeds the quantum
                if (iters >= 8){
                    // remove entity from runtime map
                    MultilevelFeedbackQueuePriorityScheduler::rm_from_map(ent);

                    // move to queue 2
                    MultilevelFeedbackQueuePriorityScheduler::add_to_list(ent, 2);
                    MultilevelFeedbackQueuePriorityScheduler::rm_from_list(ent, 1);
                    
        
                    // increment current queue pointer if queue has more than one entity
                    if (q1.count() != 1) {
                        q1_pointer = (q1_pointer + 1) % q1.count();
                    } 


                }
                 else {
                    // if quantum is not exceeded, increment iters in runtime map
                    MultilevelFeedbackQueuePriorityScheduler::rm_from_map(ent);
                    MultilevelFeedbackQueuePriorityScheduler::add_to_map(ent, iters + 1);
                    // return entity as quantum not exceeded
                    return ent;
                }

            } else {
                // if entity not in runtimes map

                // add entity to map with iters=1, as this is the entities first execution iteration
                iters = 1;
                MultilevelFeedbackQueuePriorityScheduler::add_to_map(ent, iters);

                // return entity 
                return ent;

            }   
        }

        while (!q2.empty()){
            
            // if only one entity is present, return that entity
            if (q2.count() == 1) {return q2.first();}

            // get current entity
            auto ent = q2.at(q2_pointer);

            // get number of quantums executed for if present in runtimes map
            int iters;
            if (runtimes.try_get_value(ent, iters)){
                // entity present in map

                // if number of iterations exceeds the quantum
                if (iters >= 16){
                    
                    // remove entity from runtime map
                    MultilevelFeedbackQueuePriorityScheduler::rm_from_map(ent);

                    // move to queue 3
                    MultilevelFeedbackQueuePriorityScheduler::rm_from_list(ent, 2);
                    MultilevelFeedbackQueuePriorityScheduler::add_to_list(ent, 3);
        
                    // increment current queue pointer if queue has more than one entity
                    if (q2.count() != 1) {
                        q2_pointer = (q2_pointer + 1) % q2.count();
                    } 


                }
                 else {
                    // if quantum is not exceeded, increment iters in runtime map
                    MultilevelFeedbackQueuePriorityScheduler::rm_from_map(ent);
                    MultilevelFeedbackQueuePriorityScheduler::add_to_map(ent, iters + 1);
                    // return entity as quantum not exceeded
                    return ent;
                }

            } else {
                // if entity not in runtimes map

                // add entity to map with iters=1, as this is the entities first execution iteration
                iters = 1;
                MultilevelFeedbackQueuePriorityScheduler::add_to_map(ent, iters);

                // return entity 
                return ent;

            }  

        }

        // =======================================
        // FCFS                                  |
        // =======================================


        while (!q3.empty()){
            return q3.first();
        }

        return NULL;        
    }

private:
    // runqueue
    List<SchedulingEntity *> runqueue;

    // points to scheduled entity in current queue
    int q1_pointer = 0;
    int q2_pointer = 0;

    Map<SchedulingEntity *, int> runtimes;


    // priority queues, where q1 is highest priority and q3 is lowest
    List<SchedulingEntity *> q1; // RR
    List<SchedulingEntity *> q2; // RR
    List<SchedulingEntity *> q3; // FCFS
};

/* --- DO NOT CHANGE ANYTHING BELOW THIS LINE --- */

RegisterScheduler(MultilevelFeedbackQueuePriorityScheduler);