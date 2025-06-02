#ifndef PASSENGER_H
#define PASSENGER_H

#include <QGraphicsPixmapItem>
#include <QPointF>
#include <QScopedPointer>

#include "user_data_manager.h"

namespace Didi
{
    class Passenger : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(QString user_name READ getUserName WRITE setUserName)
        Q_PROPERTY(USER_DATA user_data READ getUserData WRITE setUserData NOTIFY dataChanged)
        Q_PROPERTY(QPointF pos READ getPos WRITE setPos NOTIFY posChanged)
        Q_PROPERTY(qreal size READ getSize WRITE setSize NOTIFY sizeChanged)

    public:
        Passenger(QObject* parent = nullptr, const QString& name = QString());
        ~Passenger();

        QString getUserName();
        void setUserName(const QString& name);
        USER_DATA getUserData();
        void setUserData(const USER_DATA& data);
        QPointF getPos();
        qreal getX() const;
        qreal getY() const;
        void setPos(qreal x, qreal y);
        void setPos(QPointF pos);
        qreal getSize() const;
        void setSize(qreal size);
        bool chargeMoney(Money money);

        bool setUser(const QString& name);

        QGraphicsPixmapItem* item();

    signals:
        void dataChanged();
        void posChanged();
        void sizeChanged();

    private:
        QString m_name; // 乘客姓名
        USER_DATA m_data; // 乘客信息
        qreal m_x;        //乘客位置x坐标
        qreal m_y;        //乘客位置y坐标
        QGraphicsPixmapItem* m_item;
        qreal m_size; // 乘客图标大小

    };

    


}
Q_DECLARE_METATYPE(const Didi::Passenger*)
Q_DECLARE_METATYPE(Didi::Passenger*)
#endif // PASSENGER_H
