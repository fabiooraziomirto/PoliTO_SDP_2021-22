#include <iostream>
#include <future>

using namespace std;

// function to check if a number is prime
int is_prime (shared_future<int> f)  {
    int n = f.get();
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



int main (int argc, char **argv) {

    // BASIC WORKING MECHANISM
    cout << "###  WORKING MECHANISM OF FUTURE AND PROMISE ### " <<endl;
    promise<int> p;
    future<int> f = p.get_future();
    thread t;
    int start = atoi(argv[1]);
    int end = atoi(argv[2]);

    cout << "### EXAMPLE OF SHARED FUTURE ### " <<endl ;

    future<int> sfu;
    int x_sfu2;
    int i;
    for(i = start; i < end; i++) {
        promise<int> pA;
        future<int> fA = pA.get_future();
        shared_future<int> sf = fA.share();

        future<int> sfu2 = async(std::launch::async, is_prime, sf);

        pA.set_value(i);

        x_sfu2 = sfu2.get();

    }


    cout << "Output 2: " << x_sfu2 << endl;


    return 0;
}
