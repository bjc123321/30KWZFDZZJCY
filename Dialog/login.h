#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "Base/BaseFun/base.h"
#include "Base/BaseFun/ConfigIni/ConfigIni.h"
#include "Base/BaseFun/Sql/sql.h"

//登录名称
extern QString LoginUserName;

//登录权限 0游客 1管理员 2超级管理员
enum AccountPrivileges{AverageUser,Administrator,SuperUser};
extern AccountPrivileges LOGINUSER;

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
public:
    static Login& U(){
        static Login l;
        return l;
    }
public:
    void LoginSuccess();
    void LoginFail();
private slots:
    void on_loginbtn_clicked();

    void on_exitbtn_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
