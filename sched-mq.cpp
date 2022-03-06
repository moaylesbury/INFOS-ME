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
        sched_log.messagef(LogLevel::DEBUG, "INSTANCE CREATED");
        // TODO: Implement me!
        // for (const auto& entity : runqueue) {
        //     sched_log.messagef(LogLevel::ERROR, "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii");
        // }
        
    }

    /**
	 * Called when a scheduling entity becomes eligible for running.
	 * @param entity
	 */
	void add_to_runqueue(SchedulingEntity& entity) override
	{
		UniqueIRQLock l;
        // if (entity.priority() == SchedulingEntityPriority::REALTIME){
        sched_log.messagef(LogLevel::ERROR, "HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
        // }
		runqueue.enqueue(&entity);
        begin = true;
	}

	/**
	 * Called when a scheduling entity is no longer eligible for running.
	 * @param entity
	 */
	void remove_from_runqueue(SchedulingEntity& entity) override
	{
		UniqueIRQLock l;
        sched_log.messagef(LogLevel::ERROR, "REMOVEEEEEEEEEEEEEEEEEEEEEEEE");
        
        if (MultipleQueuePriorityScheduler::in_list(entity, p1)){
            p1.remove(&entity);
        }

        if (q_pointer >= p1.count()) {
            q_pointer--;
        }

        sched_log.messagef(LogLevel::ERROR, "rmvd");
		
        if (runtimes.contains_key(&entity)){
            runtimes.remove(&entity);
        }
		runqueue.remove(&entity);
	}

    bool in_list(SchedulingEntity& entity, List<SchedulingEntity *> lst){
        for (auto ent : lst){
            if (ent == &entity){
                return true;
            }
        }
        return false;
    }

    void add_to_p1(SchedulingEntity* entity)
    {
        UniqueIRQLock l;
        p1.append(entity);
    }

    void rm_from_p1(SchedulingEntity* entity)
    {
        if (p1.count() != 0){
            UniqueIRQLock l;
            p1.remove(entity);
        }
    }

    SchedulingEntity* get_at(int ptr)
    {
        UniqueIRQLock l;
        return p1.at(ptr);
    }

    void add_to_map(SchedulingEntity* entity, SchedulingEntity::EntityRuntime time) {
        UniqueIRQLock l;
        runtimes.add(entity, time);
    }

    void rm_from_map(SchedulingEntity* entity) {
        syslog.messagef(LogLevel::DEBUG, "removing from map");
        UniqueIRQLock l;



        Map<SchedulingEntity *, SchedulingEntity::EntityRuntime> temp;

        // auto itr = runtimes.begin();
        // while (itr != runtimes.end()) {

        //     syslog.messagef(LogLevel::FATAL, "name - %s", ToString(itr->Key->name()).c_str());
        //     ++itr;
        // }

        // move all not equal to entity in runtimes to temp
        for (const auto& node : runtimes) {
            if (node.key != entity){
                temp.add(node.key, node.value);
            } else {        
                syslog.messagef(LogLevel::FATAL, "TRIGGERED =================================================");
            }
        }

        // clear runtimes
        runtimes.clear();

        // move all in temp to runtimes
        for (const auto& node : temp) {
            runtimes.add(node.key, node.value);
        }





        syslog.messagef(LogLevel::FATAL, "REMOVED FROM MAP");












        // // runtimes.remove(entity);
        // SchedulingEntity::EntityRuntime val;
        // if (runtimes.try_get_value(entity, val)){
        //     syslog.messagef(LogLevel::DEBUG, "in map222222222");
        // } else {
        //     syslog.messagef(LogLevel::DEBUG, "Success!");
        // }
















        // if (runtimes.try_get_value(entity, val)){
        //     syslog.messagef(LogLevel::DEBUG, "in map222222222");
        // }
    }

    /**
     * Called every time a scheduling event occurs, to cause the next eligible entity
     * to be chosen.  The next eligible entity might actually be the same entity, if
     * e.g. its timeslice has not expired.
     */
    SchedulingEntity *pick_next_entity() override
    {
        // if (runqueue.count() == 0) return NULL;
		// if (runqueue.count() == 1){
        //     if (runqueue.first()->state()==3) {
        //         return runqueue.first();
        //     } else {
        //         return NULL;
        //     }
        // } 
        // TODO: Implement me!

        // tests if the runqueue is being recieved properly
        // if (runqueue.count() == 0) {
        //     sched_log.messagef(LogLevel::DEBUG, "me sad:(");   
        // }else{
        //     sched_log.messagef(LogLevel::DEBUG, "me happi:)");
        // }

        //print int
        // syslog.messagef(LogLevel::FATAL, "runqueue count - %s", ToString(runqueue.count()).c_str());


        if (begin) {
            sched_log.messagef(LogLevel::DEBUG, "begin");

            q_pointer = 0;

            // split into relevant queues

            for (const auto& entity : runqueue) {
                syslog.messagef(LogLevel::FATAL, "runqueue count - %s", ToString(runqueue.count()).c_str());
                if (p1.count() == 0){
                    sched_log.messagef(LogLevel::DEBUG, "first step correct");
                }
                syslog.messagef(LogLevel::FATAL, "priority level - %s", ToString(entity->priority()).c_str());
                if (entity->state() == 3){
                    sched_log.messagef(LogLevel::FATAL, "runnable HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
                }
                if (entity->priority()) {
                    syslog.messagef(LogLevel::FATAL, "state - %s", ToString(entity->state()).c_str());
                    if (entity->state() != 4){
                    // if (entity->state() == 2){
                        sched_log.messagef(LogLevel::DEBUG, "yoooo");
                        MultipleQueuePriorityScheduler::add_to_p1(entity);
                    }
                    // p1.append(entity);
                    // sched_log.messagef(LogLevel::DEBUG, "me oh so happi");
                }
                if (p1.count() != 0){
                    sched_log.messagef(LogLevel::DEBUG, "second step correct");
                }
                
            }

            begin = false;
        }
        // return NULL;

        // if (p1.count() != 0) { sched_log.messagef(LogLevel::DEBUG, "me oh so happi"); }
        // if (p1.count() != 0) { sched_log.messagef(LogLevel::DEBUG, "me oh so happi"); }
        // Round-Robin

        
        


        while (true && !p1.empty()) {

            // get entity at pointer
            syslog.messagef(LogLevel::DEBUG, "p1 count - %s", ToString(p1.count()).c_str());
            syslog.messagef(LogLevel::DEBUG, "q_pointer - %s", ToString(q_pointer).c_str());
            syslog.messagef(LogLevel::DEBUG, "map for runtimes - %s", ToString(runtimes.count()).c_str());
            // sched_log.messagef(LogLevel::ERROR, "dies here");
            auto ent = MultipleQueuePriorityScheduler::get_at(q_pointer);
            // sched_log.messagef(LogLevel::ERROR, "psych");
            // syslog.messagef(LogLevel::DEBUG, "name - %s", ent->name().c_str());
            
            // get entities runtime
            auto now = ent->cpu_runtime();

            // get runtime before last iteration
            SchedulingEntity::EntityRuntime val;
            if (runtimes.try_get_value(ent, val)){
                syslog.messagef(LogLevel::DEBUG, "in map");
                // in map
                // syslog.messagef(LogLevel::DEBUG, "remove from runqueue - %s", ent->name().c_str());
                
                // if entity has ran over quantum
                // syslog.messagef(LogLevel::FATAL, "runtime - %s", ToString(now).c_str());
                if (now > 100000000) { //now-val previously
                    syslog.messagef(LogLevel::FATAL, "time over");
                    // sched_log.messagef(LogLevel::DEBUG, "AYO");

                    // remove from map
                    MultipleQueuePriorityScheduler::rm_from_map(ent);

                    //remove from p1
                    // MultipleQueuePriorityScheduler::rm_from_p1(ent);
                    // syslog.messagef(LogLevel::FATAL, "made modifications");


                    // MultipleQueuePriorityScheduler::add_to_map(ent, now);

                    // increment pointer
                    if (!p1.empty() && p1.count() != 1) {
                        sched_log.messagef(LogLevel::ERROR, "A");
                        q_pointer = (q_pointer + 1) % p1.count();
                    } else if (p1.count() == 1) {
                        sched_log.messagef(LogLevel::FATAL, "return");
                        if (ent->state() == 2){
                            return ent;
                        } else {
                            return NULL;
                        }
                    } else {
                        sched_log.messagef(LogLevel::ERROR, "B");
                        q_pointer = 0;

                        if (ent->state() == 2){
                            return ent;
                        } else {
                            return NULL;
                        }
                    }

                    
                    // syslog.messagef(LogLevel::FATAL, "incremented pointer");

                    syslog.messagef(LogLevel::DEBUG, "hello");
                    

                    // begin loop again
                } else {

                    //  if entity not exceeded quantum
                    sched_log.messagef(LogLevel::FATAL, "return");
                    MultipleQueuePriorityScheduler::rm_from_map(ent);
                    // return entitys
                    if (ent->state() == 2){
                            return ent;
                        } else {
                            return NULL;
                        }
                }


                
            } else {
                // not in map

                // add entity to map
                MultipleQueuePriorityScheduler::add_to_map(ent, now);

                sched_log.messagef(LogLevel::FATAL, "return");
                // return entity
                if (ent->state() == 2){
                    return ent;
                } else {
                    return NULL;
                }
            }
           
            // if (p1.count() == 0) {return NULL;}
    

        }
        






       

























        // sched_log.messagef(LogLevel::DEBUG, "intersting");
        sched_log.messagef(LogLevel::DEBUG, "doing something with runqueue of size - %s", ToString(runqueue.count()).c_str());
        sched_log.messagef(LogLevel::DEBUG, "and p1 size - %s", ToString(p1.count()).c_str());
        return NULL;































				
        
    }
private:
    List<SchedulingEntity *> runqueue;
    bool begin = true;
    int q_pointer = -1;

    Map<SchedulingEntity *, SchedulingEntity::EntityRuntime> runtimes;

    List<SchedulingEntity *> p1;
};

/* --- DO NOT CHANGE ANYTHING BELOW THIS LINE --- */

RegisterScheduler(MultipleQueuePriorityScheduler);