#ifndef ConvolutionCalculator_H
#define ConvolutionCalculator_H

#include <QObject>
#include <QFloat16>
#include "roundarray.h"

class ConvolutionCalculator : public QObject
{
    Q_OBJECT

public:
    ConvolutionCalculator(QObject *parent = nullptr);

private:
    void NormalConvolute(RoundArray<qfloat16> *result,
                         RoundArray<qfloat16> *seq1,
                         RoundArray<qfloat16> *seq2);

public slots:
    void doWork(RoundArray<qfloat16> *result,
                RoundArray<qfloat16> *seq1,
                RoundArray<qfloat16> *seq2);

signals:
    void resultReady();
};

#endif // ConvolutionCalculator_H
