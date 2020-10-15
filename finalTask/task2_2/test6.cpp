#include<iostream>
#include<thread>
#include<mutex>

using namespace std;

mutex m;
int cnt = 10;

void thread1() {
    while (cnt > 5){
        m.lock();
        if (cnt > 0) {
            --cnt;
            cout << cnt << endl;
        }
        m.unlock();
    }
}

void thread2() {
    while (cnt > 0) {
        m.lock();
        if (cnt > 0) {
            cnt -= 10;
            cout << cnt << endl;
        }
        m.unlock();
    }
}

int main(int argc, char* argv[]) {
    thread th1(thread1);   //实例化一个线程对象th1，该线程开始执行
    thread th2(thread2);
    th1.join();
    th2.join();
    cout << "main..." << endl;
    return 0;
}
