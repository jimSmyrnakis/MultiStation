

#include "../../JobSystem/JobSystem.hpp"
#include <iostream>
#include <atomic>
#include <memory>
#include <thread>
#include <cmath>
MultiStation::JobSystem* js;
// ------------------------------------------------------------
// 2-level nested fork-join test
// Parent -> ParallelFor(children) -> WaitFor
// Child  -> ParallelFor(grand)    -> WaitFor
// ------------------------------------------------------------

struct GrandData {
    std::atomic<uint64_t>* grandDone;
    uint32_t iters;
};

struct ChildData {
    MultiStation::JobSystem* js;
    std::atomic<uint64_t>* childrenDone;
    std::atomic<uint64_t>* grandDone;
    uint32_t grandJobs;
    uint32_t iters;
};

struct ParentData {
    MultiStation::JobSystem* js;
    std::atomic<uint64_t>* parentsDone;
    std::atomic<uint64_t>* childrenDone;
    std::atomic<uint64_t>* grandDone;
    uint32_t childJobs;
    uint32_t grandJobs;
    uint32_t iters;
};

// capture-less funcs (good for function-pointer JobFunction)
static void GrandJob(MultiStation::Job job) {
   // (void)workerID; (void)id; (void)blockID; (void)size;
    auto* gd = static_cast<GrandData*>(job.data);

    // μικρό deterministic compute ώστε να μην το “σβήσει” ο compiler
    uint64_t x = 0;
    for (uint32_t i = 0; i < gd->iters; ++i) x += (i * 2654435761u) ^ (x >> 1);

    gd->grandDone->fetch_add(1, std::memory_order_relaxed);
    
    
}

static void ChildJob(MultiStation::Job job) {
    //(void)workerID; (void)id; (void)blockID; (void)size;
    auto* cd = static_cast<ChildData*>(job.data);

    // spawn grand-children and wait (nested wait inside a worker job)
    auto grandCounter = std::make_shared<std::atomic<uint32_t>>(0);

    GrandData gd;
    gd.grandDone = cd->grandDone;
    gd.iters = cd->iters;

    cd->js->ParallelFor(GrandJob, &gd, cd->grandJobs, grandCounter);
    cd->js->WaitFor(*grandCounter);

    cd->childrenDone->fetch_add(1, std::memory_order_relaxed);
}

static void ParentJob(MultiStation::Job job) {
    //(void)workerID; (void)id; (void)blockID; (void)size;
    auto* pd = static_cast<ParentData*>(job.data);

    // spawn children and wait (nested wait inside a worker job)
    auto childCounter = std::make_shared<std::atomic<uint32_t>>(0);

    ChildData cd;
    cd.js = pd->js;
    cd.childrenDone = pd->childrenDone;
    cd.grandDone = pd->grandDone;
    cd.grandJobs = pd->grandJobs;
    cd.iters = pd->iters;

    pd->js->ParallelFor(ChildJob, &cd, pd->childJobs, childCounter);
    pd->js->WaitFor(*childCounter);

    pd->parentsDone->fetch_add(1, std::memory_order_relaxed);
}

int TestJobSystemNested() {
    // λίγα threads, όχι 2.5x, γιατί σε βαριά nested tests το oversubscription θολώνει τα συμπεράσματα
    uint32_t threadsNum = std::max(1u, std::thread::hardware_concurrency());
    js = new MultiStation::JobSystem(threadsNum);

    // parameters
    const uint32_t P = 64;   // parent jobs
    const uint32_t C = 16;    // children per parent
    const uint32_t G = 16 ;    // grand per child
    const uint32_t iters = 8; // compute per job (ρύθμισέ το αν θες πιο βαρύ test)

    std::atomic<uint64_t> parentsDone{ 0 };
    std::atomic<uint64_t> childrenDone{ 0 };
    std::atomic<uint64_t> grandDone{ 0 };

    auto parentCounter = std::make_shared<std::atomic<uint32_t>>(0);

    ParentData pd;
    pd.js = js;
    pd.parentsDone = &parentsDone;
    pd.childrenDone = &childrenDone;
    pd.grandDone = &grandDone;
    pd.childJobs = C;
    pd.grandJobs = G;
    pd.iters = iters;

    // Top-level spawn
    js->ParallelFor(ParentJob, &pd, P, parentCounter);

    // Wait from main (should "help")
    js->WaitForAll();

    // Validate counts
    const uint64_t expectedParents = P;
    const uint64_t expectedChildren = uint64_t(P) * uint64_t(C);
    const uint64_t expectedGrand = uint64_t(P) * uint64_t(C) * uint64_t(G);

    std::cout << "threads = " << threadsNum << "\n";
    std::cout << "parentsDone  = " << parentsDone.load() << " / " << expectedParents << "\n";
    std::cout << "childrenDone = " << childrenDone.load() << " / " << expectedChildren << "\n";
    std::cout << "grandDone    = " << grandDone.load() << " / " << expectedGrand << "\n";

    const bool ok =
        parentsDone.load() == expectedParents &&
        childrenDone.load() == expectedChildren &&
        grandDone.load() == expectedGrand;

    std::cout << (ok ? "[OK] Nested fork-join works.\n" : "[FAIL] Wrong counts (likely scheduling / counter / TLS issue).\n");

    js->Shutdown();
    std::cin.get();
    delete js;
    return ok ? 0 : 1;
}
