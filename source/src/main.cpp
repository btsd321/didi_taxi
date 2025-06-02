#include <windows.h>
#include <iostream>
#include <QApplication>
#include <QDir>
#include <QThread>
#include "login_dialog.h"
#include "log.h"
#include "global.h"
#include "user_data_manager.h"
#include "mainwindow.h"
#include "map_manager.h"

namespace Didi
{
    void RegistMetaType()
    {
        qRegisterMetaType<const Didi::Driver*>("const Didi::Driver*");
        qRegisterMetaType<const Didi::Passenger*>("const Didi::Passenger*");
        qRegisterMetaType<Didi::Driver*>("Didi::Driver*");
        qRegisterMetaType<Didi::Passenger*>("Didi::Passenger*");
        qRegisterMetaType<Didi::TAKE_TAXI_DETAIL*>("Didi::TAKE_TAXI_DETAIL*");
        qRegisterMetaType<QSharedPointer<Didi::TAKE_TAXI_DETAIL>>("QSharedPointer<Didi::TAKE_TAXI_DETAIL>");
    }
}

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    QApplication a(argc, argv);
    //日志初始化
    Didi::CLogFunction::Init();
    // 自定义类型初始化
    Didi::RegistMetaType();

    if(!Didi::UserDataManager::Instance()->LoadUserData())
    {
        QLOG_WARN() << "载入用户信息失败！";
    }

    Didi::LoginDialog* login_dialog = new Didi::LoginDialog();
    login_dialog->show();
    a.connect(login_dialog, &Didi::LoginDialog::accepted, &a, [&]() {
        login_dialog->close();
        Didi::MainWindow* main_window = new Didi::MainWindow(nullptr, login_dialog->getUserName());
        main_window->show();
        login_dialog->deleteLater();

    });

    // Didi::MapGraphicsView mapView;
    // mapView.addDriver(0, 0); // 添加一个司机在 (0, 0) 位置
    // mapView.addDriver(3, 3);
    // mapView.addPassenger("111", QPointF(0,0)); // 添加一个乘客在 (1, 1) 位置

    // mapView.show();

    return a.exec();
}
