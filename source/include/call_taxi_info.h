#ifndef CALL_TAXI_INFO_H
#define CALL_TAXI_INFO_H

#include <QTime>
#include <QPointF>
#include <QSharedDataPointer>
#include "passenger.h"
#include "driver.h"
#include "money.h"

namespace Didi
{
    typedef struct ORDER_INFO_STRU 
    {
        Passenger* passenger; // 乘客
        QPointF passenger_position; // 乘客叫车位置
        QPointF target_position; // 目的地位置
        QTime start_call_taxi_time; // 乘客开始叫车时间

        ORDER_INFO_STRU()  
            : passenger(nullptr)
           , passenger_position(QPointF(0, 0))
           , target_position(QPointF(0, 0))
           , start_call_taxi_time(QTime(0, 0, 0))
        {
        }

        ORDER_INFO_STRU(Passenger* p, QPointF p_pos, QPointF t_pos, QTime s_time, qreal money)
            : passenger(p)
           , passenger_position(p_pos)
           , target_position(t_pos)
           , start_call_taxi_time(s_time)
        {
        }
    }ORDER_INFO;

    typedef struct TAKE_TAXI_DETAIL_STRU : public ORDER_INFO_STRU
    {
        Driver* driver; // 司机
        QPointF driver_position; // 司机开始前去接乘客的位置
        bool need_turn_to_target; // 是否需要拐弯
        QPointF waypoint_to_target; // 拐角路径点
        bool need_turn_to_passenger; // 接乘客路上是否需要拐弯
        QPointF waypoint_to_passenger; // 接乘客路上拐角路径点
        QTime start_take_taxi_time; // 乘客上车时间
        QTime start_moving_time; // 司机开始行驶时间
        QTime end_moving_time; // 司机到达目的地时间
        QTime end_take_taxi_time; // 乘客离开时间
        Money pay_money; // 乘客需要支付的金额

        TAKE_TAXI_DETAIL_STRU()
            : driver(nullptr)
            , driver_position(QPointF(0, 0))
            , need_turn_to_target(false)
            , waypoint_to_target(QPointF(0, 0))
            , need_turn_to_passenger(false)
            , waypoint_to_passenger(QPointF(0, 0))
            , start_take_taxi_time(QTime(0, 0, 0))
            , start_moving_time(QTime(0, 0, 0))
            , end_moving_time(QTime(0, 0, 0))
            , end_take_taxi_time(QTime(0, 0, 0))
            , pay_money(Money("0"))
        {
        }

        TAKE_TAXI_DETAIL_STRU &operator=(TAKE_TAXI_DETAIL_STRU &other)
        {
            this->driver = other.driver;
            this->driver_position = other.driver_position;
            this->need_turn_to_target = other.need_turn_to_target;
            this->waypoint_to_target = other.waypoint_to_target;
            this->need_turn_to_passenger = other.need_turn_to_passenger;
            this->waypoint_to_passenger = other.waypoint_to_passenger;
            this->start_take_taxi_time = other.start_take_taxi_time;
            this->start_moving_time = other.start_moving_time;
            this->end_moving_time = other.end_moving_time;
            this->end_take_taxi_time = other.end_take_taxi_time;
            this->pay_money = other.pay_money;
            this->passenger = other.passenger;
            this->passenger_position = other.passenger_position;
            this->target_position = other.target_position;
            this->start_call_taxi_time = other.start_call_taxi_time;
            return *this;
        }
    }TAKE_TAXI_DETAIL;
}
Q_DECLARE_METATYPE(Didi::TAKE_TAXI_DETAIL*);
Q_DECLARE_METATYPE(QSharedPointer<Didi::TAKE_TAXI_DETAIL>);
#endif // CALL_TAXI_INFO_H
