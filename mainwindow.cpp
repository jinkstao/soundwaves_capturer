#include <QtCharts/QChart>
#include <QtCharts/QSplineSeries>
#include <QObject>
#include <QtMath>
#include <QTime>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "roundarray.cpp"

#define     PI                      3.1415926536
#define     FRAME_TIME_INTERVAL     100
#define     FRAME_DOT_COUNT         16
#define     CALCULATION_DOT_COUNT   256
#define     BAUD_FREQUENCY          (8 / AXISX_MAX_RANGE)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pRefresher(new QTimer(this)),
    m_gOriginEmitData(new RoundArray<qfloat16>(CALCULATION_DOT_COUNT)),
    m_gNoiseData(new RoundArray<qfloat16>(CALCULATION_DOT_COUNT)),
    m_gActualEmitData(new RoundArray<qfloat16>(CALCULATION_DOT_COUNT)),
    m_gConvolutionData(new RoundArray<qfloat16>(CALCULATION_DOT_COUNT * 2 - 1)),
    m_gConvolutionDrawData(new RoundArray<qfloat16>(CALCULATION_DOT_COUNT * 2 - 1)),
    m_tConvoluteThread(new QThread()),
    m_tMaxCalculateThread(new QThread()),
    m_pCalculator(new ConvolutionCalculator()),
    m_pMaxCalculator(new MaxConvolutionCalculator())
{
    ui->setupUi(this);
    // 初始化Charts
    ui->chtOriginEmit->chart()->setTitle("原始信号");
    ui->chtNoise->chart()->setTitle("环境噪声模拟");
    ui->chtActualEmit->chart()->setTitle("叠加信号");
    ui->chtConvolution->chart()->setTitle("自相关");
    ui->chtMaxConvolutionEmit->chart()->setTitle("最大卷积");
    ui->chtConvolution->chart()->axisX()->setRange(0, CALCULATION_DOT_COUNT * 2 - 1);
    ui->chtConvolution->chart()->axisY()->setRange(-3000, 3000);
    ui->chtMaxConvolutionEmit->chart()->axisX()->setRange(0, CALCULATION_DOT_COUNT * 2 - 1);
    ui->chtMaxConvolutionEmit->chart()->axisY()->setRange(-3000, 3000);

    // 初始化链表
    for(int i = 0; i < CALCULATION_DOT_COUNT * 2 - 1; ++i)
    {
//        m_gActualEmitData.append(0.0f);
        m_gConvolutionData->append(0.0f);
        m_gConvolutionDrawData->append(0.0f);
    }

    // 初始化卷积计算线程
    m_pCalculator->moveToThread(m_tConvoluteThread);
    connect(m_tConvoluteThread, &QThread::finished, m_pCalculator, &QObject::deleteLater);
    connect(this, &MainWindow::startCalculate, m_pCalculator, &ConvolutionCalculator::doWork);
//    connect(m_pCalculator, &ConvolutionCalculator::resultReady, this, &MainWindow::CopyConvolutionData);
    m_tConvoluteThread->start();

    // 初始化最大卷积计算线程
    m_pMaxCalculator->moveToThread(m_tMaxCalculateThread);
    connect(m_tMaxCalculateThread, &QThread::finished, m_pMaxCalculator, &QObject::deleteLater);
    connect(this, &MainWindow::startMaxCalculate, m_pMaxCalculator, &MaxConvolutionCalculator::doWork);
//    connect(m_pCalculator, &ConvolutionCalculator::resultReady, this, &MainWindow::CopyConvolutionData);
    m_tMaxCalculateThread->start();

    // 初始化刷新器
    m_pRefresher->setInterval(FRAME_TIME_INTERVAL);
    m_pRefresher->start();
    connect(m_pRefresher, &QTimer::timeout, this, &MainWindow::RefreshData);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_gActualEmitData;
    delete m_gConvolutionData;
    m_tConvoluteThread->quit();
    m_tConvoluteThread->wait();
}

qfloat16 MainWindow::OriginEmitFun(qfloat16 x)
{
    qfloat16 n = 3 * qSin(x * PI * 2 * BAUD_FREQUENCY);
    return n;
}

qfloat16 MainWindow::NoiseFun()
{
    //qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    return 10.0 * qrand() / (qfloat16)RAND_MAX - 5.0;
}

void MainWindow::NormalConvolute(RoundArray<qfloat16> *result,
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

void MainWindow::RefreshData()
{
    qfloat16 fMaxConvolution;
    qfloat16 fMinConvolution;
    for(int i = 0; i < FRAME_DOT_COUNT; ++i)
    {
        // 模拟正弦波形
        ++m_nRefreshCount;
        if(m_nRefreshCount >= AXISX_MAX_RANGE / BAUD_FREQUENCY)
        {
            m_nRefreshCount = 0;
        }
        qfloat16 nOriginEmitValue = OriginEmitFun(m_nRefreshCount / SAMPLING_FREQUENCY);
        m_gOriginEmitData->append(nOriginEmitValue);
        ui->chtOriginEmit->appendData(nOriginEmitValue);
        qfloat16 nNoiseValue = NoiseFun();
        m_gNoiseData->append(nNoiseValue);
        ui->chtNoise->appendData(nNoiseValue);
        qfloat16 nActualEmitValue = nOriginEmitValue + nNoiseValue;
        m_gActualEmitData->append(nActualEmitValue);
        ui->chtActualEmit->appendData(nActualEmitValue);
    }

    emit startCalculate(m_gConvolutionData, m_gActualEmitData, m_gActualEmitData);
    if(m_gConvolutionData->size() != 0)
    {
        emit startMaxCalculate(m_gConvolutionData, &fMaxConvolution, &fMinConvolution);
    }
    ui->chtOriginEmit->refresh();
    ui->chtNoise->refresh();
    ui->chtActualEmit->refresh();
    ui->chtConvolution->appendData(m_gConvolutionData);
    ui->chtMaxConvolutionEmit->appendData(fMaxConvolution);
    ui->chtMaxConvolutionEmit->refresh();
}

void MainWindow::CopyConvolutionData()
{
    for(int i = 0; i < AXISX_MAX_RANGE * 2 - 1; ++i)
    {
        m_gConvolutionDrawData->at(i) = m_gConvolutionData->at(i);
    }
}
