#include <iostream>
#include "queue"
#include "thread"
#include "condition_variable"

using namespace std;

std::mutex m;
std::condition_variable adminCV, adderCV;
std::queue<int> taskQueue;
int var = 0;
int readers_count;

void admin_f(){
    std::unique_lock<std::mutex> l{m};
    var = 10;
    taskQueue.push(var);

    while(true) {
        if (!taskQueue.empty()) {
            l.unlock();

            l.lock();
            if(readers_count == 3 || var > 15) {
                int task =	taskQueue.back();
                cout << "==== Last Value " << task << "====\n" << "Number of Thread " <<  readers_count << endl;
                return;
            }
        }
        if(readers_count == 3 || var > 15)
            adminCV.wait(l);
    }
}
void adder_f() {
    std::unique_lock<std::mutex> l{m};
    var += rand() % 5;

    taskQueue.push(var);

    readers_count++;

    l.unlock();
    adderCV.notify_one();
}

int main() {

    std::vector<std::thread> adders;
    std::thread admin(admin_f);
    readers_count = 0;

    for(int i = 0; i < 3; i++){
        srand((unsigned)time(NULL)); //makes seed different for calling rand()
        adders.emplace_back(std::thread( adder_f));
    }

    for(auto& i : adders) {
        i.join();
    }

    adminCV.notify_one();
    admin.join();
    return 0;
}

