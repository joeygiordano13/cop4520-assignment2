#include <atomic>
#include <vector>
#include <thread>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <array>

#define NUM_GUESTS 25

enum RoomState {
    available = true;
    busy = false;
};

// SpinLock class
class SpinLock {
    private:
        std::atomic<bool> locked{false};
    public:
        // While a lock is taken the loop keeps spinning
        // until it is unlocked by another thread, else
        // if the lock is free, if is taken by the current
        // spinning thread and set to true (taken).
        void lock() {
            while (locked.exchange(true));
        }
        // Unlocking by setting the 
        // locked value to false.
        void unlock() {
            locked.store(false);
        }
};

// Make spinlock and counts.
SpinLock sl;
int count = 0; 
// Track active threads.
int active_thread_id = -1;
// Track which guests have looked at vase.
std::vector<bool> cupcakeEaten(NUM_GUESTS);

void LookAtMinotaursVase(int id) {

    // TODO: add new code here.
    // while (count < NUM_GUESTS) {
    //     sl.lock();
    //     if (id == active_thread_id) {
    //         // If guest is the leader, 
    //         // it will check the cupcake
    //         if (active_thread_id == LEADER) {
    //             if (cupcakePlaced && !cupcakeEaten[active_thread_id]) {
    //                 cupcakePlaced = false; // eat cupcake
    //                 cupcakeEaten[active_thread_id] = true;
    //                 std::cout << "Leader ate a cupcake." << std::endl;
    //             }
    //             else if (!cupcakePlaced) {
    //                 count++;
    //                 cupcakePlaced = true;
    //                 std::cout << "The leader incremented the number of guests to " << count << "." << std::endl;

    //             }
    //         }
    //         // Regular guest exits labyrinth.
    //         else {
    //             if (cupcakePlaced && !cupcakeEaten[active_thread_id]) {
    //                 cupcakePlaced = false; // eat cupcake
    //                 cupcakeEaten[active_thread_id] = true;
    //                 std::cout << "Guest " << active_thread_id << "ate a cupcake." << std::endl;
    //             }
    //         }
    //     }
    //     sl.unlock();
    // }
}

// Problem 2: 
// Choice 2) The Minotaur’s second strategy allowed the guests to place a sign on the door 
// indicating when the showroom is available. The sign would read “AVAILABLE” or “BUSY.” Every guest 
// is responsible to set the sign to “BUSY” when entering the showroom and back to “AVAILABLE” upon exit. 
// That way guests would not bother trying to go to the showroom if it is not available.

// A: Of the three choices this is the ideal strategy because it doesn't require that guests simply wait at
// the door in a queue or have the thread become inactive/off/sleeping.

int main() {
    // Allocate threads
    std::vector<std::thread> threads;
    threads.reserve(NUM_GUESTS);
    int id = -1;

    int tStart, tEnd;
    tStart = clock();

    // Start threads
    for (int i = 0; i < NUM_GUESTS; i++) {
        threads[i] = std::thread(LabyrinthRun, i);
    }

    // Minotaur requests a random guest, which activates a thread.
    while (count < NUM_GUESTS) {
        active_thread_id = rand() % NUM_GUESTS; // random guest
    }

    // Join threads
    for (auto &cur_thread : threads) {
        cur_thread.join();
    }

    tEnd = clock();
    std::cout << "execution time: " << (tEnd - tStart)/double(CLOCKS_PER_SEC) << std::endl;
    // Clear threads
    threads.clear();
    return 0;
}
