#include <iostream>
#include <thread>
#include <unistd.h>
  
using namespace std;
  
void thread01()
{
    for (int i = 0; i < 5; i++)
    {
        cout << "Thread 01 is working ！" << endl;
        sleep(1);
    }
}
void thread02()
{
    for (int i = 0; i < 5; i++)
    {
        cout << "Thread 02 is working ！" << endl;
        sleep(2);
    }
}
  
int main()
{
    thread task01(thread01);
    thread task02(thread02);
    task01.join();
    task02.join();
  
    for (int i = 0; i < 5; i++)
    {
        cout << "Main thread is working ！" << endl;
        sleep(2);
    }
    system("pause");
}
