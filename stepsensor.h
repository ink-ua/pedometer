#ifndef STEPSENSOR_H
#define STEPSENSOR_H

#include <QAccelerometerReading>
#include <QAccelerometer>
#include <QThread>
#include <QDebug>

QTM_USE_NAMESPACE

class StepsSensor : public QThread {
    Q_OBJECT

private:
    QAccelerometer sensor;
    QAccelerometerReading *reading;

public:
    StepsSensor() {
        sensor.start();
        reading = sensor.reading();
    }

    void run() {
        while(true) {
            usleep(1000);
            qreal x = reading->property("x").value<qreal>();
            qreal y = reading->value(1).value<qreal>();
            qDebug() << x << ", " << y;
            if(x > 100)
               onStep();
        }
    }

signals:
    void onStep();
};

#endif // STEPSENSOR_H
