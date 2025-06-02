#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "map_graphics_view.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace Didi
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT
        Q_PROPERTY(QString username READ getUsername WRITE setUsername NOTIFY usernameChanged)

    public:
        MainWindow(QWidget *parent = nullptr, const QString& username = QString());
        ~MainWindow();

        QString getUsername() const;
        void setUsername(const QString& username);
        MapGraphicsView* getMap() const;

    signals:
        void usernameChanged();
        void userdataChanged();

    protected:
        void resizeEvent(QResizeEvent *event) override;

    private slots:
        void on_pb_recharge_clicked();

        void on_pb_call_taxi_clicked();

    private:
        Ui::MainWindow *ui;
        QString m_username;

    };

    
}

#endif // MAINWINDOW_H
