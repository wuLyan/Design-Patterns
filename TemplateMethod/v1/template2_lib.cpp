#include <iostream>

using namespace std;

//程序库开发人员
class Library
{
  public:
    //稳定 template method
    void Run()
    {
        Step1();

        if (Step2())
        { //支持变化 ==> 虚函数的多态调用
            Step3();
        }

        for (int i = 0; i < 4; i++)
        {
            Step4(); //支持变化 ==> 虚函数的多态调用
        }

        Step5();
    }
    virtual ~Library() {}

  protected:
    // 稳定的部分写为非虚函数，不允许子类重写/覆盖
    void Step1()
    { //稳定
        cout << "Step1" << endl;
    }
    void Step3()
    { //稳定
        cout << "Step3" << endl;
    }
    void Step5()
    {  //稳定
        cout << "Step5" << endl;
    }

    // 变化的部分写为纯虚函数，要求子类必须重写/覆盖
    virtual bool Step2() = 0; //变化
    virtual void Step4() = 0; //变化
};