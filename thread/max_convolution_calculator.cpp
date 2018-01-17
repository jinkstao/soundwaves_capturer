#include "max_convolution_calculator.h"
#include "roundarray.cpp"

MaxConvolutionCalculator::MaxConvolutionCalculator(QObject *parent) : QObject(parent)
{

}

void MaxConvolutionCalculator::GetMax(RoundArray<float> *gArray,
                                      float *fMax,
                                      float *fMin)
{
    float fCurMax = gArray->at(0);
    float fCurMin = gArray->at(0);
    for(int i = 0; i < gArray->size(); ++i)
    {
        float fCurValue = gArray->at(i);
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

void MaxConvolutionCalculator::doWork(RoundArray<float> *gArray,
                                      float *fMax,
                                      float *fMin)
{
    GetMax(gArray, fMax, fMin);
    emit resultReady();
}
