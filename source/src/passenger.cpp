#include <QPixmap>
#include "passenger.h"
#include "user_data_manager.h"
#include "log.h"
#include "global.h"

namespace Didi
{
    Passenger::Passenger(QObject *parent, const QString &name)
        : m_name(name),
        m_size(100),
        m_x(0),
        m_y(0)
    {
        m_item = new QGraphicsPixmapItem(nullptr);
        m_data = UserDataManager::Instance()->GetUserData(name);
        setPos(0, 0);
        // 读取乘客图像
        QPixmap pixmap(":/passenger.png");
        // 获取图片宽高将其缩小
        pixmap = pixmap.scaled(m_size, m_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        
        if (pixmap.isNull())
        {
            QLOG_ERROR() << "Passenger image not found: %s", qPrintable(name);
        }
        else
        {
            m_item->setPixmap(pixmap);
        }
    }

    Passenger::~Passenger()
    {
        delete m_item;
    }

    QString Passenger::getUserName()
    {
        return m_name;
    }

    void Passenger::setUserName(const QString &name)
    {
        if (m_name != name)
        {
            m_name = name;
        }
    }

    USER_DATA Passenger::getUserData()
    {
        return m_data;
    }

    void Passenger::setUserData(const USER_DATA &data)
    {
        if (m_data != data)
        {
            m_data = data;
            emit dataChanged();
        }
    }

    QPointF Passenger::getPos()
    {
        return QPointF(m_x * m_size, m_y * m_size);
    }

    void Passenger::setPos(qreal x, qreal y)
    {
        if((m_x != x) || (m_y != y))
        {
            m_x = x;
            m_y = y;
            m_item->setPos(m_x * m_size, m_y * m_size);
            emit posChanged();
        }
    }

    qreal Passenger::getSize() const
    {
        return m_size;
    }

    void Passenger::setSize(qreal size)
    {
        if (m_size != size)
        {
            m_size = size;
            emit sizeChanged();
        }
    }

    QGraphicsPixmapItem* Passenger::item()
    {
        return this->m_item;
    }

    bool Passenger::setUser(const QString& name)
    {
        USER_DATA new_data = UserDataManager::Instance()->GetUserData(name);
        if(new_data == USER_DATA())
        {
            return false;
        }
        setUserName(name);
        setUserData(new_data);
        return true;
    }

    qreal Passenger::getX() const
    {
        return m_x;
    }

    qreal Passenger::getY() const
    {
        return m_y;
    }

    void Passenger::setPos(QPointF pos)
    {
        qreal x = pos.x() / m_size;
        qreal y = pos.y() / m_size;
        if((m_x != x) || (m_y != y))
        {
            m_x = x;
            m_y = y;
            m_item->setPos(pos);
            emit posChanged();
        }
    }

    bool Passenger::chargeMoney(Money money)
    {
        this->m_data.money += money;
        emit dataChanged();
        return true;
    }
}
