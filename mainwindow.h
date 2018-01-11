#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QList>
#include <QLinkedList>
#include <QFloat16>
#include <QMainWindow>
#include "chart/view.h"

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
    QList<qreal> m_Data;
    int m_nRefreshCount = 0;
    QLinkedList<qfloat16> m_gActualEmitData;
    QLinkedList<qfloat16> m_gConvolutionData;

    qfloat16 OriginEmitFun(qfloat16 x);
    qfloat16 NoiseFun();

private slots:
    void RefreshData();
};

#endif // MAINWINDOW_H
