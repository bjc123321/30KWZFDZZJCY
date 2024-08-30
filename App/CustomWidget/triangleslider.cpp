#include "triangleslider.h"
#include <QStyleOptionSlider>
#include <QPainter>
#include <QDebug>


TriangleSlider::TriangleSlider(Qt::Orientation orientation, QWidget *parent)
    : QSlider(orientation, parent) {}

void TriangleSlider::paintEvent(QPaintEvent *event) {
    QSlider::paintEvent(event); // 调用基类的绘制方法

    // 获取当前值
    int value = this->value();
    int minValue = this->minimum();
    int maxValue = this->maximum();

    int numberOfTicks = maxValue - minValue; // 刻度数量

    // 计算滑块的逻辑位置，使用范围而非像素
    double positionRatio = static_cast<double>(value - minValue) / numberOfTicks;
    int sliderLogicalPosition = static_cast<int>(positionRatio * this->width());

    QPainter painter(this);

    // 获取滑块的高度
    int sliderHeight = this->height();
    int grooveHeight = 10; // 轨道高度
    int grooveBottom = (sliderHeight - grooveHeight) / 2 + grooveHeight;

    // 绘制朝上的三角形，顶点严格对齐X轴刻度
    QPoint p1(sliderLogicalPosition, grooveBottom - 10);  // 顶点在上
    QPoint p2(sliderLogicalPosition-5, grooveBottom);           // 底边左端
    QPoint p3(sliderLogicalPosition + 5, grooveBottom);      // 底边右端

    // 创建一个三角形形状
    QPolygon triangle;
    triangle << p1 << p2 << p3;

    painter.setBrush(Qt::blue); // 设置颜色
    painter.drawPolygon(triangle); // 绘制三角形
}



