#include "login.h"
#include "ui_login.h"

#include <QSettings>
#include "GlobalSettings.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    connect(ui->btnPassword,&QPushButton::clicked,[=](){
        //密码是否可显示
        ui->btnPassword->isChecked()?
                    ui->passwordine->setEchoMode(QLineEdit::Normal):
                    ui->passwordine->setEchoMode(QLineEdit::Password);
    });

    ConfigIni::U().Read("LoginInformation", [=](QSettings &Read){
        ui->nameline->setText(Read.value("name","admin").toString());
        ui->passwordine->setText(Read.value("password","").toString());
        GlobalSettings::instance().setLoginMode(Read.value("power","").toString());
        ui->checkBox->setChecked(!ui->passwordine->text().isEmpty());
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

void Login::ExitApp()
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

    qDebug()<<"查到的数据个数:"<<queryData.length()<<"条";

    for( int i = 0 ; i < queryData.length() ; i++)
    {
        if(queryData[i].at(1) == name && queryData[i].at(2) == password)
        {
            LoginSuccess();
            if(queryData[i].at(3) == 2){
                qDebug()<<"当前为管理员模式！！！";

                GlobalSettings::instance().setLoginMode("2");

            }else{
                qDebug()<<"当前为普通用户模式";
                GlobalSettings::instance().setLoginMode("1");

            }

            //登录成功往配置文件中写
            ConfigIni::Write("LoginInformation",QMap<QString,QVariant>{
                                 {"name",ui->nameline->text()},
                                 {"password",ui->passwordine->text()},
                                 {"power",GlobalSettings::instance().getLoginMode()}
                             });
            //SaveTheLog();
            return;
        }
    }
}

void Login::on_exitbtn_clicked()
{
    ExitApp();
}
