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

    void signalBind();

    void displayBasicText();
    void fillInTheBlanks(QVector<QString> v_topData,QVector<QString> v_mediumData,QVector<QString> v_bottomData);

    void displaySteadyDetail(QSqlQueryModel *model);
    void displaySuddLoadDetail(QSqlQueryModel *model);
    void displaySuddunLoadDetail(QSqlQueryModel *model);
    void displayRecordWaveDetail(QSqlQueryModel *model);


private:
    Ui::DetailPage *ui;
};

#endif // DETAILPAGE_H
