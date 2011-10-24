import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: prefPage
    orientationLock: PageOrientation.LockPortrait

//    Label {
//        id: title
//        anchors.top: parent.top
//        text: "<hr>Preferenses"
//    }

    LabelStyle {
        id: prefLabel
        fontPixelSize: 30
    }

    Column {
        spacing: 30
        anchors.margins: 10
        anchors.top: parent.top
        anchors.left: parent.left
        Column {
            spacing: 10
            Label {
                id:lblStep
                text: "Step length (m)"
                platformStyle: prefLabel
            }
            TextField {
                id: stepLength
                onTextChanged: appcontroller.stepLength = stepLength.text
                inputMethodHints: Qt.ImhDigitsOnly
                validator: DoubleValidator {
                    bottom: 0
                    top: 2
                }
                text: appcontroller.stepLength
                //anchors.verticalCenter: lblStep.verticalCenter
            }
        }
        Column {
            spacing: 10
            Label {
                id:lblDaily
                text: "Daily distance (km)"
                platformStyle: prefLabel
            }
            TextField {
                id: daily
                text: appcontroller.daily / 1000.0
                inputMethodHints: Qt.ImhDigitsOnly
                validator: DoubleValidator {
                    bottom: 0
                    top: 1000
                }
                onTextChanged: appcontroller.daily = daily.text * 1000.0
                //anchors.verticalCenter: lblDaily.verticalCenter
            }
        }
        //            Row {
        //                spacing: 10
        //                Label {
        //                    id:lblStop
        //                    text: "Pause when stop walking"
        //                }
        //                Switch {
        //                    id: stop
        //                    checked: appcontroller.pauseWalking
        //                    anchors.verticalCenter: lblStop.verticalCenter
        //                }
        //            }
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
                width: 300
                onValueChanged: appcontroller.sensitivity = value
                //anchors.verticalCenter: lblSense.verticalCenter
            }
        }
        Row {
            spacing: 10
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
    }
}
