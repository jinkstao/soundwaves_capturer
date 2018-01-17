#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QList>
#include <QLinkedList>
#include <QThread>
#include <QMainWindow>
#include "calculator.h"
#include "chart/view.h"
#include "roundarray.h"
#include "thread/convolution_calculator.h"
#include "thread/max_convolution_calculator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *m_pRefresher;
    int m_nRefreshCount = 0;
    RoundArray<float> *m_gOriginEmitData;
    RoundArray<float> *m_gNoiseData;
    RoundArray<float> *m_gActualEmitData;
    RoundArray<float> *m_gFourierData;
    RoundArray<float> *m_gConvolutionData;
    QThread *m_tConvoluteThread;
    QThread *m_tMaxCalculateThread;
    ConvolutionCalculator *m_pCalculator;
    MaxConvolutionCalculator *m_pMaxCalculator;
    float OriginEmitFun(float x);
    float NoiseFun();

    void NormalConvolute(RoundArray<float>*, RoundArray<float>*, RoundArray<float>*);

private slots:
    void RefreshData();

signals:
    void startCalculate(RoundArray<float> *gSeqArray,
                        RoundArray<float> *gFourierArray,
                        RoundArray<float> *gConvolutionArray);
    void startMaxCalculate(RoundArray<float> *gArray,
                           float *fMax,
                           float *fMin);
};

#endif // MAINWINDOW_H
