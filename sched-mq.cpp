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
        if (MultipleQueuePriorityScheduler::in_list(entity, p2)){
            p2.remove(&entity);
        }
        if (MultipleQueuePriorityScheduler::in_list(entity, p3)){
            p3.remove(&entity);
        }
        if (MultipleQueuePriorityScheduler::in_list(entity, p4)){
            p4.remove(&entity);
        }

        if (MultipleQueuePriorityScheduler::in_list(entity, q)){
            q.remove(&entity);
            if (q.empty()){
                q_pointer = 0;
            } else {
                q_pointer = (q_pointer - 1) % q.count();
            }
        }

        // if (q_pointer >= p1.count()) {
        //     q_pointer--;
        // }

        // decrement pointer


        

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
    void add_to_p2(SchedulingEntity* entity)
    {
        UniqueIRQLock l;
        if (!MultipleQueuePriorityScheduler::in_list2(entity, p2)){
            p2.append(entity);
        }
    }
    void add_to_p3(SchedulingEntity* entity)
    {
        UniqueIRQLock l;
        if (!MultipleQueuePriorityScheduler::in_list2(entity, p3)){
            p3.append(entity);
        }
    }
    void add_to_p4(SchedulingEntity* entity)
    {
        UniqueIRQLock l;
        if (!MultipleQueuePriorityScheduler::in_list2(entity, p4)){
            p4.append(entity);
        }
    }


    void rm_from_p1(SchedulingEntity* entity)
    {
        if (p1.count() != 0){
            UniqueIRQLock l;
            p1.remove(entity);
        }
    }
    void rm_from_p2(SchedulingEntity* entity)
    {
        if (p2.count() != 0){
            UniqueIRQLock l;
            p2.remove(entity);
        }
    }
    void rm_from_p3(SchedulingEntity* entity)
    {
        if (p3.count() != 0){
            UniqueIRQLock l;
            p3.remove(entity);
        }
    }
    void rm_from_p4(SchedulingEntity* entity)
    {
        if (p4.count() != 0){
            UniqueIRQLock l;
            p4.remove(entity);
        }
    }

    SchedulingEntity* get_at(int ptr)
    {
        UniqueIRQLock l;
        return q.at(ptr);
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




    void get_top_queue() {
        UniqueIRQLock l;
        if (!p1.empty()) {
            transfer(1);
            p1.clear();
        }
        else if (!p2.empty()) {
            transfer(2);
            p2.clear();
        }
        else if (!p3.empty()) {
            sched_log.messagef(LogLevel::DEBUG, "yes");
            transfer(3);
            p3.clear();
        }
        else if (!p4.empty()) {
            transfer(4);
            p4.clear();
        }
    }

    void transfer(int n){
        UniqueIRQLock l;
        sched_log.messagef(LogLevel::DEBUG, "ash %s", ToString(p3.count()).c_str());

        if (n==1){
            for (const auto& ent : p1) {
                if (!MultipleQueuePriorityScheduler::in_list2(ent, q)){
                    sched_log.messagef(LogLevel::DEBUG, "yes2");
                    q.append(ent);
                }
            }
        }
        if (n==2){
            for (const auto& ent : p2) {
                if (!MultipleQueuePriorityScheduler::in_list2(ent, q)){
                    sched_log.messagef(LogLevel::DEBUG, "yes2");
                    q.append(ent);
                }
            }
        }
        if (n==3){
            for (const auto& ent : p3) {
                if (!MultipleQueuePriorityScheduler::in_list2(ent, q)){
                    sched_log.messagef(LogLevel::DEBUG, "yes2");
                    q.append(ent);
                }
            }
        }
        if (n==4){
            for (const auto& ent : p4) {
                if (!MultipleQueuePriorityScheduler::in_list2(ent, q)){
                    sched_log.messagef(LogLevel::DEBUG, "yes2");
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
       


        if (begin) {
            sched_log.messagef(LogLevel::DEBUG, "begin");

            for (const auto& entity : runqueue) {

                syslog.messagef(LogLevel::FATAL, "runqueue count - %s", ToString(runqueue.count()).c_str());
                syslog.messagef(LogLevel::FATAL, "priority level - %s", ToString(entity->priority()).c_str());

                if (entity->state() == 2) {
                    syslog.messagef(LogLevel::FATAL, "state - %s", ToString(entity->state()).c_str());

                    if (entity->priority() == 0){
                        MultipleQueuePriorityScheduler::add_to_p1(entity);
                    }
                    if (entity->priority() == 1){
                        MultipleQueuePriorityScheduler::add_to_p2(entity);
                    }
                    if (entity->priority() == 2){
                        MultipleQueuePriorityScheduler::add_to_p3(entity);
                    }
                    if (entity->priority() == 3){
                        MultipleQueuePriorityScheduler::add_to_p4(entity);
                    }

                }

                
            }

            begin = false;
            get_top_queue();
        }

        // check for runnable entities that have changed state (previously not runnable)
        if (runqueue.count() != q.count()) {
            for (const auto& entity : runqueue){
                if (!MultipleQueuePriorityScheduler::in_list2(entity, q) && entity->state() == 2){
                    // MultipleQueuePriorityScheduler::add_to_p1(entity);
                    begin = true;
                }
            }
        }


        // =======================================
        // Round-Robin                           |
        // =======================================

        
    // syslog.messagef(LogLevel::DEBUG, "p1 count - %s", ToString(p1.count()).c_str());
    // syslog.messagef(LogLevel::DEBUG, "q count - %s", ToString(q.count()).c_str());
    // syslog.messagef(LogLevel::DEBUG, "rq count - %s", ToString(runqueue.first()->state()).c_str());
    // return NULL;
        // q
        while (!q.empty()) {

            if (q.count() == 1) {return q.first();}

            syslog.messagef(LogLevel::DEBUG, "p1 count - %s", ToString(q.count()).c_str());
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
                    if (q.count() != 1) {
                        syslog.messagef(LogLevel::DEBUG, "B");
                        q_pointer = (q_pointer + 1) % q.count();
                    } 


                }
                 else {
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
        // sched_log.messagef(LogLevel::DEBUG, "doing something with runqueue of size - %s", ToString(runqueue.count()).c_str());
        // sched_log.messagef(LogLevel::DEBUG, "and p1 size - %s", ToString(p1.count()).c_str());
        
        if (q.empty() && p1.empty() && p2.empty() && p3.empty() && p4.empty()){
            return NULL;
        } else {
            begin = true;
        }
        































				
        
    }
private:
    List<SchedulingEntity *> runqueue;
    bool begin = true;
    int q_pointer = 0;

    Map<SchedulingEntity *, SchedulingEntity::EntityRuntime> runtimes;

    // highest prio
    List<SchedulingEntity *> p1;
    List<SchedulingEntity *> p2;
    List<SchedulingEntity *> p3;
    List<SchedulingEntity *> p4;
    // lowest prio
    
    // holder
    List<SchedulingEntity *> q;
};

/* --- DO NOT CHANGE ANYTHING BELOW THIS LINE --- */

RegisterScheduler(MultipleQueuePriorityScheduler);










// hi future michael


// before you left you were trying to add increments of quantums as map values

// this would then be used to make sure that the cpu is shared fairly