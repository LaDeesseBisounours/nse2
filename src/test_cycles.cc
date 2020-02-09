#include <cerrno>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>
#include <x86intrin.h>

#define NB_TEST 1000000

// optional wrapper if you don't want to just use __rdtsc() everywhere
inline
uint64_t readTSC() {
    // _mm_lfence();  // optionally wait for earlier insns to retire before reading the clock
    uint64_t tsc = __rdtsc();
    // _mm_lfence();  // optionally block later instructions until rdtsc retires
    return tsc;
}

inline
void CPUID(uint32_t regs[4], unsigned i) {
    asm volatile
        ("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
         : "a" (i), "c" (0));
}

std::mutex _mut;
void computation(uint64_t *res) {
    _mut.lock();

    // CALCULATE CYCLES FOR 1000MS
    const uint64_t timestamp_calibrator = readTSC();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    const uint64_t timestamp_calibration = readTSC() - timestamp_calibrator;

    // TIME CPUID
    uint64_t total_time = 0;
    for (std::size_t count = 0; count < NB_TEST; count++)
    {
        // SAVE PRE CPUID TIME
        const auto timestamp_pre = readTSC();

        std::uint32_t cpuid_data[4] = {};
        CPUID(cpuid_data, 0);

        // SAVE THE DELTA
        total_time += readTSC() - timestamp_pre;
    }

    _mut.unlock();
    std::cout << "timestamps calibration = " << timestamp_calibration << std::endl;
    std::cout << "total_cycles = " << total_time << std::endl;

    *res = 1000000 * total_time / timestamp_calibration;
}

int main(void) {

    _mut.lock();

    uint64_t res;
    std::thread t(computation, &res);
    sched_param sch;
    int policy;

    pthread_getschedparam(t.native_handle(), &policy, &sch);
    sch.sched_priority = 0;
    if (pthread_setschedparam(t.native_handle(), policy, &sch)) {
        std::cout << "Failed to setschedparam: " << std::strerror(errno) << '\n';
    }

    _mut.unlock();
    t.join();

    std::cout << "time for total execution " << res << " µs" << std::endl;
    std::cout << "time for one execution " << float(res) / float(NB_TEST)
        << " µs" << std::endl;

    return 0;
}
