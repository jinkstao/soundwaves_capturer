#include <QDebug>
#include "convolution_calculator.h"
#include "roundarray.cpp"

ConvolutionCalculator::ConvolutionCalculator(int nDotCount, QObject *parent) : QObject(parent),
    m_pCalculator(new Calculator(nDotCount))
{
    m_nDotCount = nDotCount;
    m_gSeqArray = (float*)calloc(m_nDotCount, sizeof(float));
    m_gRealArray = (float*)calloc(m_nDotCount, sizeof(float));
    m_gConvolutionArray = (float*)calloc(m_nDotCount, sizeof(float));
    m_gComplexArray = (complex*)calloc(m_nDotCount, sizeof(complex));
    m_gMultipleComplexArray = (complex*)calloc(m_nDotCount, sizeof(complex));
//    complex zeroComplex;
//    zeroComplex.real = 0.0f;
//    zeroComplex.imag = 0.0f;
//    for(int i = 0; i < nDotCount; ++i)
//    {
//        m_gSeqArray->append(0.0f);
//        m_gComplexArray->append(zeroComplex);
//        m_gMultipleComplexArray->append(zeroComplex);
//    }
}

ConvolutionCalculator::~ConvolutionCalculator()
{
    free(m_gSeqArray);
    m_gSeqArray = nullptr;
    free(m_gRealArray);
    m_gRealArray = nullptr;
    free(m_gConvolutionArray);
    m_gConvolutionArray = nullptr;
    free(m_gComplexArray);
    m_gComplexArray = nullptr;
    free(m_gMultipleComplexArray);
    m_gMultipleComplexArray = nullptr;
}

void ConvolutionCalculator::d2fft(float *gSeqArray,
                                  float *gRealArray,
                                  complex *gComplexArray,
                                  bool ifft)
{
    m_pCalculator->d2fft(gSeqArray, gRealArray, gComplexArray, ifft);
}

void ConvolutionCalculator::FastConvolute(complex *gComplexArray,
                                          float *gConvolutionArray)
{
    for(int i = 0; i < m_nDotCount; ++i)
    {
        m_pCalculator->ComplexMultiple(gComplexArray + i, gComplexArray + i, m_gMultipleComplexArray + i);
    }
    d2fft(nullptr, gConvolutionArray, m_gMultipleComplexArray, true);
}

void ConvolutionCalculator::NormalConvolute(RoundArray<float> *result,
                                            RoundArray<float> *seq1,
                                            RoundArray<float> *seq2)
{
    int len1 = seq1->size();
    int len2 = seq2->size();
    int resultLen = len1 + len2 - 1;
    for(int nRes = 0; nRes < resultLen; ++nRes)
    {
        (*result)[nRes] = 0.0f;
        for(int nSeq = 0; nSeq < len1; ++nSeq)
        {
            if(nSeq > nRes - len2 && nSeq <= nRes)
            {
                (*result)[nRes] += seq1->at(nSeq) * seq2->at(nRes - nSeq);
            }
        }
    }
}

void ConvolutionCalculator::doWork(RoundArray<float> *gSeqArray,
                                   RoundArray<float> *gFourierArray,
                                   RoundArray<float> *gConvolutionArray)
{
    for(int i = 0; i < m_nDotCount; ++i)
    {
        if(i < gSeqArray->size())
        {
            m_gSeqArray[i] = gSeqArray->at(i);
        }
        else
        {
            m_gSeqArray[i] = 0.0f;
        }
    }
    d2fft(m_gSeqArray, m_gRealArray, m_gComplexArray);
    FastConvolute(m_gComplexArray, m_gConvolutionArray);
    for(int i = 0; i < m_nDotCount; ++i)
    {
        gFourierArray->at(i) = m_gRealArray[i];
        gConvolutionArray->at(i) = m_gConvolutionArray[i];
    }
//    NormalConvolute(gConvolutionArray, gSeqArray, gSeqArray);
    emit resultReady();
}
