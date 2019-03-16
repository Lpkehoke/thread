#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
using namespace std;

unsigned long long int prSum(vector<int> &v, size_t start, size_t finish) {
    unsigned long long int sum = 0;
    // cout << "start " << start << " finish " << finish << "\t" << this_thread::get_id() <<endl;
    if(start == finish) {
        sum = v[start];
        // cout << "el " << v[start] << endl;
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

    unsigned long long int sum = 0;
    auto begin = chrono::high_resolution_clock::now();

    for ( size_t i = 0; i < numT; ++i) {
        // cout << N/numT*i << " : " << N/numT*(i+1)-1 << " sum " << sum << endl;

        tVector[i] = thread([&v,&sum, numT, i, N]() {
            auto locSum = prSum(v, N/numT*i, N/numT*(i+1)-1);
            sum += locSum;
        });
    }
    for (auto &i : tVector) {
        i.join();
    }
    auto end = chrono::high_resolution_clock::now();
    cout<<chrono::duration_cast<chrono::milliseconds>(end-begin).count()<<"ms \t" << "sum: " << sum << endl;


    begin = chrono::high_resolution_clock::now();
    sum = prSum(v, 0, N-1);
    end = chrono::high_resolution_clock::now();
    cout<<chrono::duration_cast<chrono::milliseconds>(end-begin).count()<<"ms \t" << "sum: " << sum << endl;

    return 0;
}
