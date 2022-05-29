#include "condition_variable"
#include "iostream"
#include "queue"
#include "thread"


using namespace std;

mutex m;
condition_variable cv;
queue<thread::id> parkQueue;
bool lastreader;
int size = 4;
int const NREADERS = 5000;
int readers_count;

void pushWork() {
    std::unique_lock<std::mutex> l{m};
    int time = (rand() % 200) + 100;
    this_thread::sleep_for(chrono::milliseconds(time));
    thread::id task = this_thread::get_id();

    if((readers_count + time) > NREADERS)
        lastreader=true;
    else if(parkQueue.size() < size){
        readers_count += time;
        parkQueue.push(task);
        cout << "Pushed " << task << " size " << parkQueue.size() << endl;
        cout << "During simulation on the push thread  " << readers_count << endl;
        cv.notify_one();
    } else {
        l.unlock();
        cv.notify_one();
    }

}

void workerThread() {
    std::unique_lock<std::mutex> l{m};
    while(true) {
        if (!parkQueue.empty()) {
            int time = (rand() % 400) + 400;
            this_thread::sleep_for(chrono::milliseconds(time));
            if((readers_count + time) > NREADERS)
                lastreader=true;
            else {
                readers_count += time;
                thread::id task = parkQueue.front();
                parkQueue.pop();
                cout << "==== Pop out " << task << " size " << parkQueue.size() << endl;
                cout << "During simulation on the working thread " << readers_count << endl;

            }
            l.unlock();

            l.lock();
            if(lastreader) {
                cout << endl <<  "**** No elements in the queue, no further readers, leaving program ****" << endl;
                return;
            }
        }
        if(!lastreader || parkQueue.size() == size)
            cv.wait(l);
    }

}

int main(){
    lastreader=false;
    readers_count=0;
    thread worker(workerThread);
    vector<thread> threadPool;

    for (int i = 1; i <= 15; ++i) {
        if(readers_count < NREADERS)
            threadPool.emplace_back(std::thread(pushWork));
    }
    for (auto& t : threadPool) {
        t.join();
    }

    worker.join();

}