#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "Base/BaseFun/base.h"
#include "Base/BaseFun/ConfigIni/ConfigIni.h"

//登录名称
extern QString LoginUserName;

//登录权限 1用户 2超级管理员
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

    //默认权限为普通用户
    bool isAdmin = false;
public:
    void LoginSuccess();
    void ExitApp();
private slots:
    void on_loginbtn_clicked();

    void on_exitbtn_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
