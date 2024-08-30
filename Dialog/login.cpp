#include "login.h"
#include "ui_login.h"

#include <QSettings>


Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    connect(ui->btnPassword,&QPushButton::clicked,[=](){
        ui->btnPassword->isChecked()?
                    ui->passwordine->setEchoMode(QLineEdit::Normal):
                    ui->passwordine->setEchoMode(QLineEdit::Password);
    });

    ConfigIni::U().Read("LoginInformation", [=](QSettings &Read){
        ui->nameline->setText(Read.value("name","admin").toString());
        ui->passwordine->setText(Read.value("password","").toString());

        ui->radRemember->setChecked(!ui->passwordine->text().isEmpty());
    });
}

Login::~Login()
{
    delete ui;
}

void Login::LoginSuccess()
{
    done(1);
    this->close();
}

void Login::LoginFail()
{
    done(0);
    qApp->closeAllWindows();
    qApp->exit();
}

//登录
void Login::on_loginbtn_clicked()
{
    QString name = ui->nameline->text();
    QString password = ui->passwordine->text();

    auto queryData = /*DatabaseOperations::*/Sql::SelectData("login","");

    for( int i = 0 ; i < queryData.length() ; i++)
    {
        if(queryData[i].at(1) == name && queryData[i].at(2) == password)
        {
            LoginSuccess();

            ConfigIni::Write("LoginInformation",QMap<QString,QVariant>{
                                 {"name",ui->nameline->text()},
                                 {"password",ui->passwordine->text()}
                             });
            //SaveTheLog();
            return;
        }
    }
}

void Login::on_exitbtn_clicked()
{
    LoginFail();
}
