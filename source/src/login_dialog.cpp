#include <QMessageBox>
#include "ui_login_dialog.h"
#include "login_dialog.h"
#include "user_data_manager.h"
#include "register_dialog.h"
#include "mainwindow.h"

namespace Didi
{
    LoginDialog::LoginDialog(QDialog *parent)
        : QDialog(parent), ui(new Ui::LoginDialog)
    {
        ui->setupUi(this);

        ui->line_edit_password->setEchoMode(QLineEdit::Password);
        register_dialog = nullptr;
    }

    LoginDialog::~LoginDialog()
    {
        if(register_dialog != nullptr)
        {
            register_dialog->close();
            delete register_dialog;
        }
        delete ui;
    }

    void LoginDialog::on_login_pushbutton_clicked()
    {
        // 读取line_edit_user_name的内容
        m_user_name = ui->line_edit_user_name->text();
        if(!UserDataManager::Instance()->IsExist(m_user_name))
        {
            // 用户名不存在，弹出窗口提醒用户注册
            QMessageBox::information(this, "用户名不存在", "该用户名不存在，请注册新用户。");
            return;
        }
        else
        {
            // 判断密码
            // 获取输入的密码
            auto intput_password = ui->line_edit_password->text();
            auto user_data = UserDataManager::Instance()->GetUserData(m_user_name);
            if(user_data.password == intput_password)
            {
                emit accepted();
            }
            else
            {
                QMessageBox::information(this, "密码错误", "密码错误，请重新输入。");
            }
        }
    }

    void LoginDialog::on_regist_pushbutton_clicked()
    {
        // 弹出注册窗口
        if(register_dialog != nullptr)
        {
            register_dialog->close();
            delete register_dialog;
            register_dialog = nullptr;
        }

        // 禁用登录窗口
        this->setEnabled(false);

        register_dialog = new RegisterDialog(nullptr);

        // 连接注册窗口的 finished 信号到 regist_dialog_closed 槽
        connect(register_dialog, &RegisterDialog::finished, this, &LoginDialog::regist_dialog_closed, Qt::SingleShotConnection);
        
        register_dialog->show();
    }

    void LoginDialog::regist_dialog_closed(int result)
    {
        // 重新启用登录窗口
        this->setEnabled(true);

        // 删除注册窗口实例
        if (register_dialog != nullptr)
        {
            delete register_dialog;
            register_dialog = nullptr;
        }
    }

    QString LoginDialog::getUserName() const
    {
        return this->m_user_name;
    }

    void LoginDialog::setUserName(const QString& userName)
    {
        this->m_user_name = userName;
    }

    USER_DATA LoginDialog::getUserData() const
    {
        return this->m_user_data;
    }

    void LoginDialog::setUserData(const USER_DATA& userData)
    {
        this->m_user_data = userData;
    }

    void LoginDialog::on_cancel_pushbutton_clicked()
    {
        // 关闭登录窗口
        this->close();
        this->deleteLater();
    }


}
