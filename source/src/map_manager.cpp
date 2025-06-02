#include <QRandomGenerator>
#include <QThread>
#include "map_manager.h"
#include "call_taxi_info.h"
#include "log.h"

namespace Didi
{
    MapManager::MapManager(QObject *parent, int map_length)
        : QObject(parent), m_map_length(map_length)
    {
    }

    MapManager::~MapManager()
    {
        // 清理工作，如果有需要的话
    }

    void MapManager::passenger_changed_process(Didi::Passenger *passenger)
    {
    }

    void MapManager::driver_changed_process(Didi::Driver *driver)
    {
        connect(driver, &Driver::stateChanged, this, &MapManager::driver_state_changed_process);
        connect(driver, &Driver::moveFinished, this, &MapManager::driver_move_finished_process);
        if (driver->getState() == Driver::EM_STATE_WANDER)
        {
            // 游荡状态下添加一个移动任务
            driver->clearMoveTask();
            QPointF target_pos = generateRandomPos(driver);
            driver->addMoveTask(target_pos, Driver::EM_MOVE_TYPE_IMMEDIATE);
        }
    }

    QPointF MapManager::generateRandomPos(Didi::Driver *driver)
    {
        auto current_pos = driver->getPos();
        int current_x = qRound(current_pos.x() / driver->getSize());
        int current_y = qRound(current_pos.y() / driver->getSize());

        QList<Driver::MOVE_DIRECTION> valid_move_direction_list;
        if (current_y > 0)
        {
            valid_move_direction_list.append(Driver::EM_MOVE_DIRECTION_UP);
        }
        if (current_y < m_map_length - 1)
        {
            valid_move_direction_list.append(Driver::EM_MOVE_DIRECTION_DOWN);
        }
        if (current_x > 0)
        {
            valid_move_direction_list.append(Driver::EM_MOVE_DIRECTION_LEFT);
        }
        if (current_x < m_map_length - 1)
        {
            valid_move_direction_list.append(Driver::EM_MOVE_DIRECTION_RIGHT);
        }
        if (valid_move_direction_list.empty())
        {
            QLOG_WARN() << "No valid move direction";
            return QPointF(0, 0);
        }

        // 从valid_move_direction_list中随机选择一个方向
        int random_index = QRandomGenerator::global()->bounded(valid_move_direction_list.size());
        auto move_direction = valid_move_direction_list.at(random_index);

        QPointF target_pos;
        if (move_direction == Driver::EM_MOVE_DIRECTION_UP)
        {
            target_pos = QPointF(current_pos.x(), current_pos.y() - 1 * driver->getSize());
        }
        else if (move_direction == Driver::EM_MOVE_DIRECTION_DOWN)
        {
            target_pos = QPointF(current_pos.x(), current_pos.y() + 1 * driver->getSize());
        }
        else if (move_direction == Driver::EM_MOVE_DIRECTION_LEFT)
        {
            target_pos = QPointF(current_pos.x() - 1 * driver->getSize(), current_pos.y());
        }
        else if (move_direction == Driver::EM_MOVE_DIRECTION_RIGHT)
        {
            target_pos = QPointF(current_pos.x() + 1 * driver->getSize(), current_pos.y());
        }
        else
        {
            QLOG_WARN() << "Move direction error";
            return QPointF(0, 0);
        }
        return target_pos;
    }

    void MapManager::driver_state_changed_process()
    {
        Didi::Driver *driver = qobject_cast<Didi::Driver *>(sender());
        if (!driver)
        {
            QLOG_WARN() << "Driver state changed process error, driver is null";
            return;
        }
    }

    void MapManager::driver_move_finished_process()
    {
        Didi::Driver *driver = qobject_cast<Didi::Driver *>(sender());
        if (!driver)
        {
            QLOG_WARN() << "Driver move finished process error, driver is null";
            return;
        }
        if (driver->getState() == Driver::EM_STATE_WANDER)
        {
            driver->clearMoveTask();
            QPointF target_pos = generateRandomPos(driver);
            driver->addMoveTask(target_pos, Driver::EM_MOVE_TYPE_IMMEDIATE);
        }
        else if(driver->getState() == Driver::EM_STATE_MOVING_TO_PASSENGER)
        {
            if(m_process_info->last_pos == EM_POS_TYPE_NONE)
            {
                m_process_info->last_pos = EM_POS_TYPE_DRIVER;
            }
            else if(m_process_info->last_pos == EM_POS_TYPE_DRIVER)
            {
                if(m_process_info->take_taxi_detail->need_turn_to_passenger)
                {
                    m_process_info->last_pos = EM_POS_TYPE_WAYPOINT_TO_PASSENGER;
                    driver->addMoveTask(m_process_info->take_taxi_detail->passenger_position, Driver::EM_MOVE_TYPE_IMMEDIATE);
                }
                else
                {
                    m_process_info->last_pos = EM_POS_TYPE_PASSENGER;
                    wait_passenger_on();
                }
            }
            else if(m_process_info->last_pos == EM_POS_TYPE_WAYPOINT_TO_PASSENGER)
            {
                m_process_info->last_pos = EM_POS_TYPE_PASSENGER;
                wait_passenger_on();
            }
            else
            {
                QLOG_WARN() << "Driver move finished process error, last_pos is wrong";
                return;
            }
        }
        else if(driver->getState() == Driver::EM_STATE_MOVING_TO_TARGET)
        {
            if(m_process_info->last_pos == EM_POS_TYPE_PASSENGER)
            {
                if(m_process_info->take_taxi_detail->need_turn_to_target)
                {
                    m_process_info->last_pos = EM_POS_TYPE_WAYPOINT_TO_TARGET;
                    driver->addMoveTask(m_process_info->take_taxi_detail->target_position, Driver::EM_MOVE_TYPE_IMMEDIATE);
                }
                else
                {
                    m_process_info->last_pos = EM_POS_TYPE_TARGET;
                    wait_passenger_off();
                }
            }
            else if(m_process_info->last_pos == EM_POS_TYPE_WAYPOINT_TO_TARGET)
            {
                m_process_info->last_pos = EM_POS_TYPE_TARGET;
                wait_passenger_off();
            }
            else
            {
                QLOG_WARN() << "Driver move finished process error, last_pos is wrong";
                return;
            }
        }
    }

    void MapManager::start_call_taxi_process(QSharedPointer<Didi::TAKE_TAXI_DETAIL> info)
    {
        if(m_process_info.isNull())
        {
            m_process_info.reset(new PROCESS_INFO_STRU());
        }
        m_process_info->last_pos = EM_POS_TYPE_NONE;
        *(m_process_info->take_taxi_detail) = *info;
        if (m_process_info->take_taxi_detail == nullptr)
        {
            QLOG_WARN() << "Start call taxi process error, detail is null";
            return;
        }

        // 通知司机前往接乘客
        auto& called_driver = m_process_info->take_taxi_detail->driver;
        called_driver->setState(Driver::EM_STATE_MOVING_TO_PASSENGER);
        called_driver->clearMoveTask();
        if(m_process_info->take_taxi_detail->need_turn_to_passenger)
        {
            called_driver->addMoveTask(m_process_info->take_taxi_detail->waypoint_to_passenger, Driver::EM_MOVE_TYPE_DELAY);
        }
        else
        {
            called_driver->addMoveTask(m_process_info->take_taxi_detail->passenger_position, Driver::EM_MOVE_TYPE_DELAY);
        }
    }

    void MapManager::wait_passenger_on()
    {
        auto& called_driver = m_process_info->take_taxi_detail->driver;
        auto passenger = m_process_info->take_taxi_detail->passenger;
        passenger->item()->setVisible(false);
        called_driver->setState(Driver::EM_STATE_STOP_WAIT_ARRIVE);
        QThread::msleep(1000);
        called_driver->setState(Driver::EM_STATE_MOVING_TO_TARGET);
        if(m_process_info->take_taxi_detail->need_turn_to_target)
        {
            called_driver->addMoveTask(m_process_info->take_taxi_detail->waypoint_to_target, Driver::EM_MOVE_TYPE_IMMEDIATE);
        }
        else
        {
            called_driver->addMoveTask(m_process_info->take_taxi_detail->target_position, Driver::EM_MOVE_TYPE_IMMEDIATE);
        }
    }

    void MapManager::wait_passenger_off()
    {
        auto& called_driver = m_process_info->take_taxi_detail->driver;
        auto passenger = m_process_info->take_taxi_detail->passenger;
        called_driver->setState(Driver::EM_STATE_STOP_WAIT_LEAVE);
        passenger->setPos(m_process_info->take_taxi_detail->target_position);
        passenger->item()->setVisible(true);
        QThread::msleep(1000);
        pay_money_process(); // 扣费处理
        emit passenger_get_off(passenger->getPos());
        called_driver->setState(Driver::EM_STATE_WANDER);
        called_driver->clearMoveTask();
        QPointF target_pos = generateRandomPos(called_driver);
        called_driver->addMoveTask(target_pos, Driver::EM_MOVE_TYPE_IMMEDIATE);
        called_driver->item()->setVisible(true);
    }

    void MapManager::pay_money_process()
    {
        auto& payer = m_process_info->take_taxi_detail->passenger;
        auto fee = m_process_info->take_taxi_detail->pay_money;
        auto payer_data = payer->getUserData();
        if (payer_data.money < fee)
        {
            QLOG_WARN() << "Passenger does not have enough money to pay, current money:" << payer_data.money.toQString() << ", required fee:" << fee.toQString();
            return;
        }
        else
        {
            payer_data.money -= fee;
            payer->setUserData(payer_data);
            UserDataManager::Instance()->UpdateUserData(payer->getUserName(), payer_data);
            QLOG_INFO() << "Payment successful, remaining money:" << payer_data.money.toQString();
        }
    }
}
