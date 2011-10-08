import QtQuick 1.1
import com.meego 1.0

Page {
    id: mainPage
    tools: buttonTools
    orientationLock: PageOrientation.LockPortrait

    Column {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
        spacing: 10
        Row {
            spacing: 5
            InfoBox {
                id: distance
                title: "Distance"
                text: appcontroller.formatDistance(appcontroller.distance)
            }
            InfoBox {
                id: cal
                title: "Calories"
                text: appcontroller.cal
            }
        }
        Row {
            spacing: 5
            InfoBox {
                id: speed
                title: "Speed"
                text: appcontroller.speed
            }
            InfoBox {
                id: avgSpeed
                title: "Average speed"
                text: appcontroller.avgSpeed
            }
        }
    }

    Column {
        id: colMain
        spacing: 20
        anchors.centerIn: parent
        InfoBox {
            id: time
            title:  "Time"
            fontPixelSize: 50
            width:  460
            height: width * 0.2
            text: appcontroller.time
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
        color: "#888888" //"#ddd8dd"
        border.width: 1
        radius: 10
        opacity: 0.8
        anchors.top: colMain.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 100
        width: step.width
        height: 50
        Row {
            anchors.margins: 10
            anchors.centerIn: parent
            spacing: 10
            Label {
                text: "Today"
                platformStyle: LabelStyle {
                    textColor: "white"
                    fontPixelSize: 30
                }
            }
            Column {
                id:col
                ProgressBar {
                    id: pb
                    width: 230
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
                    text: appcontroller.formatDistance(appcontroller.todayDistance)
                    platformStyle: LabelStyle {
                        textColor: "yellow"
                        fontPixelSize: 30
                    }
                }
            }
            Label {
                text: appcontroller.formatDistance(appcontroller.daily)
                platformStyle: LabelStyle {
                    textColor: "white"
                    fontPixelSize: 30
                }
            }
        }
    }

    Timer {
        id:fakeSteps
        interval: 500
        running: appcontroller.running; repeat: true
        onTriggered: {
            appcontroller.steps++;
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

    ToolBarLayout {
        id: buttonTools
        //            ToolIcon {
        //                visible: false
        //                iconId: "toolbar-back";
        //                onClicked: { appMenu.close(); pageStack.pop(); }
        //            }
        ToolIcon {
            id: btnStop
            platformIconId: appcontroller.running ? "toolbar-mediacontrol-pause" : "toolbar-mediacontrol-play";
            //anchors.right: parent === undefined ? undefined : parent.right
            onClicked: appcontroller.running = !appcontroller.running;
        }
        ToolIcon {
            platformIconId: "toolbar-mediacontrol-stop"
            onClicked: appcontroller.reset()
        }
        ToolIcon {
            platformIconId: "toolbar-view-menu";
            anchors.right: parent === undefined ? undefined : parent.right
            onClicked: (appMenu.status == DialogStatus.Closed) ? appMenu.open() : appMenu.close()
        }
    }
}
