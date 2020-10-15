#include <iostream>
#include <thread>
#include <unistd.h>
#include <mutex>
  
using namespace std;
  
mutex mu;  //线程互斥对象
  
int totalNum = 100;
  
void thread01()
{
    while (totalNum > 0)
    {
        mu.lock(); //同步数据锁
        cout << totalNum << endl;
        totalNum--;
        sleep(2);
        mu.unlock();  //解除锁定
    }
}
void thread02()
{
    while (totalNum > 0)
    {
        mu.lock();
        cout << totalNum << endl;
        totalNum--;
        sleep(2);
        mu.unlock();
    }
}
  
int main()
{
    thread task01(thread01);
    thread task02(thread02);
    task01.detach();
    task02.detach();
    system("pause");
}
