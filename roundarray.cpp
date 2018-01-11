#include <stdlib.h>
#include "roundarray.h"

template<class T>
RoundArray<T>::RoundArray(int nMaxSize)
{
    m_nMaxSize = nMaxSize;
    m_nCurSize = 0;
    m_nHeadPos = 0;
    m_pArray = (T*)calloc(m_nMaxSize, sizeof(T));
}

template<class T>
RoundArray<T>::~RoundArray()
{
    free(m_pArray);
    m_pArray = nullptr;
}

template<class T>
int RoundArray<T>::size()
{
    return m_nCurSize;
}

template<class T>
RoundArray<T> &RoundArray<T>::append(T value)
{
    if(m_nCurSize < m_nMaxSize)
    {
        // 正常存数据
        *(m_pArray + m_nCurSize) = value;
        ++m_nCurSize;
    }
    else
    {
        // 移动头部指示器，将后来数据覆盖之前的数据
        if(m_nHeadPos >= m_nMaxSize - 1)
        {
            // 若头部指示器指向最后一个元素，则使其回到开头
            m_nHeadPos = 0;
        }
        else
        {
            // 否则移动头部指示器
            ++m_nHeadPos;
        }
        // 将数据存入头部指示器前一个位置
        int nTailPos = m_nHeadPos > 0 ? m_nHeadPos - 1 : m_nMaxSize - 1;
        *(m_pArray + nTailPos) = value;
    }
    return *this;
}

template <class T>
T &RoundArray<T>::operator[](int i)
{
    int nCalPos = m_nHeadPos + i;
    if(nCalPos > m_nMaxSize - 1)
    {
        nCalPos -= m_nMaxSize;
    }
    return *(m_pArray + nCalPos);
}

template <class T>
T &RoundArray<T>::at(int i)
{
    int nCalPos = m_nHeadPos + i;
    if(nCalPos > m_nMaxSize - 1)
    {
        nCalPos -= m_nMaxSize;
    }
    return *(m_pArray + nCalPos);
}
