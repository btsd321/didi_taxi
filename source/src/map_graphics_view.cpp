#include <QMessageBox>
#include "map_graphics_view.h"
#include "global.h"
#include "log.h"

namespace Didi
{
    #define DEFAULT_MAP_LENGTH 12
    #define DEFAULT_CELL_SIZE 100

    MapGraphicsView::MapGraphicsView(QWidget *parent)
        : QGraphicsView(parent), 
        m_passenger(nullptr),
        m_map_length(DEFAULT_MAP_LENGTH),
        m_take_taxi_detail(nullptr)
    {
        m_map_size = m_map_length * DEFAULT_CELL_SIZE;
        m_manager = new MapManager(this);
        connect(this, &MapGraphicsView::passengerChanged, m_manager, &MapManager::passenger_changed_process);
        connect(this, &MapGraphicsView::driverChanged, m_manager, &MapManager::driver_changed_process);
        connect(this, &MapGraphicsView::startCallTaxiSignal, m_manager, &MapManager::start_call_taxi_process);
        mapInit();

        connect(m_manager, &MapManager::passenger_get_off, this, [this](QPointF pos) -> void{
            emit passengerGetOffSignal(pos);
        });
    }

    MapGraphicsView::~MapGraphicsView()
    {
        
    }

    void MapGraphicsView::mapInit()
    {
        setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        setDragMode(QGraphicsView::ScrollHandDrag);
        m_scene = new QGraphicsScene(this);

        for(int i=0;i < m_map_length + 1; i++)
        {
            QGraphicsLineItem *line_y = new QGraphicsLineItem(m_map_size/m_map_length*i,0,m_map_size/m_map_length*i,m_map_size);
            QGraphicsLineItem *line_x = new QGraphicsLineItem(0,m_map_size/m_map_length*i,m_map_size,m_map_size/m_map_length*i);
            line_y->setZValue(10.0);
            line_x->setZValue(10.0);
            m_scene->addItem(line_x);
            m_scene->addItem(line_y);
        }
        this->scale(5000.0/m_map_size/m_map_length,5000.0/m_map_size/m_map_length);

        this->setScene(m_scene);
        this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    }

    qreal MapGraphicsView::getMapSize() const
    {
        return m_map_size;
    }

    void MapGraphicsView::setMapSize(qreal size)
    {
        m_map_size = size;
    }

    qreal MapGraphicsView::getCellSize() const
    {
        return m_map_size / m_map_length;
    }

    Passenger* MapGraphicsView::getPassenger() const
    {
        return m_passenger;
    }

    void MapGraphicsView::wheelEvent(QWheelEvent *event)
    {
        if (event->angleDelta().y() > 0)
        {
            scale(1.15, 1.15); // 放大
        }
        else
        {
            scale(1.0 / 1.15, 1.0 / 1.15); // 缩小
        }
    }

    bool MapGraphicsView::addPassenger(const QString& name, QPointF position)
    {
        if(m_passenger!= nullptr)
        {
            QLOG_WARN() << "Passenger already exists!";
            return false;
        }
        m_passenger = new Passenger(this, name);
        m_passenger->setPos(position);
        m_scene->addItem(m_passenger->item());
        emit passengerChanged(m_passenger);
        return true;
    }

    bool MapGraphicsView::removePassenger()
    {
        if(m_passenger == nullptr)
        {
            QLOG_WARN() << "Passenger not exists!";
            return false;
        }
        delete m_passenger;
        m_passenger = nullptr;
        return true;
    }

    bool MapGraphicsView::setPassenger(const QString& name)
    {
        if(m_passenger == nullptr)
        {
            m_passenger = new Passenger(this, name);
        }
        if(m_passenger->setUser(name))
        {
            emit passengerChanged(m_passenger);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool MapGraphicsView::addDriver(qreal x, qreal y)
    {
        Driver* new_driver = new Driver(this);
        new_driver->setPos(x,y);
        m_drivers.append(new_driver);
        m_scene->addItem(new_driver->item());
        emit driverChanged(new_driver);
        return true;
    }

    QList<Driver*> MapGraphicsView::getDrivers()
    {
        return m_drivers;
    }

    int MapGraphicsView::getMapLength() const
    {
        return m_map_length;
    }

    bool MapGraphicsView::genarateDetail(ORDER_INFO& order_info, Driver* driver)
    {
        // 计算费用
        qreal distance = qAbs(order_info.passenger_position.x() - order_info.target_position.x())
                        + qAbs(order_info.passenger_position.y() - order_info.target_position.y());
        int64_t cell_count = distance / driver->getSize(); 
        if(distance / driver->getSize() != cell_count)
        {
            QMessageBox::information(this, "提示", "Driver size不合适，影响价格计算，请调整！");
            return false;
        }
        Money fee = driver->getStartPrice() + driver->getUnitPrice() * cell_count;
        if(order_info.passenger->getUserData().money < fee)
        {
            QMessageBox::information(this, "提示", "余额不足，请充值后再叫车！");
            return false;
        }

        m_take_taxi_detail.reset(new TAKE_TAXI_DETAIL());
        m_take_taxi_detail->passenger = order_info.passenger;
        m_take_taxi_detail->passenger_position = order_info.passenger_position;
        m_take_taxi_detail->target_position = order_info.target_position;
        m_take_taxi_detail->start_call_taxi_time = QTime::currentTime();
        m_take_taxi_detail->driver = driver;
        auto driver_move_list = driver->getMoveTask();
        if(driver_move_list->isEmpty())
        {
            m_take_taxi_detail->driver_position = driver->getPos();
        }
        else
        {
            m_take_taxi_detail->driver_position = driver_move_list->last();
        }
        if((order_info.passenger_position.x() != order_info.target_position.x())
            && (order_info.passenger_position.y() != order_info.target_position.y()))
        {
            m_take_taxi_detail->need_turn_to_target = true;
            m_take_taxi_detail->waypoint_to_target = QPointF(order_info.passenger_position.x(),
               order_info.target_position.y());
        }
        else
        {
            m_take_taxi_detail->need_turn_to_target = false;
        }
        if((order_info.passenger_position.x() != m_take_taxi_detail->driver_position.x())
            && (order_info.passenger_position.y() != m_take_taxi_detail->driver_position.y()))
        {
            m_take_taxi_detail->need_turn_to_passenger = true;
            m_take_taxi_detail->waypoint_to_passenger = QPointF(order_info.passenger_position.x(),
               m_take_taxi_detail->driver_position.y());
        }
        else
        {
            m_take_taxi_detail->need_turn_to_passenger = false;
        }
        m_take_taxi_detail->pay_money = fee;
        return true;
    }

    bool MapGraphicsView::prepareCallTaxi(ORDER_INFO& order_info)
    {
        if(order_info.passenger == nullptr)
        {
            QLOG_WARN() << "Passenger not exists!";
            return false;
        }
        auto passenger_pos = order_info.passenger->getPos();

        // 查找最近的司机
        Driver* nearest_driver = nullptr;
        QList<Driver*> idle_drivers;
        for(auto& driver : m_drivers)
        {
            if(driver->getState() == Driver::EM_STATE_WANDER)
            {
                idle_drivers.append(driver);
            }
        }
        qreal min_distance = std::numeric_limits<qreal>::max();
        if(idle_drivers.isEmpty())
        {
            QLOG_WARN() << "No drivers available!";
            return false;
        }
        for(Driver* driver : idle_drivers)
        {
            auto driver_pos = driver->getPos();
            qreal distance = (driver_pos - passenger_pos).manhattanLength();
            if(distance < min_distance)
            {
                min_distance = distance;
                nearest_driver = driver;
            }
        }

        if(genarateDetail(order_info, nearest_driver))
        {
            order_info.passenger->setPos(order_info.passenger_position);
            emit startCallTaxiSignal(m_take_taxi_detail);
            return true;
        }
        else
        {
            return false;
        }

    }

    TAKE_TAXI_DETAIL* MapGraphicsView::getTakeTaxiDetail()
    {
        return m_take_taxi_detail.data();
    }
}
