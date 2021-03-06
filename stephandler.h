#ifndef STEPSENSOR_H
#define STEPSENSOR_H

#include <QAccelerometerFilter>
//#include <QDebug>
#include <QDateTime>
#include <math.h>

#include "appcontroller.h"

QTM_USE_NAMESPACE

class StepHandler : public QObject, public QAccelerometerFilter {
    Q_OBJECT

private:
    float mYOffset;
    float mScale;
    float mLastValue;
    short mLastDirection;
    float mLowerLimit;//, mUpperLimit;
    float mLastDiff;
    int mLastMatch;
    float mLastExtremes[2];
//    quint64 mTime;
    quint64 mExtremeTime[2];

public:
    StepHandler() {
        mYOffset = 240;
        mScale = -8;
        mLowerLimit = 10;
        //mUpperLimit = 500;
        mLastMatch = -1;

        mLastValue = 0;
    }

    bool filter(QAccelerometerReading *reading) {
        qreal values[3] = { reading->x(), reading->y(), reading->z() };

        quint64 time = QDateTime::currentMSecsSinceEpoch();

        qreal vSum = 0;
        for(int i = 0; i < 3; i++)
            vSum += mYOffset + values[i] * mScale;
        float v = vSum * (AppController::getInstance())->getSensitivity();

        short direction = (v > mLastValue ? 1 : (v < mLastValue ? -1 : 0));
        if (direction == -mLastDirection) {
            //qDebug() << "direction changed";
            short extType = (direction > 0 ? 0 : 1); // minumum or maximum?
            mLastExtremes[extType] = mLastValue;

            float diff = fabs(mLastExtremes[extType] - mLastExtremes[1 - extType]);

            if(time - mExtremeTime[extType] > 50 && time - mExtremeTime[1 - extType] > 50) {
                    quint64 timeDiff = mExtremeTime[1 - extType] - mExtremeTime[extType];

                    //qDebug() << diff << (diff > mLowerLimit) /*<< (diff < mUpperLimit)*/ << (timeDiff > 100 / appController->getSensitivity()) << (timeDiff < 1000);
                    if (diff > mLowerLimit /*&& diff < mUpperLimit*/ && timeDiff > 100 / AppController::getInstance()->getSensitivity() && timeDiff < 1000) {
                        bool isAlmostAsLargeAsPrevious = diff > (mLastDiff * 0.4);
                        bool isPreviousLargeEnough = mLastDiff > (diff * 0.2);
                        bool isNotContra = (mLastMatch != 1 - extType);

                        //qDebug() << "limit passed" << isAlmostAsLargeAsPrevious << isPreviousLargeEnough << isNotContra;
                        if (isAlmostAsLargeAsPrevious && isPreviousLargeEnough && isNotContra) {
                            //qDebug() << timeDiff;
                            emit onStep();
                            mLastMatch = extType;
                        }
                        else
                            mLastMatch = -1;
                    }
                }
            if(time - mExtremeTime[extType] > 100)
                mExtremeTime[extType] = time;

            mLastDiff = diff;
        }
        mLastDirection = direction;
        mLastValue = v;
        return false;
    }

signals:
    void onStep();
};

#endif // STEPSENSOR_H
