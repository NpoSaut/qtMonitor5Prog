#ifndef SINGLETONE_H
#define SINGLETONE_H

#include <QDebug>

template <class T>
class Singletone
{
public:
    Singletone(T &rObject)
    {
        if(!s_pInstance)
            s_pInstance = &rObject;
    }
    ~Singletone()
    {
        if(s_pInstance)
            s_pInstance = 0;
    }

    static T &instance()
    {
        if(s_pInstance)
            return *s_pInstance;
    }
private:
    static T *s_pInstance;
};

template <class T> T *Singletone<T>::s_pInstance = 0;
#endif // SINGLETONE_H
