#ifndef MAX_CONVOLUTION_CALCULATOR_H
#define MAX_CONVOLUTION_CALCULATOR_H

#include <QObject>
#include <QFloat16>
#include "roundarray.h"

class MaxConvolutionCalculator : public QObject
{
    Q_OBJECT

public:
    MaxConvolutionCalculator(QObject *parent = nullptr);

private:
    void GetMax(RoundArray<qfloat16> *gArray,
                qfloat16 *fMax,
                qfloat16 *fMin);

public slots:
    void doWork(RoundArray<qfloat16> *gArray,
                qfloat16 *fMax,
                qfloat16 *fMin);

signals:
    void resultReady();
};

#endif // MAX_CONVOLUTION_CALCULATOR_H
