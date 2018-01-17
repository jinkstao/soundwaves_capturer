#include <QDebug>
#include "calculator.h"

Calculator::Calculator(int nTotalCount)
{
    m_nTotalCount = nTotalCount;
    m_nLevel = (int)(qLn(m_nTotalCount) / qLn(2));
    m_nMapValue = 4 * SIN_TABLE_SIZE / m_nTotalCount;
    CreateTable();
    m_gWList = (complex*)calloc(m_nTotalCount / 2, sizeof(complex));
    m_gTempList = (complex*)calloc(m_nTotalCount, sizeof(complex));
}

Calculator::~Calculator()
{
    free(m_gWList);
    m_gWList = nullptr;
    free(m_gTempList);
    m_gTempList = nullptr;
}

void Calculator::CreateTable()
{
    float t;
    for (int i = 0; i < SIN_TABLE_SIZE; i++)
    {
        t = i * PI / 2.0 / SIN_TABLE_SIZE;
        m_gSinTable[i] = qSin(t);
    }
}

float Calculator::m_fSin(int t)
{
    int u;
    if (t == -SIN_TABLE_SIZE)
    {
        u = -t - 1;
    }
    else if (t > -SIN_TABLE_SIZE)
    {
        u = -t;
    }
    else
    {
        u = t + 2 * SIN_TABLE_SIZE;
    }
    return -m_gSinTable[u];
}

float Calculator::m_fCos(int t)
{
    int u, sign;
    if (t == 0)
    {
        u = t + SIN_TABLE_SIZE - 1;
        sign = 1;
    }
    else if (t > -SIN_TABLE_SIZE)
    {
        u = t + SIN_TABLE_SIZE;
        sign = 1;
    }
    else
    {
        u = -(t + SIN_TABLE_SIZE);
        sign = -1;
    }
    return sign * m_gSinTable[u];
}

void Calculator::ComplexAdd(complex *complex1, complex *complex2, complex *res)
{
    res->real = complex1->real + complex2->real;
    res->imag = complex1->imag + complex2->imag;
}

void Calculator::ComplexMinus(complex *complex1, complex *complex2, complex *res)
{
    res->real = complex1->real - complex2->real;
    res->imag = complex1->imag - complex2->imag;
}

void Calculator::ComplexMultiple(complex *complex1, complex *complex2, complex *res)
{
    res->real = complex1->real * complex2->real - complex1->imag * complex2->imag;
    res->imag = complex1->real * complex2->imag + complex1->imag * complex2->real;
}

void Calculator::d2fft(float *gSeqArray,
                       float *gRealArray,
                       complex *gComplexArray,
                       bool ifft)
{
    int i, j, k, angle, interval, halfN, gap;
    unsigned int rev, num;
    complex tmp_c;

    //if it is FFT, copy real sequence to temp sequence, or copy the complex sequence
    for (i = 0; i < m_nTotalCount; i++)
    {
        if (ifft)
        {
            m_gTempList[i].real = gComplexArray[i].real;
            m_gTempList[i].imag = gComplexArray[i].imag;
        }
        else
        {
            m_gTempList[i].real = gSeqArray[i];
            m_gTempList[i].imag = 0;
        }
    }

    // Euler Formula
    for (i = 0; i < m_nTotalCount / 2; i++)
    {
        angle = -i * m_nMapValue;
        m_gWList[i].real = m_fCos(angle);
        m_gWList[i].imag = ifft ? -m_fSin(angle) : m_fSin(angle);
    }

    // Butterfly Transform
    for (i = 0; i < m_nLevel; i++)
    {
        interval = 1 << i;	// store count of groups
        halfN = 1 << (m_nLevel - i);	// store length of every group
        for (j = 0; j < interval; j++)
        {
            // j means group j
            gap = j * halfN;
            for (k = 0; k < halfN / 2; k++)
            {
                ComplexAdd(m_gTempList + k + gap, m_gTempList + k + gap + halfN / 2, gComplexArray + k + gap);
                ComplexMinus(m_gTempList + k + gap, m_gTempList + k + gap + halfN / 2, &tmp_c);
                ComplexMultiple(&tmp_c, m_gWList + k * interval, gComplexArray + k + gap + halfN / 2);
            }
        }
        // copy result to input
        for (j = 0; j < m_nTotalCount; j++)
        {
            m_gTempList[j] = gComplexArray[j];
        }
    }

    // invert the output code bit
    for (i = 0; i < m_nTotalCount; i++)
    {
        rev = 0;
        num = (unsigned int)i;
        for (j = 0; j < m_nLevel; j++)
        {
            rev <<= 1;
            rev |= num & 1;
            num >>= 1;
        }
        gComplexArray[rev] = m_gTempList[i];
    }

    if(gRealArray != nullptr)
    {
        if (ifft)
        {
            for (i = 0; i < m_nTotalCount; i++)
            {
                gRealArray[i] = gComplexArray[i].real / m_nTotalCount;
            }
        }
        else
        {
            for (i = 0; i < m_nTotalCount; i++)
            {
                gRealArray[i] = gComplexArray[i].module();
            }
        }
    }
}
