#include <iostream>
#include <thread>
#include <unistd.h>
  
using namespace std;
  
//定义带参数子线程
void thread01(int num)
{
    for (int i = 0; i < num; i++)
    {
        cout << "Thread 01 is working ！" << endl;
        sleep(1);
    }
}
void thread02(int num)
{
    for (int i = 0; i < num; i++)
    {
        cout << "Thread 02 is working ！" << endl;
        sleep(2);
    }
}
  
int main()
{
    thread task01(thread01, 5);  //带参数子线程
    thread task02(thread02, 5);
    task01.detach();
    task02.detach();
  
    for (int i = 0; i < 5; i++)
    {
        cout << "Main thread is working ！" << endl;
        sleep(2);
    }
    system("pause");
}
