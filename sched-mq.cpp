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
        //     sched_log.messagef(LogLevel::ERROR, "HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
        // }
		runqueue.enqueue(&entity);
	}

	/**
	 * Called when a scheduling entity is no longer eligible for running.
	 * @param entity
	 */
	void remove_from_runqueue(SchedulingEntity& entity) override
	{
		UniqueIRQLock l;
		p1.remove(&entity);
        runtimes.remove(&entity);
		runqueue.remove(&entity);
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
        UniqueIRQLock l;
        runtimes.remove(entity);
    }

    /**
     * Called every time a scheduling event occurs, to cause the next eligible entity
     * to be chosen.  The next eligible entity might actually be the same entity, if
     * e.g. its timeslice has not expired.
     */
    SchedulingEntity *pick_next_entity() override
    {
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
            auto test = SchedulingEntityPriority::REALTIME;
            auto st = SchedulingEntityState::RUNNABLE;
            for (const auto& entity : runqueue) {
                syslog.messagef(LogLevel::FATAL, "runqueue count - %s", ToString(runqueue.count()).c_str());
                if (p1.count() == 0){
                    sched_log.messagef(LogLevel::DEBUG, "first step correct");
                }
                syslog.messagef(LogLevel::FATAL, "priority level - %s", ToString(entity->priority()).c_str());
                if (entity->priority() == 2){
                    sched_log.messagef(LogLevel::FATAL, "HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
                }
                if (entity->priority()) {
                    if (entity->state() == st){
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

        
        


        while (true && p1.count() > 0) {

            // get entity at pointer
            auto ent = MultipleQueuePriorityScheduler::get_at(q_pointer);
            // syslog.messagef(LogLevel::DEBUG, "name - %s", ent->name().c_str());
            syslog.messagef(LogLevel::FATAL, "p1 count - %s", ToString(p1.count()).c_str());

            // get entities runtime
            auto now = ent->cpu_runtime();

            // get runtime before last iteration
            SchedulingEntity::EntityRuntime val;
            if (runtimes.try_get_value(ent, val)){
                // syslog.messagef(LogLevel::DEBUG, "in map");
                // in map
                // syslog.messagef(LogLevel::DEBUG, "remove from runqueue - %s", ent->name().c_str());
                
                // if entity has ran over quantum
                // syslog.messagef(LogLevel::FATAL, "runtime - %s", ToString(now).c_str());
                if (now - val > 200000000) {
                    syslog.messagef(LogLevel::FATAL, "time over");
                    // sched_log.messagef(LogLevel::DEBUG, "AYO");

                    // remove from map
                    MultipleQueuePriorityScheduler::rm_from_map(ent);

                    //remove from p1
                    MultipleQueuePriorityScheduler::rm_from_p1(ent);


                    // MultipleQueuePriorityScheduler::add_to_map(ent, now);

                    // increment pointer
                    q_pointer = (q_pointer + 1) % p1.count();

                    // begin loop again
                } else {

                    //  if entity not exceeded quantum
                    sched_log.messagef(LogLevel::DEBUG, "return");
                     
                    // return entitys
                    return ent;
                }


                
            } else {
                // not in map

                // add entity to map
                MultipleQueuePriorityScheduler::add_to_map(ent, now);

                sched_log.messagef(LogLevel::DEBUG, "return");
                // return entity
                return ent;
            }
           
            // if (p1.count() == 0) {return NULL;}
    

        }
        






       

























        // sched_log.messagef(LogLevel::DEBUG, "intersting");
        sched_log.messagef(LogLevel::DEBUG, "unhandled except");
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