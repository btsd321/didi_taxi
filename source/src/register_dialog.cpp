#include <QMessageBox.h>
#include "register_dialog.h"
#include "ui_register_dialog.h"
#include "user_data_manager.h"
#include "log.h"

namespace Didi
{
    RegisterDialog::RegisterDialog(QDialog *parent)
        : QDialog(parent), ui(new Ui::RegisterDialog)
    {
        ui->setupUi(this);

        ui->line_edit_new_password->setEchoMode(QLineEdit::Password);
        ui->line_edit_confirm_password->setEchoMode(QLineEdit::Password);
    }

    RegisterDialog::~RegisterDialog()
    {
        delete ui;
    }

    void RegisterDialog::on_pb_regist_clicked()
    {
        // 读取用户名，密码和再次确认密码
        QString new_user_name = ui->line_edit_new_user_name->text();
        QString new_password = ui->line_edit_new_password->text();
        QString confirm_password = ui->line_edit_confirm_password->text();
        if(UserDataManager::Instance()->IsExist(new_user_name))
        {
            QMessageBox::information(this, "用户名已注册", "该用户名已被注册，请更换用户名。");
            return;
        }

        // 验证两次密码是否一致
        if (new_password != confirm_password)
        {
            QMessageBox::information(this, "两次密码不一致", "两次输入的密码不一致，请重新输入。");
            return;
        }

        USER_DATA user_data;
        user_data.password = new_password;
        user_data.money = Money("0");

        if(!UserDataManager::Instance()->AddUserData(new_user_name, user_data))
        {
            QLOG_WARN() << "注册失败";
        }

        // 关闭窗口
        close();
        
    }

    void RegisterDialog::on_pb_cancel_clicked()
    {
        //关闭窗口
        close();
        
    }


}
