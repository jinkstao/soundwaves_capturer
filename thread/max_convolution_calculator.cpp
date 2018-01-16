#include "max_convolution_calculator.h"
#include "roundarray.cpp"

MaxConvolutionCalculator::MaxConvolutionCalculator(QObject *parent) : QObject(parent)
{

}

void MaxConvolutionCalculator::GetMax(RoundArray<qfloat16> *gArray,
                                      qfloat16 *fMax,
                                      qfloat16 *fMin)
{
    qfloat16 fCurMax = gArray->at(0);
    qfloat16 fCurMin = gArray->at(0);
    for(int i = 0; i < gArray->size(); ++i)
    {
        qfloat16 fCurValue = gArray->at(i);
        if(fCurValue > fCurMax)
        {
            fCurMax = fCurValue;
        }
        if(fCurValue < fCurMin)
        {
            fCurMin = fCurValue;
        }
    }
    *fMax = fCurMax;
    *fMin = fCurMin;
}

void MaxConvolutionCalculator::doWork(RoundArray<qfloat16> *gArray,
                                      qfloat16 *fMax,
                                      qfloat16 *fMin)
{
    GetMax(gArray, fMax, fMin);
    emit resultReady();
}
