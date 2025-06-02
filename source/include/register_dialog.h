#ifndef REGISTER_DIALOG_H
#define REGISTER_DIALOG_H

#include <QDialog>

namespace Ui {
class RegisterDialog;
}

namespace Didi
{
    class RegisterDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit RegisterDialog(QDialog *parent = nullptr);
        ~RegisterDialog();

    private slots:
        void on_pb_regist_clicked();

        void on_pb_cancel_clicked();

    private:
        Ui::RegisterDialog *ui;
    };
}

#endif // REGISTER_DIALOG_H
