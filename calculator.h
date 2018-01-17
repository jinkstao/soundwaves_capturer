#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <QFloat16>
#include <QtMath>
#define         SIN_TABLE_SIZE      65536
#define         PI                  3.1415926536

typedef struct struct_complex
{
    qfloat16 real;
    qfloat16 imag;

    qfloat16 module()
    {
        return sqrt(real * real + imag * imag);
    }

    struct_complex &operator=(struct_complex &value)
    {
        real = value.real;
        imag = value.imag;
        return *this;
    }

    struct_complex &operator+(struct_complex &value)
    {
        real += value.real;
        imag += value.imag;
        return *this;
    }

    struct_complex &operator-(struct_complex &value)
    {
        real -= value.real;
        imag -= value.imag;
        return *this;
    }

    struct_complex &operator*(struct_complex &value)
    {
        real = real * value.real - imag * value.imag;
        imag = real * value.imag + imag * value.real;
        return *this;
    }
}complex;

class Calculator
{
public:
    Calculator(int nTotalCount);
    ~Calculator();
    void d2fft(qfloat16 *gSeqArray,
               qfloat16 *gRealArray,
               complex *gComplexArray,
               bool ifft = false);

private:
    int m_nLevel;
    int m_nTotalCount;
    int m_nMapValue;
    complex *m_gWList;
    complex *m_gTempList;
    qfloat16 m_gSinTable[SIN_TABLE_SIZE];
    void CreateTable();
    qfloat16 m_fSin(int t);
    qfloat16 m_fCos(int t);
};

#endif // CALCULATOR_H
