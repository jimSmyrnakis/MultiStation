#include <iostream>
#include <thread>
//#include "SceneManager/SceneManager.hpp"
#include "ComponentArray/ComponentArray.hpp"
#include "ISystem/ISystem.hpp"
#include <sstream>
using namespace MultiStation;

// Mock System Implementation
/*class TestSystem : public ISystem {
    std::string m_name;
    bool m_writable;

public:
    TestSystem(int& cnt ,const std::string& name, bool writable)
        : ISystem(writable ? WRITABLE : 0), m_name(name), m_writable(writable) , count(&cnt) {
        
        m_id = id++;
    }

    uint32_t GetFlags() const override {
        return m_writable ? WRITABLE : 0;
    }

    void OnCreate(ComponentRegistry* registry) override {
        std::cout << m_name << " created\n";
        *count = 0;
        registry->AddComponent<Transform3D>(100);
        if (m_id == 0) {
            std::string* pstr = registry->GetEntityName(100);
            (*pstr) = "Test Entity";
        }
    }

    void OnUpdate(ComponentRegistry* registry, float dt, ManagerControlBlock* controlBlock) override {
        
        float startCpuTime = Time::GetTimeInSeconds();
        std::cout << m_name << " updating ==> loop Count : " << (*count) << std::endl;
        if (m_writable) {
            (*count)++;
            registry->AddComponent<Transform3D>((*count) / 10);
            // Let the simulation run a few loops
            //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        else if (m_id == 0){
            for (int i = 0; i < ((*count) / 10); i++) {
                uint32_t components =
                    registry->GetAllComponents<Transform3D>()->GetEntityComponents(i).size();
                std::cout << "Entity " << i << " (" << *registry->GetEntityName(i) << " ) : " << std::endl;
                std::cout << "Transform3D Components count : " << components << std::endl;
            }
        }
        
        
        if (*count >= 1000) {
            controlBlock->SetState(ManagerState::DESTROY);
        }

        startCpuTime = Time::GetTimeInSeconds() - startCpuTime;
        timeCpuUsed += startCpuTime;
        totalTime += dt ;
    }

    void OnDestroy(ComponentRegistry* registry, ManagerControlBlock* controlBlock) override {
        std::stringstream sst;
        sst << m_name << " destroyed ==> loop Count : " << (*count) << std::endl ;
        sst << "Total Time : " << totalTime << "\n";
        sst << "Time Cpu Used : " << timeCpuUsed << std::endl;
        sst << "cpu Usage : " << timeCpuUsed / totalTime << " , waiting : " << 1 - (timeCpuUsed/totalTime) <<  std::endl;
        if (m_writable) {
            /*for (int i = 0; i < ((*count) / 10); i++) {
                uint32_t components =
                    registry->GetAllComponents<Transform3D>()->GetEntityComponents(i).size();
                sst << "Entity " << i << " (" << *registry->GetEntityName(i) << " ) : " << std::endl;
                sst << "Transform3D Components count : " << components << std::endl;
            }*/
        /*}

        sst.flush();

        std::cout << sst.str() << std::endl;
        

    }

    int* count ;
    static int id;
    int m_id;
    float totalTime = 0.0f;
    float timeCpuUsed = 0.0f;
};
int TestSystem::id = 0;
int cnt;*/
int main() {
    /*SceneManager* manager = SceneManager::Create();
    
    // Sync systems (2 readers, 1 writer)
    manager->AssignSyncSystem(0, new TestSystem(cnt , "SyncReader1", false));
    manager->AssignSyncSystem(0, new TestSystem(cnt , "SyncReader2", false));
    manager->AssignSyncSystem(1, new TestSystem(cnt , "SyncWriter" , true ));

    // Async systems (1 reader, 1 writer)
    manager->AssignAsyncSystem(0, new TestSystem(cnt , "AsyncReader", false));
    manager->AssignAsyncSystem(1, new TestSystem(cnt , "AsyncWriter", true));

    // Start threads / systems
    manager->StartSystems();

    // Let the simulation run a few loops
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Stop threads / cleanup
    manager->StopSystems();
    SceneManager::Destroy(manager);
*/
    return 0;
}
