#include <atomic>
#include <vector>
#include <thread>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <array>

#define LEADER 1
#define NUM_GUESTS 25


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
// Flag for whether cupcake is placed.
bool cupcakePlaced = true;
// Track active threads.
int active_thread_id = -1;
// Track which guests have eaten cupcake.
std::vector<bool> cupcakeEaten(NUM_GUESTS);

// NO CUPCAKE = LIGHT OFF
// CUPCAKE THERE = LIGHT ON
void LabyrinthRun(int id) {
    while (count < NUM_GUESTS) {
        sl.lock();
        if (id == active_thread_id) {
            // If guest is the leader, 
            // it will check the cupcake
            if (active_thread_id == LEADER) {
                if (cupcakePlaced && !cupcakeEaten[active_thread_id]) {
                    cupcakePlaced = false; // eat cupcake
                    cupcakeEaten[active_thread_id] = true;
                    std::cout << "Leader ate a cupcake." << std::endl;
                }
                else if (!cupcakePlaced) {
                    count++;
                    cupcakePlaced = true;
                    std::cout << "The leader incremented the number of guests to " << count << "." << std::endl;

                }
            }
            // Regular guest exits labyrinth.
            else {
                if (cupcakePlaced && !cupcakeEaten[active_thread_id]) {
                    cupcakePlaced = false; // eat cupcake
                    cupcakeEaten[active_thread_id] = true;
                    std::cout << "Guest " << active_thread_id << "ate a cupcake." << std::endl;
                }
            }
        }
        sl.unlock();
    }
}

// Problem 1: Minotaur’s Birthday Party (50 points)
// Create a program to simulate the winning strategy (protocol) where each guest is represented by one running thread. 
// In your program you can choose a concrete number for N or ask the user to specify N at the start.
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
