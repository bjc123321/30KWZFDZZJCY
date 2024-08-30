#ifndef TRIANGLESLIDER_H
#define TRIANGLESLIDER_H

#include <QSlider>
#include <QWidget>

class TriangleSlider : public QSlider {
    Q_OBJECT

public:
    TriangleSlider(Qt::Orientation orientation, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // TRIANGLESLIDER_H
