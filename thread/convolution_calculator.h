#ifndef ConvolutionCalculator_H
#define ConvolutionCalculator_H

#include <QObject>
#include <QFloat16>
#include "roundarray.h"
#include "calculator.h"

class ConvolutionCalculator : public QObject
{
    Q_OBJECT

public:
    ConvolutionCalculator(int nDotCount, QObject *parent = nullptr);
    ~ConvolutionCalculator();
private:
    int m_nDotCount;
    Calculator *m_pCalculator;
    qfloat16 *m_gSeqArray;
    qfloat16 *m_gRealArray;
    qfloat16 *m_gConvolutionArray;
    complex *m_gComplexArray;
    complex *m_gMultipleComplexArray;
    void d2fft(qfloat16 *gSeqArray,
               qfloat16 *gRealArray,
               complex *gComplexArray,
               bool ifft = false);
    void FastConvolute(complex *gComplexArray,
                       qfloat16 *gConvolutionArray);
    void NormalConvolute(RoundArray<qfloat16> *result,
                         RoundArray<qfloat16> *seq1,
                         RoundArray<qfloat16> *seq2);

public slots:
    void doWork(RoundArray<qfloat16> *gSeqArray,
                RoundArray<qfloat16> *gFourierArray,
                RoundArray<qfloat16> *gConvolutionArray);

signals:
    void resultReady();
};

#endif // ConvolutionCalculator_H
