#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QList>
#include <QLinkedList>
#include <QFloat16>
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
    RoundArray<qfloat16> *m_gOriginEmitData;
    RoundArray<qfloat16> *m_gNoiseData;
    RoundArray<qfloat16> *m_gActualEmitData;
    RoundArray<qfloat16> *m_gFourierData;
    RoundArray<qfloat16> *m_gConvolutionData;
    QThread *m_tConvoluteThread;
    QThread *m_tMaxCalculateThread;
    ConvolutionCalculator *m_pCalculator;
    MaxConvolutionCalculator *m_pMaxCalculator;
    qfloat16 OriginEmitFun(qfloat16 x);
    qfloat16 NoiseFun();

    void NormalConvolute(RoundArray<qfloat16>*, RoundArray<qfloat16>*, RoundArray<qfloat16>*);

private slots:
    void RefreshData();

signals:
    void startCalculate(RoundArray<qfloat16> *gSeqArray,
                        RoundArray<qfloat16> *gFourierArray,
                        RoundArray<qfloat16> *gConvolutionArray);
    void startMaxCalculate(RoundArray<qfloat16> *gArray,
                           qfloat16 *fMax,
                           qfloat16 *fMin);
};

#endif // MAINWINDOW_H
