#include <iostream>
#include <thread>
//#include "SceneManager/SceneManager.hpp"
#include "ComponentArray/ComponentArray.hpp"
#include "ISystem/ISystem.hpp"
#include "Task/TaskManager.hpp"
#include "Task/Schedulers/Pipeline/Pipeline.hpp"
std::atomic<int> shared_i = 0;
using namespace MultiStation;
TaskManager* g_taskManager = nullptr;
class MyTask : public Task {

public:
	void Run(uint16_t threadID) override {
		std::cout << "Task running on thread " << threadID << std::endl;
		int i = shared_i.load(std::memory_order_acquire);
		if ( (i >= 24) && !threadID) {
			std::cout << "Stopping Task Manager from thread " << threadID << std::endl;
			g_taskManager->Stop();
		}
	}

};

class MyParrallelTask : public Task {
public:
	void Run(uint16_t threadID) override {
		int i = shared_i.load(std::memory_order_acquire);
		shared_i.store(i + 1, std::memory_order_release);
		std::cout << "Parallel Task running on thread " << threadID << " , i = " << i << std::endl;
		std::cout.flush();
	}
};

int main() {
    
	Pipeline* pipeline = new Pipeline();
	IPipelineTaskMetadata* meta1 = new IPipelineTaskMetadata();
	meta1->stage = 0;
	meta1->canRunInParallel = false;

	IPipelineTaskMetadata* meta2 = new IPipelineTaskMetadata();
	meta2->stage = 0;
	meta2->canRunInParallel = true;

	MyParrallelTask* parallelTask = new MyParrallelTask();
	MyTask* task = new MyTask();

	g_taskManager = new TaskManager(pipeline, 4);
	
	g_taskManager->AddTask(parallelTask, meta2);
	g_taskManager->AddTask(task, meta1);
	g_taskManager->AddTask(task, meta1);
	g_taskManager->AddTask(task, meta1);
	g_taskManager->AddTask(task, meta1);

	g_taskManager->Start(true);
	g_taskManager->JoinAll();
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	shared_i.store(0, std::memory_order_release);
	g_taskManager->Start(true);
	g_taskManager->JoinAll();
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	shared_i.store(0, std::memory_order_release);
	g_taskManager->RemoveTask(parallelTask);
	g_taskManager->Start(false);
	std::this_thread::sleep_for(std::chrono::seconds(20));
	g_taskManager->Stop();
	g_taskManager->JoinAll();

	// Release resources
	delete g_taskManager;
	delete pipeline;
	delete meta1;
	delete meta2;
	delete parallelTask;
	delete task;

    return 0;
}
