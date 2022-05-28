#include <iostream>
#include <future>

using namespace std;

// factorial version 1
void factorial(const int &N, promise<int>& pr) {
    int res = 1;
    for ( int i=N; i> 1; i-- )
        res *=i;
    pr.set_value(res);
}

// factorial_v2 version 2
int factorial_v2(std::future<int>& f ) {
    int res = 1;
    int N = f.get();
    for ( int i=N; i> 1; i-- )
        res *=i;
    return res;
}


//factorial version 3
int factorial_v3( shared_future<int> f ) {
    int res = 1;
    int N = f.get();
    for ( int i=N; i> 1; i--)
        res *=i;
    return res;
}


// function to check if a number is prime
bool is_prime (int n)  {

        if (n <= 1){
            std::cout << n << " is NOT prime" << std::endl;
            return false;
        }

        // Check from 2 to n-1
        for (int j=2; j<n; j++) {
            if (n % j == 0) {
                std::cout << n << " is NOT prime, divisible by " << j  << std::endl;
                return false;
            }
        }

        std::cout << n << " is prime" << std::endl;
        return true;
    }

//Functions for thread communication

void func1( promise<int> p ) {
    int res = 18;
    p.set_value(res);
}

int func2 ( future<int> f) {
    int res=f.get();
    return res;
}

// Function for packaged task
int compute_double(int value) { return value*2; }


int main () {

    // BASIC WORKING MECHANISM
    cout << "###  WORKING MECHANISM OF FUTURE AND PROMISE ### " <<endl;
    promise<int> p;
    future<int> f = p.get_future();
    thread t = thread(factorial, 4, ref(p));
    // here we have the data
    int x = f.get();
    cout << "Result is : " << x << endl;
    t.join();

    // WITH ASYNC
    cout << "### FUTURE AND PROMISE WITH ASYNC ### " <<endl ;

    std::future<bool> fut = std::async(std::launch::async, is_prime, 117);
    future<bool> fut2 = async(std::launch::deferred, is_prime, 5);

    // ... do other work ...
    bool ret = fut.get();// waits for is_prime to return
    cout << "Is 117 prime? " << ret << endl;

    cout << "Is 5 prime ? " << fut2.get() << endl; //further synchronization might be necessary

    // FURTHER EXAMPLE WITH ASYNC AND FACTORIAL V2
    cout << "### FURTHER EXAMPLE WITH FACTORIAL V2 ### " <<endl ;
    std::promise<int> pr_input;
    std::future<int> fut_input = pr_input.get_future();
    std::future<int> fut_fact = async(std::launch::async, factorial_v2, std::ref(fut_input));

    // here we have the data
    pr_input.set_value(4);
    int final_result = fut_fact.get();

    cout << "Factorial of 4 is : " << final_result << endl;

    //EXAMPLE OF THREAD COMMUNICATION
    cout << "### EXAMPLE OF THREAD COMMUNICATION ### " <<endl ;

    promise<int> p1;
    future<int> f1 = p1.get_future();
    future<void> fu1 = async(func1, move(p1) );
    future<int> fu2 = async(func2, move(f1) );
    x = fu2.get();
    cout << "Value of x: " << x << endl;


    //EXAMPLE OF PACKAGED TASK
    cout << "### EXAMPLE OF PACKAGED TASK ### " <<endl ;

    packaged_task<int(int)> tsk(compute_double);
    future<int> fut_pck = tsk.get_future();
    tsk(19);
    int r_value = fut_pck .get();
    cout << "Output: " << r_value << endl;

    packaged_task<int(int)> tsk2(compute_double);
    future<int> fut_pck2 = tsk2.get_future();
    thread th(std::move(tsk2), 19);
    int r_value2  = fut_pck2.get();
    cout << "Output: " << r_value2 << endl; th.join();

    //EXAMPLE OF SHARED FUTURE
    cout << "### EXAMPLE OF SHARED FUTURE ### " <<endl ;
    promise<int> pA;
    future<int> fA = pA.get_future();
    shared_future<int> sf = fA.share();

    future<int> sfu = async(std::launch::async, factorial_v3, sf);
    future<int> sfu2 = async(std::launch::async, factorial_v3, sf);
    future<int> sfu3 = async(std::launch::async, factorial_v3, sf);
    pA.set_value(4);
    int x_sfu = sfu.get();
    int x_sfu2 = sfu2.get();
    int x_sfu3 = sfu3.get();

    cout << "Output 1: " << x_sfu << endl;
    cout << "Output 2: " << x_sfu2 << endl;
    cout << "Output 3: " << x_sfu3 << endl;

    return 0;
}


