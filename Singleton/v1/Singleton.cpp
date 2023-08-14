#include <atomic>
#include <mutex>

class Lock{

};

class Singleton{
private:
    Singleton();
    Singleton(const Singleton& other);
    // 必须将构造函数和拷贝构造函数显式声明为private，不能省略
    // 也可以将构造函数设为protected，以允许派生类访问
public:
    static Singleton* getInstance();
    static Singleton* m_instance;
};

Singleton* Singleton::m_instance = nullptr;
// 静态成员变量必须在类外初始化，且只能初始化一次，初始化时不再使用static

// 线程非安全版本
Singleton* Singleton::getInstance() {
    if (m_instance == nullptr) {
        m_instance = new Singleton();
    }
    return m_instance;
}

// 线程安全版本，但锁的代价过高
Singleton* Singleton::getInstance() {
    Lock lock; // 局部变量会在函数结束时自动析构，释放锁
    if (m_instance == nullptr) {
        m_instance = new Singleton();
    }
    return m_instance;
}

// 双检查(两次if判断) 锁，但由于内存读写reorder不安全
Singleton* Singleton::getInstance() {
    if (m_instance == nullptr){
        Lock lock; // 只锁定写操作的代码
        if (m_instance == nullptr) {
            m_instance = new Singleton();
            // new表达式在CPU指令集执行时可能存在以下两种顺序：
            //      分配内存 -> 调用构造函数 -> 赋值给m_instance
            //      分配内存 -> 赋值给m_instance -> 调用构造函数
            // 由于内存读写reorder，可能导致m_instance先被赋值，但还未调用构造函数创建对象
        }
    }
    return m_instance; // 读操作不需要加锁
}

// C++ 11版本之后的跨平台实现 (Java、C#关键字:volatile)
std::atomic<Singleton*> Singleton::m_instance;
std::mutex Singleton::m_mutex;

Singleton* Singleton::getInstance() {
    Singleton* tmp = m_instance.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire); // 获取内存fence
    if (tmp == nullptr) {
        std::lock_guard<std::mutex> lock(m_mutex);
        tmp = m_instance.load(std::memory_order_relaxed);
        if (tmp == nullptr) {
            tmp = new Singleton;
            std::atomic_thread_fence(std::memory_order_release); // 释放内存fence
            m_instance.store(tmp, std::memory_order_relaxed);
        }
    }
    return tmp;
}








