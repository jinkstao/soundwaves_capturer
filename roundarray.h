#ifndef ROUNDARRAY_H
#define ROUNDARRAY_H

template<typename T>
class RoundArray
{
private:
    T *m_pArray;
    unsigned long m_nMaxSize;
    unsigned long m_nCurSize;
    unsigned long m_nHeadPos;
public:
    RoundArray(unsigned long nMaxSize);
    ~RoundArray();
    RoundArray<T> &append(T value);
    inline T &operator[](unsigned long i);
};

#endif // ROUNDARRAY_H
