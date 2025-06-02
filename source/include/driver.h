#ifndef DRIVER_H
#define DRIVER_H

#include <QObject>
#include <QPoint>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include "money.h"

namespace Didi
{
    
    class Driver : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(QPointF pos READ getPos WRITE setPos)
        Q_PROPERTY(qreal size READ getSize WRITE setSize NOTIFY sizeChanged)
        Q_PROPERTY(Money startPrice READ getStartPrice WRITE setStartPrice)
        Q_PROPERTY(Money unitPrice READ getUnitPrice WRITE setUnitPrice)
        Q_PROPERTY(qreal speed READ getSpeed WRITE setSpeed)
        
    public:
        
        // 图标朝向
        typedef enum DIRECTION_ENUM
        {
            EM_DIRECTION_RIGHT = 0,
            EM_DIRECTION_LEFT,
            EM_DIRECTION_MAX
        }DIRECTION;

        // 移动类型
        typedef enum MOVE_TYPE_ENUM
        {
            EM_MOVE_TYPE_IMMEDIATE = 0, // 立刻移动
            EM_MOVE_TYPE_DELAY,        // 延迟移动
            EM_MOVE_TYPE_MAX
        }MOVE_TYPE;

        // 移动方向
        typedef enum MOVE_DIRECTION_ENUM
        {
            EM_MOVE_DIRECTION_UP = 0,
            EM_MOVE_DIRECTION_DOWN,
            EM_MOVE_DIRECTION_LEFT,
            EM_MOVE_DIRECTION_RIGHT,
            EM_MOVE_DIRECTION_MAX
        } MOVE_DIRECTION;

        // 状态
        typedef enum STATE_ENUM
        {
            EM_STATE_STOP_WAIT_ARRIVE = 0,  // 停止，等待乘客上车
            EM_STATE_STOP_WAIT_LEAVE,      // 停止，等待乘客下车
            EM_STATE_WANDER,     // 游荡，等待订单
            EM_STATE_MOVING_TO_TARGET,    // 载人，正在移动
            EM_STATE_MOVING_TO_PASSENGER, // 前往乘客位置
            EM_STATE_MAX
        }STATE;

        explicit Driver(QObject *parent = nullptr);
        ~Driver();

        QPointF getPos() const;
        void setPos(qreal x, qreal y);
        void setPos(QPointF pos);
        
        qreal getSize() const;
        void setSize(qreal size);
        Driver::DIRECTION getDirection() const;
        bool setDirection(Driver::DIRECTION direction);
        Driver::STATE getState() const;
        bool setState(Driver::STATE state);

        QGraphicsPixmapItem* item() const;

        Q_INVOKABLE void moveTo(qreal x, qreal y);
        
        bool addMoveTask(QPointF& new_pos, Driver::MOVE_TYPE type);
        bool removeMoveTask(qreal x, qreal y);
        bool clearMoveTask();
        QList<QPointF>* getMoveTask();

        Money getStartPrice();
        Money getUnitPrice();
        qreal getSpeed();
        void setStartPrice(Money startPrice);
        void setUnitPrice(Money unitPrice);
        void setSpeed(qreal speed);

    private:
        Q_INVOKABLE void moveToNextPosition();
        QGraphicsPixmapItem* m_item;
        qreal m_x;
        qreal m_y;
        qreal m_size;
        Driver::DIRECTION m_direction;
        Driver::STATE m_state;
        QPropertyAnimation *m_posAnimation; // 添加新的动画对象
        QList<QPointF>* m_movePosList; // 存储移动任务路径点
        Money m_startPrice; //起步价
        Money m_unitPrice; //每公里单价
        qreal m_speed; // 速度，单位为像素/秒

    signals:
        void sizeChanged();
        void directionChanged();
        void stateChanged();
        void moveFinished();

    public slots:
        void updatePos(); // 添加一个槽函数来更新位置
    };

}

Q_DECLARE_METATYPE(const Didi::Driver*)
Q_DECLARE_METATYPE(Didi::Driver*)
#endif // DRIVER_H
