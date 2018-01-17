#ifndef MAX_CONVOLUTION_CALCULATOR_H
#define MAX_CONVOLUTION_CALCULATOR_H

#include <QObject>
#include "roundarray.h"

class MaxConvolutionCalculator : public QObject
{
    Q_OBJECT

public:
    MaxConvolutionCalculator(QObject *parent = nullptr);

private:
    void GetMax(RoundArray<float> *gArray,
                float *fMax,
                float *fMin);

public slots:
    void doWork(RoundArray<float> *gArray,
                float *fMax,
                float *fMin);

signals:
    void resultReady();
};

#endif // MAX_CONVOLUTION_CALCULATOR_H
