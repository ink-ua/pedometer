import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: mainPage
    orientationLock: PageOrientation.LockPortrait

    Column {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
        spacing: 5
        Row {
            spacing: 5
            InfoBox {
                id: distance
                title: "Distance"
                text: formatter.formatDistance(appcontroller.distance)
            }
            InfoBox {
                id: cal
                title: "Calories"
                text: formatter.formatCalories(appcontroller.cal)
            }
        }
        Row {
            spacing: 5
            InfoBox {
                id: speed
                title: "Current speed"
                text: formatter.formatSpeed(appcontroller.speed)
            }
            InfoBox {
                id: avgSpeed
                title: "Average speed"
                text: formatter.formatSpeed(appcontroller.avgSpeed)
            }
        }
    }

    Column {
        id: colMain
        spacing: 10
        anchors.centerIn: parent
        InfoBox {
            id: time
            title:  "Time"
            fontPixelSize: 50
            width:  460
            height: width * 0.2
            text: formatter.formatTime(appcontroller.seconds)
        }
        InfoBox {
            id: step
            title: "Steps"
            text: appcontroller.steps
            fontPixelSize: 70
            width:  460
            height: width * 0.3
        }
    }

    Rectangle {
        id: today
        color: "#888888"
        border.width: 1
        radius: 10
        //activeFocus: opacity: 0.8
        anchors.top: colMain.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 40
        width: step.width
        height: 60

        Label {
            text: "Today"
            id: todayLbl
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            platformStyle: LabelStyle {
                textColor: "white"
                fontPixelSize: 30
            }
        }
        Column {
            id:col
            anchors.left: todayLbl.right
            anchors.leftMargin: 5
            anchors.right: targetLbl.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            ProgressBar {
                id: pb
                width: parent.width
                minimumValue: 0
                maximumValue: appcontroller.daily
                value: appcontroller.todayDistance
            }
            Label {
                anchors.left: pb.left
                anchors.leftMargin: {
                    var pos = (pb.width / appcontroller.daily) * appcontroller.todayDistance;
                    if(pos < pb.width - width / 2) {
                        pos -= width / 2;
                        if(pos < 0)
                            pos = 0;
                    }
                    else
                        pos = pb.width - width;
                    return pos;
                }
                text: formatter.formatDistance(appcontroller.todayDistance)
                platformStyle: LabelStyle {
                    textColor: "yellow"
                    fontPixelSize: 30
                }
            }
        }
        Label {
            id: targetLbl
            text: formatter.formatDistance(appcontroller.daily)
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            platformStyle: LabelStyle {
                textColor: "white"
                fontPixelSize: 30
            }
        }
    }

    ButtonRow {
        anchors.top: today.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 30
        exclusive: false
        Button {
            iconSource: "image://theme/icon-m-" + (appcontroller.running ? "toolbar-mediacontrol-pause" : "toolbar-mediacontrol-play") + (appcontroller.inverted ? "-white" : "")
            onClicked: appcontroller.running = !appcontroller.running
            platformStyle: ButtonStyle {
                buttonHeight: 80
            }
        }
        Button {
            iconSource: "image://theme/icon-m-toolbar-mediacontrol-stop" + (appcontroller.inverted ? "-white" : "")
            onClicked: appcontroller.reset()
            platformStyle: ButtonStyle {
                buttonHeight: 80
            }
        }
    }

    Timer {
        id: timer
        interval: 1000
        running: appcontroller.running; repeat: true
        onTriggered: {
            appcontroller.seconds++;
        }
    }
}
