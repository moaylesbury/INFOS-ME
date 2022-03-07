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
        // sched_log.messagef(LogLevel::ERROR, "REMOVEEEEEEEEEEEEEEEEEEEEEEEE");
        
        if (MultipleQueuePriorityScheduler::in_list(entity, p1)){
            
            p1.remove(&entity);
        }

        // if (q_pointer >= p1.count()) {
        //     q_pointer--;
        // }

        // decrement pointer


        if (p1.empty()){
            q_pointer = 0;
        } else {
            q_pointer = (q_pointer - 1) % p1.count();
        }

        sched_log.messagef(LogLevel::ERROR, "rmvd");
		
        if (runtimes.contains_key(&entity)){
            // sched_log.messagef(LogLevel::ERROR, "IN MAPPPPPPPPPPPPPPPPPPPPPPPP");
            // runtimes.remove(&entity);
            MultipleQueuePriorityScheduler::rm_from_map(&entity);
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

    bool in_list2(SchedulingEntity* entity, List<SchedulingEntity *> lst){
        for (auto ent : lst){
            if (ent == entity){
                return true;
            }
        }
        return false;
    }

    void add_to_p1(SchedulingEntity* entity)
    {
        UniqueIRQLock l;
        if (!MultipleQueuePriorityScheduler::in_list2(entity, p1)){
            p1.append(entity);
        }
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

    void add_to_map(SchedulingEntity* entity, SchedulingEntity::EntityRuntime val) {
        UniqueIRQLock l;
        runtimes.add(entity, val);
    }

    void rm_from_map(SchedulingEntity* entity) {
        // syslog.messagef(LogLevel::DEBUG, "removing from map");
        UniqueIRQLock l;



        Map<SchedulingEntity *, SchedulingEntity::EntityRuntime> temp;

   

        // move all not equal to entity in runtimes to temp
        for (const auto& node : runtimes) {
            if (node.key != entity){
                temp.add(node.key, node.value);
            } else {        
                // syslog.messagef(LogLevel::FATAL, "TRIGGERED =================================================");
            }
        }

        // clear runtimes
        runtimes.clear();

        // move all in temp to runtimes
        for (const auto& node : temp) {
            runtimes.add(node.key, node.value);
        }

    }

    SchedulingEntity::EntityRuntime modulo(SchedulingEntity::EntityRuntime a, SchedulingEntity::EntityRuntime b) {
        auto n = -1;
        
        SchedulingEntity::EntityRuntime big;
        SchedulingEntity::EntityRuntime small;

        if (a > b) {
            big = a;
            small = b;
        } else if (b > a) {
            big = b;
            small = a;
        } else {
            return 0;
        }

        auto r = big;

        while (r > 0) {
            r -= small;
            n++;
        }
        
        return n;
    }

    SchedulingEntity::EntityRuntime multiply(SchedulingEntity::EntityRuntime a, SchedulingEntity::EntityRuntime b) {
        SchedulingEntity::EntityRuntime x = 0;
        SchedulingEntity::EntityRuntime zero = 0;
        while (b > zero) {
            x = x + a;
            b = b - 1;
        }
        return x;
    }

    bool runtime_exceeded(SchedulingEntity::EntityRuntime runtime, SchedulingEntity::EntityRuntime hund_ms, SchedulingEntity::EntityRuntime n) {
        // is runtime > hund_ms * n?
        // doing this directly does not work. I believe this is because EntityRuntime does not hold integers large enough
        // this provides an alternative
        // syslog.messagef(LogLevel::FATAL, "runtime - %s", ToString(runtime).c_str());
        // syslog.messagef(LogLevel::FATAL, "n value - %s", ToString(n).c_str());
        // SchedulingEntity::EntityRuntime one = 1;
        // while (n != 0) {
        //     // syslog.messagef(LogLevel::FATAL, "iteration");
        //     runtime = runtime - hund_ms;
        //     n--;
        //     if (runtime < one) {
        //         syslog.messagef(LogLevel::FATAL, "RETURNING FALSE ===========================================");
        //         return false;}
        // }
        // syslog.messagef(LogLevel::FATAL, "RETURNING TRUE  ===========================================");
        // return true;

        if (runtime > multiply(hund_ms, n)) {
            syslog.messagef(LogLevel::FATAL, "RETURNING TRUE  ===========================================");
            return true;
        } else {
            syslog.messagef(LogLevel::FATAL, "RETURNING FALSE ===========================================");
            return false;
        }




    }

    void print_runtime(SchedulingEntity::EntityRuntime time) {
        int x = 0;
        SchedulingEntity::EntityRuntime zero = 0;
        SchedulingEntity::EntityRuntime one = 1;
        while (time > zero) {
            time = time - one;
            x++;
        }
        syslog.messagef(LogLevel::FATAL, "TIMER! - %s", ToString(x).c_str());
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


        if (begin) {
            sched_log.messagef(LogLevel::DEBUG, "begin");

            for (const auto& entity : runqueue) {

                syslog.messagef(LogLevel::FATAL, "runqueue count - %s", ToString(runqueue.count()).c_str());
                syslog.messagef(LogLevel::FATAL, "priority level - %s", ToString(entity->priority()).c_str());

                if (entity->priority()) {
                    syslog.messagef(LogLevel::FATAL, "state - %s", ToString(entity->state()).c_str());

                    if (entity->state() == 2){
                        MultipleQueuePriorityScheduler::add_to_p1(entity);
                    }

                }

                
            }

            begin = false;
        }

        // check for runnable entities that have changed state (previously not runnable)
        if (runqueue.count() != p1.count()) {
            for (const auto& entity : runqueue){
                if (!MultipleQueuePriorityScheduler::in_list2(entity, p1) && entity->state() == 2){
                    MultipleQueuePriorityScheduler::add_to_p1(entity);
                }
            }
        }


        // Round-Robin
        while (!p1.empty()) {

            if (p1.count() == 1) {return p1.first();}

            syslog.messagef(LogLevel::DEBUG, "p1 count - %s", ToString(p1.count()).c_str());
            syslog.messagef(LogLevel::DEBUG, "rq count - %s", ToString(runqueue.count()).c_str());
            syslog.messagef(LogLevel::DEBUG, "q_pointer - %s", ToString(q_pointer).c_str());
            syslog.messagef(LogLevel::DEBUG, "map for runtimes - %s", ToString(runtimes.count()).c_str());
            
            
            auto ent = MultipleQueuePriorityScheduler::get_at(q_pointer);
            auto now = ent->cpu_runtime();

            syslog.messagef(LogLevel::FATAL, "entity name - %s", ent->name().c_str());

            // get runtime before last iteration
            SchedulingEntity::EntityRuntime n;
            if (runtimes.try_get_value(ent, n)){
                // syslog.messagef(LogLevel::DEBUG, "n value - %s", ToString(n).c_str());
                syslog.messagef(LogLevel::DEBUG, "in map");
                // in map
                // if entity has ran over quantum

                // if (now > 100000000 * n) {
                SchedulingEntity::EntityRuntime hund_ms = 100000000; 
                // if (MultipleQueuePriorityScheduler::runtime_exceeded(now, hund_ms, n)) { 
                if (now > hund_ms){
                    
                    syslog.messagef(LogLevel::DEBUG, "A");
                    syslog.messagef(LogLevel::FATAL, "time over");

                    // update map n=number of quantums run for +1
                    MultipleQueuePriorityScheduler::rm_from_map(ent);
                    // MultipleQueuePriorityScheduler::add_to_map(ent, n+1);

                    // increment pointer
                    if (!p1.empty() && p1.count() != 1) {
                        syslog.messagef(LogLevel::DEBUG, "B");
                        q_pointer = (q_pointer + 1) % p1.count();
                    } 


                } else {
                    //  if entity not exceeded quantum
                    syslog.messagef(LogLevel::DEBUG, "C");
                    return ent;
                }


                
            } else {
                // not in map

                // add entity to map
                n = 1;
                MultipleQueuePriorityScheduler::add_to_map(ent, n);

                // return entity
                return ent;

            }
           
            

        }
        






       

























        // sched_log.messagef(LogLevel::DEBUG, "intersting");
        sched_log.messagef(LogLevel::DEBUG, "doing something with runqueue of size - %s", ToString(runqueue.count()).c_str());
        sched_log.messagef(LogLevel::DEBUG, "and p1 size - %s", ToString(p1.count()).c_str());
        return NULL;































				
        
    }
private:
    List<SchedulingEntity *> runqueue;
    bool begin = true;
    int q_pointer = 0;

    Map<SchedulingEntity *, SchedulingEntity::EntityRuntime> runtimes;

    List<SchedulingEntity *> p1;
};

/* --- DO NOT CHANGE ANYTHING BELOW THIS LINE --- */

RegisterScheduler(MultipleQueuePriorityScheduler);










// hi future michael


// before you left you were trying to add increments of quantums as map values

// this would then be used to make sure that the cpu is shared fairly

