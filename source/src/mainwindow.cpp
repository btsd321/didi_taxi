#include <QGraphicsLineItem>
#include <QIntValidator>
#include <QMessageBox>
#include <QThread>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log.h"
#include "user_data_manager.h"
#include "charge_dialog.h"
#include "call_taxi_info.h"

namespace Didi
{
    MainWindow::MainWindow(QWidget *parent, const QString &username)
        : QMainWindow(parent),
        ui(new Ui::MainWindow),
        m_username(username)
    {
        ui->setupUi(this);
        setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint); // 允许窗口关闭按钮
        setWindowState(Qt::WindowFullScreen);
        showMaximized(); // 启动时最大化
        USER_DATA user_data = UserDataManager::Instance()->GetUserData(username);
        connect(this, &MainWindow::userdataChanged, this, [this](){
            USER_DATA user_data = UserDataManager::Instance()->GetUserData(m_username);
            ui->money->setText(user_data.money.toQString());
            ui->user_name->setText(m_username);
            ui->graph->setPassenger(m_username);
        });
        connect(ui->graph, &MapGraphicsView::passengerGetOffSignal, this, [this](QPointF pos)->void{
            ui->line_edit_start_x->setText(QString::number(pos.x() / ui->graph->getCellSize()));
            ui->line_edit_start_y->setText(QString::number(pos.y() / ui->graph->getCellSize()));
            auto data = UserDataManager::Instance()->GetUserData(m_username);
            ui->money->setText(data.money.toQString());
        });
        ui->user_name->setText(username);
        ui->money->setText(user_data.money.toQString());
        ui->line_edit_start_x->setValidator(new QIntValidator(0, ui->graph->getMapSize() - 1, this));
        ui->line_edit_start_y->setValidator(new QIntValidator(0, ui->graph->getMapSize() - 1, this));
        ui->line_edit_target_x->setValidator(new QIntValidator(0, ui->graph->getMapSize() - 1, this));
        ui->line_edit_target_y->setValidator(new QIntValidator(0, ui->graph->getMapSize() - 1, this));
        ui->graph->addPassenger(username, QPoint(0, 0)); // 添加乘客
        ui->graph->addDriver(0, 1);
        ui->graph->addDriver(7, 1);
        ui->graph->addDriver(3, 1);

    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }

    void MainWindow::resizeEvent(QResizeEvent *event)
    {
        QWidget::resizeEvent(event);

        // 获取大窗口的原始大小和当前大小
        QSize oldSize = event->oldSize();
        QSize newSize = event->size();

        // 计算缩放比例
        qreal scaleX = static_cast<qreal>(newSize.width()) / oldSize.width();
        qreal scaleY = static_cast<qreal>(newSize.height()) / oldSize.height();

        // 应用缩放比例到 QGraphicsView
        qreal scale_finish = fmin(scaleX, scaleY);
        if (scale_finish > 0)
        {
            ui->graph->scale(scale_finish, scale_finish);
        }
    }

    QString MainWindow::getUsername() const
    {
        return m_username;
    }

    void MainWindow::setUsername(const QString &username)
    {
        if (m_username != username)
        {
            m_username = username;
            emit usernameChanged();
        }
    }

    MapGraphicsView* MainWindow::getMap() const
    {
        return ui->graph;
    }

    void MainWindow::on_pb_recharge_clicked()
    {
        USER_DATA user_data = UserDataManager::Instance()->GetUserData(m_username);
        // 弹出充值对话框
        ChargeDialog charge_dialog(this, &user_data);
        charge_dialog.exec();
        if(UserDataManager::Instance()->UpdateUserData(m_username, user_data))
        {
            emit userdataChanged();
        }
    }

    void MainWindow::on_pb_call_taxi_clicked()
    {
        if(ui->line_edit_start_x->text().isEmpty() || 
            ui->line_edit_start_y->text().isEmpty() || 
            ui->line_edit_target_x->text().isEmpty() || 
            ui->line_edit_target_y->text().isEmpty())
        {
            QMessageBox::warning(this, "警告", "起点或终点坐标不能为空！叫车失败");
            return;
        }
        auto cell_size = ui->graph->getCellSize();
        QPoint start_point(cell_size * ui->line_edit_start_x->text().toInt(), cell_size * ui->line_edit_start_y->text().toInt());
        QPoint target_point(cell_size * ui->line_edit_target_x->text().toInt(), cell_size * ui->line_edit_target_y->text().toInt());
        auto passenger = ui->graph->getPassenger();
        if(passenger == nullptr)
        {
            QMessageBox::warning(this, "警告", "请先向地图中添加乘客！叫车失败");
            return;
        }
        ORDER_INFO order_info;
        order_info.passenger = passenger;
        order_info.passenger_position = start_point;
        order_info.target_position = target_point;
        order_info.start_call_taxi_time = QTime::currentTime();
        if(!ui->graph->prepareCallTaxi(order_info))
        {
            QMessageBox::warning(this, "警告", "准备叫车失败，请检查地图设置！");
            return;
        }
    }



}
