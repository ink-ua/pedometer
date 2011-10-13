#ifndef STEPSENSOR_H
#define STEPSENSOR_H

#include <QAccelerometerFilter>
#include <QDebug>
#include <math.h>

//#define RADIANS_TO_DEGREES 57.2957795

QTM_USE_NAMESPACE

class StepHandler : public QObject, public QAccelerometerFilter {
    Q_OBJECT

private:
    float mYOffset;
    float mScale;
    float mLastValues[3*2];
    float mLastDirections[3*2];
    float mLimit;
    float mLastDiff[3*2];
    int mLastMatch;
    float mLastExtremes[3*2][3*2];

public:
    StepHandler() : x(0), y(0), z(0), t(0) {
        int h = 480; // TODO: remove this constant
        mYOffset = h * 0.5f;
        mScale = -(h * 0.5f * (1.0f / 10)); //SensorManager.MAGNETIC_FIELD_EARTH_MAX
        mLimit = 10;
        mLastMatch = -1;
    }

    bool filter(QAccelerometerReading *reading) {
        qreal values[3] = { reading->x(), reading->y(), reading->z() };

        qreal vSum = 0;
        for(int i = 0; i < 3; i++)
            vSum += mYOffset + values[i] * mScale;

        int k = 0;
        float v = vSum / 3;

        float direction = (v > mLastValues[k] ? 1 : (v < mLastValues[k] ? -1 : 0));
        if (direction == -mLastDirections[k]) {
            // Direction changed
            int extType = (direction > 0 ? 0 : 1); // minumum or maximum?
            mLastExtremes[extType][k] = mLastValues[k];
            float diff = abs(mLastExtremes[extType][k] - mLastExtremes[1 - extType][k]);

            if (diff > mLimit) {
                bool isAlmostAsLargeAsPrevious = diff > (mLastDiff[k]*2/3);
                bool isPreviousLargeEnough = mLastDiff[k] > (diff/3);
                bool isNotContra = (mLastMatch != 1 - extType);

                if (isAlmostAsLargeAsPrevious && isPreviousLargeEnough && isNotContra) {
                    emit onStep();
                    mLastMatch = extType;
                }
                else {
                    mLastMatch = -1;
                }
            }
            mLastDiff[k] = diff;
        }
        mLastDirections[k] = direction;
        mLastValues[k] = v;

        //qDebug() << t++ << rx << ry << rz;

//        qreal divider = sqrt(rx * rx + ry * ry + rz * rz);

//        float lowPassFactor = 0.1;

//        // Calculate the axis angles in degrees and reduce the noise in sensor
//        // readings.
//        x = (acos(rx / divider) * RADIANS_TO_DEGREES - 90 - x) * lowPassFactor;
//        y = (acos(ry / divider) * RADIANS_TO_DEGREES - 90 - y) * lowPassFactor;
//        z = (acos(rz / divider) * RADIANS_TO_DEGREES - 90 - z) * lowPassFactor;

//        //emit rotationChanged(x);

//        qDebug() << t++ << x << y << z;

//        qreal dx = x - rx;
//        qreal dy = y - ry;
//        qreal dz = z - rz;
//        if(abs(rx) + abs(ry) + abs(rz) > 12
//                )
//        //if(abs(x - rx) > 1 && abs(y - ry) > 1 && abs(z - rz) > 1)
//            emit onStep();

//        x = rx;
//        y = ry;
//        z = rz;

        // Don't store the reading in the sensor.
        return false;
    }

signals:
    void onStep();
    void rotationChanged(const QVariant &deg);
};

#endif // STEPSENSOR_H
