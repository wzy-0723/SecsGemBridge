#pragma once

template <typename T>
class Singleton
{
public:
    static T* Instance();
    static void Clear();

private:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton & operator= (const Singleton &) = delete;

private:
    static T* m_instance;
    //static std::unique_ptr<T> m_instance;
};

template <typename T>
T* Singleton<T>::m_instance = nullptr;

template <typename T>
T* Singleton<T>::Instance()
{
    {
        if (nullptr == m_instance)
            m_instance = new T;
    };

    return m_instance;
};

template <typename T>
void Singleton<T>::Clear()
{
    if (nullptr != m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    };
};

#ifndef SINGLETON_PTR
#define SINGLETON_PTR(T) Singleton<T>::Instance()
#endif

#ifndef SINGLETON_CLEAR
#define SINGLETON_CLEAR(T) Singleton<T>::Clear()
#endif