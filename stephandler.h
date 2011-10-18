#ifndef STEPSENSOR_H
#define STEPSENSOR_H

#define MAGNETIC_FIELD_EARTH_MAX 60.0f
#define MAGNETIC_FIELD_EARTH_MIN 30.0f
#define GRAVITY_EARTH 9.806650161743164f

#include <QAccelerometerFilter>
#include <QDebug>
#include <QDateTime>
#include <math.h>

QTM_USE_NAMESPACE

class StepHandler : public QObject, public QAccelerometerFilter {
    Q_OBJECT

private:
    float mYOffset;
    float mScale;
    float mLastValues[3*2];
    float mLastDirections[3*2];
    float mLowerLimit, mUpperLimit;
    float mLastDiff[3*2];
    int mLastMatch;
    float mLastExtremes[3*2][3*2];

public:
    StepHandler() {
        int h = 480; // TODO: remove this constant
        mYOffset = h * 0.5f;
        mScale = -(h * 0.5f * (1.2f / MAGNETIC_FIELD_EARTH_MAX));
        mLowerLimit = 8;
        mUpperLimit = 50;
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

            if (diff > mLowerLimit && diff < mUpperLimit) {
                //qDebug() << diff;

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

        return false;
    }

signals:
    void onStep();
};

#endif // STEPSENSOR_H
