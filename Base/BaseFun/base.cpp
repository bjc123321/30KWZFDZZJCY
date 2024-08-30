#include "base.h"

static Base &stc_Base = Base::U();

Base::Base(QObject *parent) : QObject(parent)
{
    setCurMode(curMode);
}

void Base::setCurMode(Base::WORK_MODE mode)
{
    switch (mode) {
    case TEXT_MODE:
        QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
        break;
    case PRODUCE_MODE:
        QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, false);
        break;
    }
    curMode = mode;
}

void Base::setCurMode(Base::POWER_MODE mode)
{
    switch (mode) {
    case ADMIN_MODE:
        setCurMode(TEXT_MODE);
        break;
    case USER_MODE:
    case VISITOR_MODE:
        setCurMode(PRODUCE_MODE);
        break;
    }
}

Base::WORK_MODE Base::getCurMode()
{
    return curMode;
}

void Base::SetCurMode(Base::WORK_MODE mode)
{
    Base::U().setCurMode(mode);
}

void Base::SetCurMode(Base::POWER_MODE mode)
{
    Base::U().setCurMode(mode);
}

Base::WORK_MODE Base::GetCurMode()
{
    return Base::U().getCurMode();
}




Tool::SetCustomPlot(QCustomPlot *plot, QCheckBox *box0, QCheckBox *box1, QCheckBox *box2)
{
    plot->connect(box0,QCheckBox::clicked,plot,[=](bool isChecked){
        plot->graph(0)->setVisible(isChecked);
        plot->replot();
    });
    plot->connect(box1,QCheckBox::clicked,plot,[=](bool isChecked){
        plot->graph(1)->setVisible(isChecked);
        plot->replot();
    });
    plot->connect(box2,QCheckBox::clicked,plot,[=](bool isChecked){
        plot->graph(2)->setVisible(isChecked);
        plot->replot();
    });

    plot->addGraph();//添加曲线
    plot->graph(0)->setPen(QPen(Qt::blue)); //给曲线设置颜色
    plot->addGraph();//添加曲线
    plot->graph(1)->setPen(QPen(Qt::red)); //给曲线设置颜色
    plot->addGraph();//添加曲线
    plot->graph(2)->setPen(QPen(Qt::yellow)); //给曲线设置颜色

    plot->xAxis2->setVisible(true);
    plot->xAxis2->setTickLabels(false);
    plot->yAxis2->setVisible(true);
    plot->yAxis2->setTickLabels(false);
    // 使上下两个X轴的范围总是相等，使左右两个Y轴的范围总是相等
    plot->connect(plot->xAxis, SIGNAL(rangeChanged(QCPRange)), plot->xAxis2, SLOT(setRange(QCPRange)));
    plot->connect(plot->yAxis, SIGNAL(rangeChanged(QCPRange)), plot->yAxis2, SLOT(setRange(QCPRange)));

    //三个vector来存放数据
    QVector<double> x(251), y0(251), y1(251),y2(400);
    for (int i=0; i<251; ++i)
    {
        x[i] = i;

        y0[i] = qExp(-i/150.0)*qCos(i/10.0); // 指数衰减（振幅）
        y1[i] = qExp(-i/150.0);              // 指数衰减
        y2[i] = qExp(-i/100);
    }
    // 边框右侧和上侧均显示刻度线，但不显示刻度值:
    //注意：每个plot有上下左右四个坐标轴xAxis、xAxis2、yAxis、yAxis2，我们可以选择是否显示坐标轴、坐标轴的刻度、刻度的数值
    plot->xAxis2->setVisible(true);
    plot->xAxis2->setTickLabels(false);
    plot->yAxis2->setVisible(true);
    plot->yAxis2->setTickLabels(false);
    // 使上下两个X轴的范围总是相等，使左右两个Y轴的范围总是相等
    plot->connect(plot->xAxis, SIGNAL(rangeChanged(QCPRange)), plot->xAxis2, SLOT(setRange(QCPRange)));
    plot->connect(plot->yAxis, SIGNAL(rangeChanged(QCPRange)), plot->yAxis2, SLOT(setRange(QCPRange)));
    // 把已存在的数据填充进graph的数据区
    plot->graph(0)->setData(x, y0);
    plot->graph(1)->setData(x, y1);
    plot->graph(2)->setData(x, y2);
    //自动调整XY轴的范围，以便显示出graph(0)中所有的点
    plot->graph(0)->rescaleAxes();
    //自动调整XY轴的范围，以便显示出graph(1)中所有的点
    plot->graph(1)->rescaleAxes(true);
    //自动调整XY轴的范围，以便显示出graph(2)中所有的点
    plot->graph(2)->rescaleAxes(true);
    // 支持鼠标拖拽轴的范围、滚动缩放轴的范围，左键点选图层
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

}
