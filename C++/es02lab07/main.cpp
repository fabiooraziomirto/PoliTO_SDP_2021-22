#include <iostream>
#include "queue"
#include "thread"
#include "condition_variable"
#include <future>

using namespace std;
int num;



void take_value_f(const int &N, promise<int>& pr){
    num = N;
    pr.set_value(num);
}

bool check_if_prime_f(){
    if (num <= 1){
        return false;
    }
    for(int i = 2; i < num; i++){
        if(num % i == 0){

            return false;
        }
    }

    return true;
}

void give_answer_f(bool res){
    if(res){
        cout << num << " is prime" << endl;
    } else {
        cout << num << " is NOT prime" << endl;
    }

}

int main(int argc, char **argv) {
    promise<int> p;
    future<int> f = p.get_future();
    thread take_value = thread(take_value_f, atoi(argv[1]), ref(p));
    int n = f.get();
    cout << "Result is : " << n << endl;
    take_value.join();

    std::future<bool> fut = std::async(std::launch::async, check_if_prime_f);

    bool r = fut.get();

    thread give_answer(give_answer_f, r);
    give_answer.join();


    return 0;
}
