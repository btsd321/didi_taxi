
#include "charge_dialog.h"
#include "log.h"

namespace Didi
{
    ChargeDialog::ChargeDialog(QWidget *parent, USER_DATA *user_data) : QDialog(parent),
                                                                        ui(new Ui::ChargeDialog),
                                                                        m_charge_amount_type(EM_CHARGE_AMOUNT_DEFAULT),
                                                                        m_user_data(user_data)
    {
        ui->setupUi(this);
    }

    ChargeDialog::~ChargeDialog()
    {
        delete ui;
    }

    void ChargeDialog::on_pb_charge_10_clicked()
    {
        m_charge_amount_type = EM_CHARGE_AMOUNT_10;
    }

    void ChargeDialog::on_pb_charge_20_clicked()
    {
        m_charge_amount_type = EM_CHARGE_AMOUNT_20;
    }

    void ChargeDialog::on_pb_charge_30_clicked()
    {
        m_charge_amount_type = EM_CHARGE_AMOUNT_30;
    }

    void ChargeDialog::on_pb_charge_50_clicked()
    {
        m_charge_amount_type = EM_CHARGE_AMOUNT_50;
    }

    void ChargeDialog::on_pb_charge_100_clicked()
    {
        m_charge_amount_type = EM_CHARGE_AMOUNT_100;
    }

    void ChargeDialog::on_pb_charge_200_clicked()
    {
        m_charge_amount_type = EM_CHARGE_AMOUNT_200;
    }

    void ChargeDialog::on_buttonBox_accepted()
    {
        if (m_user_data == nullptr)
        {
            QLOG_ERROR() << "乘客数据为空个，无法进行充值操作";
            return;
        }
        switch (m_charge_amount_type)
        {
            case EM_CHARGE_AMOUNT_10:
            {
                m_user_data->money += Money("10");
            }
            break;
            case EM_CHARGE_AMOUNT_20:
            {
                m_user_data->money += Money("20");
            }
            break;
            case EM_CHARGE_AMOUNT_30:
            {
                m_user_data->money += Money("30");
            }
            break;
            case EM_CHARGE_AMOUNT_50:
            {
                m_user_data->money += Money("50");
            }
            break;
            case EM_CHARGE_AMOUNT_100:
            {
                m_user_data->money += Money("100");
            }
            break;
            case EM_CHARGE_AMOUNT_200:
            {
                m_user_data->money += Money("200");
            }
            break;
            default:
            {
                QLOG_ERROR() << "未知的充值金额类型，无法进行充值操作";
                return;
            }
            break;
        }
        return;
    }

}
