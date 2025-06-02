#ifndef CHARGE_DIALOG_H
#define CHARGE_DIALOG_H

#include <QDialog>
#include "ui_charge_dialog.h"
#include "user_data_manager.h"

namespace Ui
{
    class ChargeDialog;
}

namespace Didi
{
    typedef enum CHARGE_AMOUNT_TYPE_ENUM
    {
        EM_CHARGE_AMOUNT_DEFAULT = 0,
        EM_CHARGE_AMOUNT_10,
        EM_CHARGE_AMOUNT_20,
        EM_CHARGE_AMOUNT_30,
        EM_CHARGE_AMOUNT_50,
        EM_CHARGE_AMOUNT_100,
        EM_CHARGE_AMOUNT_200,
        EM_CHARGE_AMOUNT_MAX
    } CHARGE_AMOUNT_TYPE;

    class ChargeDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit ChargeDialog(QWidget *parent = nullptr, USER_DATA *user_data = nullptr);
        ~ChargeDialog();

    private slots:
        void on_pb_charge_10_clicked();
        void on_pb_charge_20_clicked();
        void on_pb_charge_30_clicked();
        void on_pb_charge_50_clicked();
        void on_pb_charge_100_clicked();
        void on_pb_charge_200_clicked();

        void on_buttonBox_accepted();

    private:
        Ui::ChargeDialog *ui;
        CHARGE_AMOUNT_TYPE m_charge_amount_type;
        USER_DATA *m_user_data;
    };
}

#endif // CHARGE_DIALOG_H
