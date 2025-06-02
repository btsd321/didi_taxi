#include <QTimer>
#include <QMessageBox>
#include "driver.h"
#include "log.h"


namespace Didi
{

    Driver::Driver(QObject *parent)
        : QObject(parent),
        m_startPrice("5.0"),
        m_unitPrice("2.0"),
        m_speed(100.0), 
        m_item(nullptr), 
        m_size(100), 
        m_direction(DIRECTION::EM_DIRECTION_LEFT),
        m_state(STATE::EM_STATE_WANDER),
        m_movePosList(new QList<QPointF>)
    {
        QPixmap pixmap = QPixmap(":/car.png");
        pixmap = pixmap.scaled(m_size, m_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m_item = new QGraphicsPixmapItem(pixmap);
        m_posAnimation = new QPropertyAnimation(this, "pos");
        m_item->setZValue(5);
        //m_item->setTransformOriginPoint(m_item->boundingRect().width() / 2, m_item->boundingRect().height() / 2);
        // 连接动画的finished信号到一个槽函数，以便在动画结束时更新位置值
        connect(m_posAnimation, &QPropertyAnimation::finished, this, &Driver::updatePos);

    }

    Driver::~Driver()
    {
        if(m_item != nullptr)
        {
            delete m_item;
            m_item = nullptr;
        }
    }

    QPointF Driver::getPos() const
    {
        return QPointF(m_x * m_size, m_y * m_size);
    }

    void Driver::setPos(qreal x, qreal y)
    {
        if((m_x != x) || (m_y != y))
        {
            m_x = x;
            m_y = y;
            m_item->setPos(m_x * m_size, m_y * m_size);
        }
    }
    
    qreal Driver::getSize() const
    {
        return m_size;
    }

    void Driver::setSize(qreal size)
    {
        if (m_size != size)
        {
            m_size = size;
            emit sizeChanged();
        }
    }

    QGraphicsPixmapItem* Driver::item() const
    {
        return m_item;
    }

    Driver::DIRECTION Driver::getDirection() const
    {
        return m_direction;
    }

    bool Driver::setDirection(Driver::DIRECTION direction)
    {
        if (m_direction != direction)
        {
            m_direction = direction;
            //将item左右翻转
            if(m_item!= nullptr)
            {
                QPixmap pixmap = m_item->pixmap();
                //m_item->setTransformOriginPoint(m_item->boundingRect().width() / 2, m_item->boundingRect().height() / 2);
                m_item->setPixmap(pixmap.transformed(QTransform().scale(-1, 1)));
                // 重新设置变换中心

            }
            emit directionChanged();
        }
        return true;
    }

    Driver::STATE Driver::getState() const
    {
        return m_state;
    }

    void Driver::setPos(QPointF pos)
    {
        m_item->setPos(pos);
        m_x = pos.x() / m_size;
        m_y = pos.y() / m_size;
    }

    bool Driver::setState(Driver::STATE state)
    {
        if (m_state != state)
        {
            if((state == STATE::EM_STATE_STOP_WAIT_LEAVE)||
                (state == STATE::EM_STATE_WANDER)||
                (state == STATE::EM_STATE_MOVING_TO_PASSENGER))
            {
                QPixmap pixmap = QPixmap(":/car.png");
                //m_item->setTransformOriginPoint(m_item->boundingRect().width() / 2, m_item->boundingRect().height() / 2);
                pixmap = pixmap.scaled(m_size, m_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                if(m_direction == EM_DIRECTION_LEFT)
                {
                    m_item->setPixmap(pixmap);
                }
                else if(m_direction == EM_DIRECTION_RIGHT)
                {
                    m_item->setPixmap(pixmap.transformed(QTransform().scale(-1, 1)));
                }
                else
                {
                    QLOG_WARN() << "Set Driver State Failed!";
                    return false;
                }
            }
            else if((state == STATE::EM_STATE_STOP_WAIT_ARRIVE)||
                    (state == STATE::EM_STATE_MOVING_TO_TARGET))
            {
                QPixmap pixmap = QPixmap(":/moving_car.png");
                //m_item->setTransformOriginPoint(m_item->boundingRect().width() / 2, m_item->boundingRect().height() / 2);
                pixmap = pixmap.scaled(m_size, m_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                if(m_direction == EM_DIRECTION_LEFT)
                {
                    m_item->setPixmap(pixmap);
                }
                else if(m_direction == EM_DIRECTION_RIGHT)
                {
                    m_item->setPixmap(pixmap.transformed(QTransform().scale(-1, 1)));
                }
                else
                {
                    QLOG_WARN() << "Set Driver State Failed!";
                    return false;
                }
            }
            else
            {
                QLOG_WARN() << "Driver state error";
            }
            m_state = state;
            emit stateChanged();
        }
        return true;
    }

    Money Driver::getStartPrice()
    {
        return m_startPrice;
    }

    Money Driver::getUnitPrice()
    {
        return m_unitPrice;
    }

    qreal Driver::getSpeed()
    {
        return m_speed;
    }

    void Driver::setStartPrice(Money startPrice)
    {
        if(startPrice <= Money("0"))
        {
            QMessageBox::warning(nullptr, "Warning", "Start price cannot be negative or zero!");
            return;
        }
        m_startPrice = startPrice;
    }

    void Driver::setUnitPrice(Money unitPrice)
    {
        if(unitPrice <= Money("0"))
        {
            QMessageBox::warning(nullptr, "Warning", "Unit price cannot be negative or zero!");
            return;
        }
        m_unitPrice = unitPrice;
    }

    void Driver::setSpeed(qreal speed)
    {
        m_speed = speed;
    }

    void Driver::moveTo(qreal x, qreal y)
    {
        // 计算目标位置
        QPointF targetPos(x * m_size, y * m_size);
        QPointF currentPos = item()->pos();
        // 计算距离
        qreal distance = (targetPos - currentPos).manhattanLength();

        if(currentPos.x() < targetPos.x())
        {
            setDirection(Driver::EM_DIRECTION_RIGHT);
        }
        else if(currentPos.x() > targetPos.x())
        {
            setDirection(Driver::EM_DIRECTION_LEFT);
        }
        else
        {

        }
        
        // 设置动画的目标值
        m_posAnimation->setStartValue(QPointF(m_x * m_size, m_y * m_size));
        m_posAnimation->setEndValue(targetPos);
        // 设置动画持续时间，单位为毫秒
        m_posAnimation->setDuration(distance * m_speed / 10);
        // 启动动画
        m_posAnimation->start();
    }

    void Driver::moveToNextPosition()
    {
        if(!m_movePosList->empty())
        {
            QPointF targetPos = m_movePosList->front();
            this->moveTo(targetPos.x() / m_size, targetPos.y() / m_size);
        }
    }

    // 新增的槽函数，用于在动画结束时更新位置值
    void Driver::updatePos()
    {
        QPointF new_pos = item()->pos();
        m_x = new_pos.x() / m_size;
        m_y = new_pos.y() / m_size;
        //m_item->setPos(m_x, m_y);
        for(auto it = m_movePosList->begin(); it != m_movePosList->end(); ++it)
        {
            if(qFuzzyCompare(it->x(), m_x * m_size) && qFuzzyCompare(it->y(), m_y * m_size))
            {
                m_movePosList->erase(it);
                break;
            }
        }
        emit moveFinished();
    }

    bool Driver::addMoveTask(QPointF& new_pos, Driver::MOVE_TYPE type)
    {
        if(m_movePosList == nullptr)
        {
            m_movePosList = new QList<QPointF>;
        }
        if((!m_movePosList->empty())&&(m_movePosList->last() == new_pos))
        {
            QLOG_WARN() << "Move task already exists!";
            return false;
        }
        m_movePosList->append(new_pos);
        if(type == Driver::EM_MOVE_TYPE_IMMEDIATE)
        {
            moveToNextPosition();
        }
        else if(type == Driver::EM_MOVE_TYPE_DELAY)
        {
            connect(this, &Driver::moveFinished, this, &Driver::moveToNextPosition, Qt::UniqueConnection);
        }
        else
        {
            QLOG_WARN() << "Move type error!";
            return false;
        }
        return true;
    }

    bool Driver::removeMoveTask(qreal x, qreal y)
    {
        for(auto it = m_movePosList->begin(); it != m_movePosList->end(); ++it)
        {
            if(qFuzzyCompare(it->x(), x * m_size) && qFuzzyCompare(it->y(), y * m_size))
            {
                m_movePosList->erase(it);
                return true;
            }
        }
        return false;
    }
    
    bool Driver::clearMoveTask()
    {
        m_movePosList->clear();
        return true;
    }

    QList<QPointF>* Driver::getMoveTask()
    {
        return m_movePosList;
    }
}
