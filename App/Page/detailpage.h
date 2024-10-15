#ifndef DETAILPAGE_H
#define DETAILPAGE_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlQueryModel>


namespace Ui {
class DetailPage;
}

class DetailPage : public QDialog
{
    Q_OBJECT
public:
    explicit DetailPage(QWidget *parent = nullptr);
    ~DetailPage();


    void displaySteadyDetail(QSqlQueryModel *model);


private:
    Ui::DetailPage *ui;
};

#endif // DETAILPAGE_H
