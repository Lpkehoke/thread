#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <atomic>
using namespace std;

unsigned long long int prSum(vector<int> &v, size_t start, size_t finish) {
    unsigned long long int sum = 0;
    if(start == finish) {
        sum = v[start];
    } else {
        sum += prSum(v, start, start+(finish-start)/2);
        sum += prSum(v, start+(finish-start)/2+1, finish);
    }

    return sum;
}

int main() {
    const size_t N = 100000000;
    vector<int> v(N);
    for (size_t i = 0; i < N; ++i) {
        v[i] = rand() % 2;
    }

    const size_t numT = 8;
    vector<thread> tVector(numT);

    unsigned long long int sum1 = 0;
    atomic<unsigned long long int> sum2 = 0;

    mutex mx;

    auto begin = chrono::high_resolution_clock::now();
    for ( size_t i = 0; i < numT; ++i) {
        tVector[i] = thread([&v, &sum1, &sum2, &mx, numT, i, N]() {
            auto locSum = prSum(v, N/numT*i, N/numT*(i+1)-1);

            mx.lock();
            sum1 += locSum;
            mx.unlock();

            sum2 += locSum;

        });
    }
    for (auto &i : tVector) {
        i.join();
    }
    auto end = chrono::high_resolution_clock::now();
    cout<<chrono::duration_cast<chrono::milliseconds>(end-begin).count()<<"ms \t" << "sum1: " << sum1 << endl;
    cout<<chrono::duration_cast<chrono::milliseconds>(end-begin).count()<<"ms \t" << "sum2: " << sum2 << endl;

    //
    // begin = chrono::high_resolution_clock::now();
    // sum = prSum(v, 0, N-1);
    // end = chrono::high_resolution_clock::now();
    // cout<<chrono::duration_cast<chrono::milliseconds>(end-begin).count()<<"ms \t" << "sum: " << sum << endl;

    return 0;
}
