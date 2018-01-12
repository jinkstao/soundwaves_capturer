#include <QtCharts/QChart>
#include <QtCharts/QSplineSeries>
#include <QObject>
#include <QtMath>
#include <QTime>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "roundarray.cpp"

#define     PI              3.1415926536
#define     DOT_COUNT       16

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pRefresher(new QTimer(this)),
    m_gActualEmitData(new RoundArray<qfloat16>(MAX_RANGE)),
    m_gConvolutionData(new RoundArray<qfloat16>(MAX_RANGE * 2 - 1)),
    m_gConvolutionDrawData(new RoundArray<qfloat16>(MAX_RANGE * 2 - 1)),
    m_tWorkThread(new QThread()),
    m_pCalculator(new ConvolutionCalculator())
{
    ui->setupUi(this);
    // 初始化Charts
    ui->chtOriginEmit->chart()->setTitle("原始发射波形");
    ui->chtNoise->chart()->setTitle("噪声波形");
    ui->chtActualEmit->chart()->setTitle("叠加发射波形");
    ui->chtConvolution->chart()->setTitle("卷积波形");
    ui->chtConvolution->chart()->axisX()->setRange(0, MAX_RANGE * 2 - 1);
    ui->chtConvolution->chart()->axisY()->setRange(-3000, 3000);

    // 初始化链表
    for(int i = 0; i < MAX_RANGE * 2 - 1; ++i)
    {
//        m_gActualEmitData.append(0.0f);
        m_gConvolutionData->append(0.0f);
        m_gConvolutionDrawData->append(0.0f);
    }

    // 初始化计算线程
    m_pCalculator->moveToThread(m_tWorkThread);
    connect(m_tWorkThread, &QThread::finished, m_pCalculator, &QObject::deleteLater);
    connect(this, &MainWindow::startCalculate, m_pCalculator, &ConvolutionCalculator::doWork);
    connect(m_pCalculator, &ConvolutionCalculator::resultReady, this, &MainWindow::CopyConvolutionData);
    m_tWorkThread->start();

    // 初始化刷新器
    m_pRefresher->setInterval(100);
    m_pRefresher->start();
    connect(m_pRefresher, &QTimer::timeout, this, &MainWindow::RefreshData);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_gActualEmitData;
    delete m_gConvolutionData;
    m_tWorkThread->quit();
    m_tWorkThread->wait();
}

qfloat16 MainWindow::OriginEmitFun(qfloat16 x)
{
    return 3 * qSin(x * PI * 4 / MAX_RANGE);
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
    for(int i = 0; i < DOT_COUNT; ++i)
    {
        // 模拟正弦波形
        ++m_nRefreshCount;
        if(m_nRefreshCount >= MAX_RANGE)
        {
            m_nRefreshCount = 0;
        }
        qfloat16 nOriginEmitValue = OriginEmitFun((qfloat16)m_nRefreshCount);
        ui->chtOriginEmit->appendData(nOriginEmitValue);
        qfloat16 nNoiseValue = NoiseFun();
        ui->chtNoise->appendData(nNoiseValue);
        qfloat16 nActualEmitValue = nOriginEmitValue + nNoiseValue;
        ui->chtActualEmit->appendData(nActualEmitValue);
        // 插入链表
        m_gActualEmitData->append(nActualEmitValue);
    }

    emit startCalculate(m_gConvolutionData, m_gActualEmitData, m_gActualEmitData);
    ui->chtOriginEmit->refresh();
    ui->chtNoise->refresh();
    ui->chtActualEmit->refresh();
//    // 计算卷积
//    NormalConvolute(m_gConvolutionData, m_gActualEmitData, m_gActualEmitData);
    ui->chtConvolution->appendData(m_gConvolutionDrawData);
}

void MainWindow::CopyConvolutionData()
{
    for(int i = 0; i < MAX_RANGE * 2 - 1; ++i)
    {
        m_gConvolutionDrawData->at(i) = m_gConvolutionData->at(i);
    }
}
