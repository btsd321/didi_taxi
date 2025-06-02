#ifndef LOGIN_WIDGET_H
#define LOGIN_WIDGET_H

#include <QWidget>
#include "register_dialog.h"
#include "user_data_manager.h"

namespace Ui
{
    class LoginDialog;
}

namespace Didi
{
    class LoginDialog : public QDialog
    {
        Q_OBJECT

        Q_PROPERTY(QString userName READ getUserName WRITE setUserName)
        Q_PROPERTY(USER_DATA userData READ getUserData WRITE setUserData)

    public:
        explicit LoginDialog(QDialog *parent = nullptr);
        ~LoginDialog();
        QString getUserName() const;
        void setUserName(const QString& userName);
        USER_DATA getUserData() const;
        void setUserData(const USER_DATA& userData);

    private slots:
        void on_login_pushbutton_clicked();

        void on_regist_pushbutton_clicked();

        void regist_dialog_closed(int result);

        void on_cancel_pushbutton_clicked();

    private:
        Ui::LoginDialog *ui;
        RegisterDialog* register_dialog;
        QString m_user_name;
        USER_DATA m_user_data;
    };




}

#endif // LOGIN_WIDGET_H
