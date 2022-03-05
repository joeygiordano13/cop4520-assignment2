#include <atomic>
#include <thread>
#include <iostream>
#include <array>
#include <chrono>

#define NUM_GUESTS 25

enum State { available, busy };

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
// Track which guests have looked at vase.
std::array<bool, NUM_GUESTS> ponderedVase{};
State vaseState = State::available;

void PonderMinotaursVase(int id) {
    // Allow for a guest to potentially enter room multiple times.
    while (count < NUM_GUESTS) {
        sl.lock();
        if (vaseState == State::available) {
            if (!ponderedVase[id]) {
                vaseState = State::busy;
                std::cout << "Guest ID: " << id << " is pondering the vase." << std::endl;
                // Stop the thread to allow guest up to 500ms to ponder vase.
                std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 500));
                // Make vase available again.
                ponderedVase[id] = true;
                vaseState = State::available;
                // Number of guests who have seen vase.
                count++;
            }
            else {
                vaseState = State::busy;
                std::cout << "Guest ID: " << id << " is pondering the vase, again...!" << std::endl;
                // Give the repeat-viewers less time to stare at vase. 
                std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));
                // Make vase available again, don't update count.
                vaseState = State::available;
            }
        }
        sl.unlock();
    }


}

// Problem 2: 
// Choice 2) The Minotaur’s second strategy allowed the guests to place a sign on the door 
// indicating when the showroom is available. The sign would read “AVAILABLE” or “BUSY.” Every guest 
// is responsible to set the sign to “BUSY” when entering the showroom and back to “AVAILABLE” upon exit. 
// That way guests would not bother trying to go to the showroom if it is not available.
int main() {
    // Allocate threads
    std::array<std::thread, NUM_GUESTS> threads{};

    int tStart, tEnd;
    tStart = clock();

    // Start threads
    for (size_t i = 0; i < NUM_GUESTS; i++) {
        threads[i] = std::thread(PonderMinotaursVase, i);
    }

    // Join threads
    for (auto &cur_thread : threads) {
        cur_thread.join();
    }

    tEnd = clock();
    std::cout << "All guests have viewed the Minotaur's vase! Count " << count << "." << std::endl;
    std::cout << "execution time: " << (tEnd - tStart)/double(CLOCKS_PER_SEC) << std::endl;
    return 0;
}