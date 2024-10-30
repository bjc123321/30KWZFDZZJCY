#include "login.h"
#include "ui_login.h"

#include <QSettings>
#include "GlobalSettings.h"
#include "Base/BaseFun/Sql/databasemanager.h"
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

    QSqlTableModel *model = DatabaseManager::getInstance(GlobalSettings::sqlPath).getModel();
    model->setTable("login");
    QSqlQueryModel *queryModel = DatabaseManager::getInstance(GlobalSettings::sqlPath).queryRecord(model,"");

    if(queryModel){
        // 输出每一行的数据
        for (int row = 0; row < queryModel->rowCount(); ++row) {

            if(queryModel->data(queryModel->index(row, 1)).toString() == name &&
                    queryModel->data(queryModel->index(row, 2)).toString() == password)
            {

                LoginSuccess();
                if(queryModel->data(queryModel->index(row, 3)).toString() == "2"){

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

}

void Login::on_exitbtn_clicked()
{
    ExitApp();
}
