#ifndef ORIENTATIONFILTER_H
#define ORIENTATIONFILTER_H

//#include <QRotationFilter>

//QTM_USE_NAMESPACE

//class RotationFilter : public QObject, public QRotationFilter {
//    Q_OBJECT
//private:
//    qreal values[3];
//    qreal matrix[3][3];

//public:
//    RotationFilter() {
//        for(short i = 0; i < 3; i++)
//            values[i] = 0;
//    }

//    bool filter(QRotationReading *reading) {
//        qreal values[3] = { reading->x(), reading->y(), reading->z() };
//        // matrix
//        return false;
//    }

////    qreal* getValues() const {
////        return values;
////    }

//    // order of angles is z, x, y
////    qreal* getMatrix() const {
////        return matrix;
////    }
//};
#endif // ORIENTATIONFILTER_H
