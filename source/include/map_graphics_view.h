#ifndef MAP_GRAPHICS_VIEW_H
#define MAP_GRAPHICS_VIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QList>
#include <QPoint>

#include "map_manager.h"
#include "call_taxi_info.h"

namespace Didi
{

    
    class MapGraphicsView : public QGraphicsView
    {
        Q_OBJECT
        Q_PROPERTY(qreal mapSize READ getMapSize WRITE setMapSize NOTIFY mapSizeChanged)

    public:
        MapGraphicsView(QWidget *parent = 0);
        ~MapGraphicsView();

        qreal getMapSize() const;
        void setMapSize(qreal size);
        int getMapLength() const;
        qreal getCellSize() const; // 地图格大小

        bool addPassenger(const QString& name, QPointF position);
        bool removePassenger();
        bool setPassenger(const QString& name);
        Passenger* getPassenger() const;

        bool prepareCallTaxi(ORDER_INFO& order_info);

        bool addDriver(qreal x, qreal y);

        QList<Driver*> getDrivers();

        TAKE_TAXI_DETAIL* getTakeTaxiDetail();
    
    signals:
        void mapSizeChanged();
        void passengerChanged(Didi::Passenger* passenger);
        void driverChanged(Didi::Driver* driver);
        void startCallTaxiSignal(QSharedPointer<Didi::TAKE_TAXI_DETAIL> info);
        void passengerGetOffSignal(QPointF pos);

    protected:
        void wheelEvent(QWheelEvent *event) override;

    private:
        void mapInit();
        QGraphicsScene* m_scene;
        qreal m_map_size;
        int m_map_length;

        Passenger* m_passenger;
        QList<Driver*> m_drivers;

        MapManager* m_manager;

        QSharedPointer<Didi::TAKE_TAXI_DETAIL> m_take_taxi_detail;

        bool genarateDetail(ORDER_INFO& order_info, Driver* driver);
    };


}

#endif // MAP_GRAPHICS_VIEW_H
