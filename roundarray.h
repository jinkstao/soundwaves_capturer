#ifndef ROUNDARRAY_H
#define ROUNDARRAY_H

template<class T> class RoundArray
{
private:
    T *m_pArray;
    int m_nMaxSize;
    int m_nCurSize;
    int m_nHeadPos;
public:
    RoundArray(int nMaxSize);
    ~RoundArray();
    int size();
    RoundArray<T> &append(T value);
    T &operator[](int i);
    T &at(int i);
};

#endif // ROUNDARRAY_H
