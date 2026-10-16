#include "IO.hpp"
#include "IOManager.hpp"

#include <thread>
#include <atomic>
#include <chrono>

namespace MultiStation
{
    namespace IO
    {
        
        #define s_manager IOManager::Get()
        static std::thread s_ioThread;
        static std::atomic_bool s_running = false;


        // ---------------------------------------------------------
        // Internal
        // ---------------------------------------------------------

        


        static void WorkerLoop()
        {
            while (s_running.load())
            {
                /*
                 * UpdateIO currently processes one request.
                 *
                 * Later you can replace this tiny sleep with:
                 * condition_variable / semaphore / blocking queue.
                 */
                s_manager.UpdateIO();

                std::this_thread::sleep_for(
                    std::chrono::milliseconds(1)
                );
            }
        }


        // ---------------------------------------------------------
        // Lifetime
        // ---------------------------------------------------------

        bool Init(void)
        {
            if (s_running.load())
                return false;

            s_running.store(true);

            try
            {
                s_ioThread =
                    std::thread(WorkerLoop);
            }
            catch (...)
            {
                s_running.store(false);
                return false;
            }

            return true;
        }


        void Fini(void)
        {
            if (!s_running.load())
                return;

            s_running.store(false);

            if (s_ioThread.joinable())
                s_ioThread.join();

            
            
            s_manager.Fini();
            
        }

        
        void PollCallbacks(void)
        {
            
            std::shared_ptr<Result> result;
            while (s_manager.PopResult(
                result))
            {
                if (result->callback)
                    result->callback(*result);
            }
        }


        
    }
}