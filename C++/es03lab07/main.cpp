#include <iostream>
#include "queue"
#include "thread"
#include "condition_variable"

using namespace std;

std::mutex m;
std::condition_variable adminCV, adderCV;
std::queue<thread::id> parkQueue;
int size = 2;
int tot_simulation = 15;
int time_simulation = 0;
int readers_count;

void admin_f(){
    std::unique_lock<std::mutex> l{m};

    while(true) {
        if (parkQueue.size() >= size) {
            int time = (rand() % 4) + 4;
            std::this_thread::sleep_for(std::chrono::seconds(time));
            time_simulation += time;

            thread::id task = parkQueue.front();
            parkQueue.pop();
            cout << "==== Car out the parking " << task << "====" <<  endl;
            l.unlock();

            l.lock();
            if(parkQueue.empty()) {
                cout << endl <<  "**** No elements in the queue, no further readers, leaving program ****" << endl;
                return;
            }
        }
        if(time_simulation == tot_simulation || parkQueue.size() == size)
            adminCV.wait(l);
    }
}
void parking_f() {
    std::unique_lock<std::mutex> l{m};

    int time = (rand() % 2) + 1;
    std::this_thread::sleep_for(std::chrono::seconds(time));

    parkQueue.push(this_thread::get_id());

    time_simulation += time;
    cout << "==== Car inside the parking " << this_thread::get_id() << "====" <<  endl;
    l.unlock();
    adderCV.notify_one();
}

int main() {

    std::vector<std::thread> cars;
    std::thread admin(admin_f);

    for(int i = 0; i < 6; i++){
        srand((unsigned)time(NULL)); //makes seed different for calling rand()
        cars.emplace_back(std::thread( parking_f));
    }

    for(auto& i : cars) {
        i.join();
    }

    adminCV.notify_one();
    admin.join();
    return 0;
}