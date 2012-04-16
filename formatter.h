#ifndef DISTANCEFORMATTER_H
#define DISTANCEFORMATTER_H

//#include <QDebug>
#include <QObject>

#define METERS_IN_MILE 1609.344
#define METERS_IN_FOOT 0.3048
#define KG_IN_LB 0.45359237

#define METRIC 0
#define IMPERIAL 1

class Formatter : public QObject
{
    Q_OBJECT

    Formatter(QObject* parent =0) : QObject(parent) {
        units = METRIC;
    }

public:

    QString formatDistance(double distance) const {
        QString ret;
        switch(units) {
            case METRIC:
                if(distance > 1000)
                    ret.sprintf("%.2f km", distance / 1000.0);
                else
                    ret.sprintf("%.2f m", distance);
                break;
            case IMPERIAL:
                if(distance > METERS_IN_MILE)
                    ret.sprintf("%.2f mi", distance / METERS_IN_MILE);
                else
                    ret.sprintf("%.1f ft", distance / METERS_IN_FOOT);
                break;
        }
        return ret;
    }

    QString formatSpeed(double speed) const {
        return formatDistance(speed) + "/h";
    }

    QString formatPercent(double v) const {
        QString ret;
        ret.sprintf("%.2f %", v);
        return ret;
    }

    QString formatTime(int seconds) const {
        int sec = seconds % 60;
        int min = (seconds / 60) % 60;
        int hr = (seconds / 3600) % 24;

        QString ret;
        ret.sprintf("%.2d:%.2d:%.2d", hr, min, sec);
        return ret;
    }

    int getUnits() const {
        return units;
    }

    void setUnits(int units) {
        this->units = units;
    }

    static Formatter* getInstance() {
        if(instance == 0) {
            instance = new Formatter();
        }
        return instance;
    }

private:
    int units;
    static Formatter* instance;
};
#endif // DISTANCEFORMATTER_H
