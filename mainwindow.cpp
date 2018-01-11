#include <QtCharts/QChart>
#include <QtCharts/QSplineSeries>
#include <QtMath>
#include <QTime>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define     PI      3.1415926536
#define		LEVEL			9	// log2(RESULT_LENGTH)
#define		MAP_VALUE		512	// 4 * TABLE_COUNT / RESULT_LENGTH

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pRefresher(new QTimer(this))
{
    ui->setupUi(this);

    // 初始化Charts
    ui->chtOriginEmit->chart()->setTitle("原始发射波形");
    ui->chtNoise->chart()->setTitle("噪声波形");
    ui->chtActualEmit->chart()->setTitle("叠加发射波形");

//    // 初始化链表
//    for(int i = 0; i < MAX_RANGE; ++i)
//    {
//        m_gActualEmitData.append(0.0f);
//        m_gConvolutionData.append(0.0f);
//    }

    // 初始化刷新器
    m_pRefresher->setInterval(10);
    m_pRefresher->start();
    connect(m_pRefresher, &QTimer::timeout, this, &MainWindow::RefreshData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

qfloat16 MainWindow::OriginEmitFun(qfloat16 x)
{
    return 5 * qSin(x * PI / 64);
}

qfloat16 MainWindow::NoiseFun()
{
    //qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    return 10.0 * qrand() / (qfloat16)RAND_MAX - 5.0;
}

void MainWindow::RefreshData()
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
//    // 插入链表
//    m_gActualEmitData.append(nActualEmitValue);
//    if(m_gActualEmitData.size() > MAX_RANGE)
//    {
//        m_gActualEmitData.removeFirst();
//    }
}
