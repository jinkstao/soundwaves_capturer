#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <QtMath>
#define         SIN_TABLE_SIZE      65536
#define         PI                  3.1415926536

typedef struct struct_complex
{
    float real;
    float imag;

    float module()
    {
        return sqrt(real * real + imag * imag);
    }
}complex;

class Calculator
{
public:
    Calculator(int nTotalCount);
    ~Calculator();
    void ComplexAdd(complex *complex1, complex *complex2, complex *res);
    void ComplexMinus(complex *complex1, complex *complex2, complex *res);
    void ComplexMultiple(complex *complex1, complex *complex2, complex *res);
    void d2fft(float *gSeqArray,
               float *gRealArray,
               complex *gComplexArray,
               bool ifft = false);

private:
    int m_nLevel;
    int m_nTotalCount;
    int m_nMapValue;
    complex *m_gWList;
    complex *m_gTempList;
    float m_gSinTable[SIN_TABLE_SIZE];
    void CreateTable();
    float m_fSin(int t);
    float m_fCos(int t);
};

#endif // CALCULATOR_H
