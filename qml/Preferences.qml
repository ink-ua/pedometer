import QtQuick 1.0
import com.meego 1.0

Page {
    id: prefPage
    //tools: mainToolBar
    orientationLock: PageOrientation.LockPortrait

    Flickable {
        id: flickable
        anchors.centerIn: parent
        flickableDirection: Flickable.HorizontalFlick
        anchors.topMargin: 10
        anchors.leftMargin: 10

        Column {
            spacing: 10
            anchors.centerIn: parent
            Row {
                spacing: 10
                Label {
                    id:lblStep
                    text: "Step length (m) "
//                    platformStyle: LabelStyle {
//                        textColor: "white"
//                    }
                }
                TextField {
                    id: stepLength
                    onTextChanged: appcontroller.stepLength = stepLength.text
                    validator: DoubleValidator {
                        bottom: 0
                        top: 2
                    }
                    text: appcontroller.stepLength
                    anchors.verticalCenter: lblStep.verticalCenter
                }
            }
            Row {
                spacing: 10
                Label {
                    id:lblDaily
                    text: "Daily distance (km) "
                }
                TextField {
                    id: daily
                    text: appcontroller.daily / 1000.0
                    validator: DoubleValidator {
                        bottom: 0
                        top: 1000
                    }
                    onTextChanged: appcontroller.daily = daily.text * 1000.0
                    anchors.verticalCenter: lblDaily.verticalCenter
                }
            }
            Row {
                spacing: 10
                Label {
                    id:lblStop
                    text: "Pause when stop walking"
                }
                Switch {
                    id: stop
                    checked: appcontroller.pauseWalking
                    anchors.verticalCenter: lblStop.verticalCenter
                }
            }
            Row {
                spacing: 10
                Label {
                    id:lblSense
                    text: "Sensetivity "
                }
                Slider {
                    id: sense
                    stepSize: 0.1
                    valueIndicatorVisible: true
                    value: 0.75
                    minimumValue: 0
                    maximumValue: 1
                    width: 200
                    anchors.verticalCenter: lblSense.verticalCenter
                }
            }
        }
    }

//    ToolBarLayout {
//            id: prefTools
//            ToolIcon {
//                iconId: "toolbar-back";
//                onClicked: { appMenu.close(); pageStack.pop(); }
//            }
//            ToolButtonRow {
//                ToolButton {
//                    text:"Save"
//                    onClicked: {
//                        appcontroller.stepLength = stepLength.text;
//                        appcontroller.daily = daily.text * 1000.0;
//                    }
//                }
//            }
//        }
}
