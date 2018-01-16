#include "convolution_calculator.h"
#include "roundarray.cpp"

ConvolutionCalculator::ConvolutionCalculator(QObject *parent) : QObject(parent)
{

}

void ConvolutionCalculator::NormalConvolute(RoundArray<qfloat16> *result,
                                            RoundArray<qfloat16> *seq1,
                                            RoundArray<qfloat16> *seq2)
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

void ConvolutionCalculator::doWork(RoundArray<qfloat16> *result,
                                   RoundArray<qfloat16> *seq1,
                                   RoundArray<qfloat16> *seq2)
{
    NormalConvolute(result, seq1, seq2);
    emit resultReady();
}
