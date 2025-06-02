#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include <QObject>
#include "call_taxi_info.h"
#include "passenger.h"
#include "driver.h"

namespace Didi
{
    typedef enum POS_TYPE_ENUM
    {
        EM_POS_TYPE_NONE = 0,
        EM_POS_TYPE_DRIVER, // 司机接单位置
        EM_POS_TYPE_WAYPOINT_TO_PASSENGER, // 接乘客路上拐角位置
        EM_POS_TYPE_PASSENGER, // 乘客下单位置
        EM_POS_TYPE_WAYPOINT_TO_TARGET, // 乘客到达目的地路上拐角位置
        EM_POS_TYPE_TARGET, // 到达目的地位置
        EM_POS_TYPE_MAX
    }POS_TYPE;

    typedef struct PROCESS_INFO_STRU
    {
        QSharedPointer<Didi::TAKE_TAXI_DETAIL> take_taxi_detail;
        POS_TYPE last_pos;  // 记录最近的位置类型

        PROCESS_INFO_STRU()
            : take_taxi_detail(QSharedPointer<Didi::TAKE_TAXI_DETAIL>(new Didi::TAKE_TAXI_DETAIL()))
            , last_pos(EM_POS_TYPE_NONE)
        {
        }
    }PROCESS_INFO;

    class MapManager : public QObject
    {
        Q_OBJECT

    public:
        explicit MapManager(QObject *parent = nullptr, int map_length = 10);
        ~MapManager();

    private:

        QPointF generateRandomPos(Didi::Driver* driver);
        void wait_passenger_on();// 等待乘客上车
        void wait_passenger_off();// 等待乘客下车
        QSharedPointer<PROCESS_INFO_STRU> m_process_info;
        void pay_money_process();//扣费处理

        int m_map_length;
    signals:
        void passenger_get_off(QPointF pos);

    public slots:
        void passenger_changed_process(Didi::Passenger* passenger);
        void driver_changed_process(Didi::Driver* driver);
        void driver_state_changed_process();
        void driver_move_finished_process();
        void start_call_taxi_process(QSharedPointer<Didi::TAKE_TAXI_DETAIL> info);
    };

    
}

#endif // MAP_MANAGER_H
