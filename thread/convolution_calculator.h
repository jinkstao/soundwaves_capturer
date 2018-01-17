#ifndef ConvolutionCalculator_H
#define ConvolutionCalculator_H

#include <QObject>
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
    float *m_gSeqArray;
    float *m_gRealArray;
    float *m_gConvolutionArray;
    complex *m_gComplexArray;
    complex *m_gMultipleComplexArray;
    void d2fft(float *gSeqArray,
               float *gRealArray,
               complex *gComplexArray,
               bool ifft = false);
    void FastConvolute(complex *gComplexArray,
                       float *gConvolutionArray);
    void NormalConvolute(RoundArray<float> *result,
                         RoundArray<float> *seq1,
                         RoundArray<float> *seq2);

public slots:
    void doWork(RoundArray<float> *gSeqArray,
                RoundArray<float> *gFourierArray,
                RoundArray<float> *gConvolutionArray);

signals:
    void resultReady();
};

#endif // ConvolutionCalculator_H
