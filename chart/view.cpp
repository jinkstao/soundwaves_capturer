/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "view.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtWidgets/QGraphicsTextItem>
#include <QtGui/QMouseEvent>
#include <QDebug>

View::View(QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent),
      m_chart(0),
      m_series(new QLineSeries(this))
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // chart
    m_chart = new QChart;
    m_chart->legend()->hide();
    m_chart->addSeries(m_series);
    m_chart->setTheme(QChart::ChartThemeDark);
    // 坐标轴初始化
    m_chart->createDefaultAxes();
    auto *axisX = m_chart->axisX();
    auto *axisY = m_chart->axisY();
    axisX->setRange(0, MAX_RANGE);
//    axisX->setLabelFormat("%d");
    axisY->setRange(-10, 10);
//    axisY->setLabelFormat("%d");

//    m_chart->addAxis(axisX, Qt::AlignBottom);
//    m_chart->addAxis(axisY, Qt::AlignLeft);
//    m_series->attachAxis(axisX);
//    m_series->attachAxis(axisY);

    m_series->setPen(QPen(Qt::green, 1));

    m_chart->setAcceptHoverEvents(true);

    setRenderHint(QPainter::Antialiasing);
    scene()->addItem(m_chart);

    this->setMouseTracking(true);
}

QChart *View::chart()
{
    return m_chart;
}

QLineSeries *View::series()
{
    return m_series;
}

void View::appendData(qfloat16 value)
{
    m_gData.append(value);
    if(m_gData.size() > MAX_RANGE)
    {
        m_gData.removeFirst();
    }
    m_series->clear();
    QLinkedList<qfloat16>::const_iterator x = m_gData.constEnd();
    for(int i = 0; i < MAX_RANGE; ++i) {
        if(m_gData.size() > i)
        {
            m_series->append(i, *(x--));
        }
        else
        {
            m_series->append(i, 0.0);
        }
    }
}

void View::appendData(QList<qfloat16> *values)
{
    m_series->clear();
    for(int i = 0; i < MAX_RANGE * 2 - 1; ++i) {
        if(values->size() > i)
        {
            m_series->append(i, values->at(values->size() - i - 1));
        }
        else
        {
            m_series->append(i, 0.0);
        }
    }
}

void View::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
         m_chart->resize(event->size());
    }
    QGraphicsView::resizeEvent(event);
}
