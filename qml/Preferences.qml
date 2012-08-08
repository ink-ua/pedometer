import QtQuick 1.1
import com.nokia.meego 1.1

Page {
    id: prefPage
    orientationLock: PageOrientation.LockPortrait

//    Label {
//        id: title
//        anchors.top: parent.top
//        anchors.topMargin: 10
//        width: parent.width
//        text: "Preferenses<hr>"
//        style: LabelStyle {
//            fontPixelSize: 40
//        }
//    }

    LabelStyle {
        id: prefLabel
        fontPixelSize: 30
    }

    Column {
        spacing: 30;
        anchors.top: parent.top;
        anchors.topMargin: 5;
        anchors.horizontalCenter: parent.horizontalCenter;

        Grid {
            columns: 2
            spacing: 30
            Label {
                id:lblStep
                text: "Stride length (m)"
                platformStyle: prefLabel
            }
            TextField {
                id: stepLength
                width: 130
                onTextChanged: appcontroller.stepLength = stepLength.text
                inputMethodHints: Qt.ImhDigitsOnly
                validator: DoubleValidator {
                    bottom: 0
                    top: 2
                }
                text: appcontroller.stepLength
                anchors.verticalCenter: lblStep.verticalCenter
            }
        }
        Grid {
            columns: 2
            spacing: 30
            Label {
                id:lblDaily
                text: "Daily distance (km)"
                platformStyle: prefLabel
            }
            TextField {
                id: daily
                width: 130
                text: appcontroller.daily / 1000.0
                inputMethodHints: Qt.ImhDigitsOnly
                validator: DoubleValidator {
                    bottom: 0
                    top: 1000
                }
                onTextChanged: appcontroller.daily = daily.text * 1000.0
                anchors.verticalCenter: lblDaily.verticalCenter
            }
        }
        Grid {
            columns: 2
            spacing: 30
            Label {
                id:lblWeight
                text: "Weight (kg)"
                platformStyle: prefLabel
            }
            TextField {
                id: weight
                width: 130
                onTextChanged: appcontroller.weight = weight.text
                inputMethodHints: Qt.ImhDigitsOnly
                validator: DoubleValidator {
                    bottom: 0
                    top: 500
                }
                text: appcontroller.weight
                anchors.verticalCenter: lblWeight.verticalCenter
            }
        }
        Grid {
            columns: 2
            spacing: 30
            Label {
                id: lblFreeze
                text: "Freeze when not walking"
                platformStyle: prefLabel
            }
            Switch {
                checked: appcontroller.freeze
                onCheckedChanged: {
                    appcontroller.freeze = checked;
                }
                anchors.verticalCenter: lblFreeze.verticalCenter
            }
        }
        Grid {
            columns: 2
            spacing: 30
            Label {
                id: lblPair
                text: "Count pairs of steps"
                platformStyle: prefLabel
            }
            Switch {
                checked: appcontroller.countPairs
                onCheckedChanged: {
                    appcontroller.countPairs = checked;
                }
                anchors.verticalCenter: lblPair.verticalCenter
            }
        }
        Column {
            spacing: 10
            Label {
                id:lblSens
                text: "Sensetivity"
                platformStyle: prefLabel
            }
            Slider {
                id: sens
                stepSize: 0.05
                valueIndicatorVisible: true
                minimumValue: 0.0
                maximumValue: 1.0
                value: appcontroller.sensitivity
                width: 450
                onValueChanged: appcontroller.sensitivity = value
                //anchors.verticalCenter: lblSense.verticalCenter
            }
        }
        Grid {
            columns: 2
            spacing: 30
            Label {
                id: lblInv
                text: "Dark theme"
                platformStyle: prefLabel
            }
            Switch {
                checked: appcontroller.inverted
                onCheckedChanged: {
                    appcontroller.inverted = checked;
                    theme.inverted = checked;
                }
                anchors.verticalCenter: lblInv.verticalCenter
            }
        }
        Column {
            spacing: 10
            Label {
                text: "Units"
                platformStyle: prefLabel
            }
            ButtonRow {
                Button {
                    id: metric
                    text: "Metric"
                }
                Button {
                    id: imperial
                    text: "Imperial"
                }
                checkedButton: {
                    if(appcontroller.units === 0)
                        return metric;
                    else
                        return imperial;
                }
                onCheckedButtonChanged: {
                    if(checkedButton === metric)
                        appcontroller.units = 0;
                    else
                        appcontroller.units = 1;
                }
            }
        }
    }
}
