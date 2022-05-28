#include "condition_variable"
#include "iostream"
#include "queue"
#include "thread"


using namespace std;

mutex m;
condition_variable cv;
queue<thread::id> parkQueue;
bool lastreader;
int const NREADERS = 400;
int readers_count;

void pushWork() {
    int time = (rand() % 200) + 100;
    this_thread::sleep_for(chrono::milliseconds(time));

    std::unique_lock<std::mutex> l{m};
    thread::id task = this_thread::get_id();
    parkQueue.push(task);
    cout << "Pushed " << task << endl;

    readers_count += time;
    if(readers_count > NREADERS)
        lastreader=true;

    l.unlock();
    cv.notify_one();
}

void workerThread() {


    std::unique_lock<std::mutex> l{m};
    while(true) {

        if (!parkQueue.empty())
        {
            int time = (rand() % 200) + 100;
            this_thread::sleep_for(chrono::milliseconds(time));
            readers_count += time;
            if(readers_count > NREADERS)
                lastreader=true;
            thread::id task = parkQueue.front();
            parkQueue.pop();
            cout << "==== Worked " << task << "====" <<  endl;
            l.unlock();

            l.lock();
            if(parkQueue.empty() && lastreader) {
                cout << endl <<  "**** No elements in the queue, no further readers, leaving program ****" << endl;
                return;
            }
        }
        if(!lastreader)
            cv.wait(l);
    }
}

int main(){

    thread worker(workerThread);

    vector<thread> threadPool;
    lastreader=false;
    readers_count=0;

    for (int i = 1; i <= 9; ++i) {
        if(readers_count < NREADERS)
            threadPool.emplace_back(std::thread(pushWork));

    }
    for (auto& t : threadPool) {
        t.join();
    }
    worker.join();

}