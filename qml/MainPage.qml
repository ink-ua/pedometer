import QtQuick 1.1
import com.meego 1.0

Page {
    id: mainPage
    tools: buttonTools

    //orientationLock: PageOrientation.LockPortrait

    Row {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
        spacing: 10
        InfoBox {
            id: time
            title:  "Time"
            text: appcontroller.time
        }
        InfoBox {
            id: distance
            title: "Distance"
            text: appcontroller.formatDistance(appcontroller.distance)
        }
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
        InfoBox {
            id: cal
            title: "Calories"
            text: appcontroller.cal
        }
    }
    InfoBox {
        id: step
        title: "Steps"
        text: appcontroller.steps
        fontPixelSize: 70
        anchors.centerIn: parent
        width:  512
        height: 160
    }

//    Button {
//        id: btnStop
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.top: step.bottom
//        anchors.topMargin: 30
//        text: appcontroller.running ? "Pause" : "Start"
//        onClicked: appcontroller.running = !appcontroller.running;
//        platformStyle: btnStopStyle
//    }

    Rectangle {
        id: today
        color: "#aaaaaa" //"#ddd8dd"
        border.width: 1
        radius: 10
        opacity: 0.8
        anchors.top: step.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        //anchors.left: parent.left
        //anchors.right: parent.right
        anchors.margins: 40
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
                }
                //anchors.horizontalCenter: parent.horizontalCenter
                //anchors.right: col.left
            }
            Column {
                id:col
                //anchors.centerIn: parent
                ProgressBar {
                    id: pb
                    width: 300
                    minimumValue: 0
                    maximumValue: appcontroller.daily
                    value: appcontroller.todayDistance
                    //anchors.horizontalCenter: parent.horizontalCenter
                }
                Label {
                    anchors.left: pb.left
                    anchors.leftMargin: {
                        var pos = (pb.width / appcontroller.daily) * appcontroller.todayDistance;
                        if(pos < pb.width - width) {
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
                    }
                }
            }
            Label {
                //anchors.left: col.right
                //anchors.horizontalCenter: parent.horizontalCenter
                text: appcontroller.formatDistance(appcontroller.daily)
                platformStyle: LabelStyle {
                    textColor: "white"
                }
            }
        }
    }

    Timer {
        id:fakeSteps
        interval: 100
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

    QueryDialog {
        id: queryStop
        titleText: "Stop"
        message: "Are you sure you want to stop pedometer?"
        acceptButtonText: "Yes"
        rejectButtonText: "No"
        onAccepted: appcontroller.reset();
    }

    ToolBarLayout {
            id: buttonTools
            ToolIcon {
                visible: false
                iconId: "toolbar-back";
                onClicked: { appMenu.close(); pageStack.pop(); }
            }
            ToolButtonRow {
                ToolButton {
                    id: btnStop
                    text: appcontroller.running ? "Pause" : "Start"
                    onClicked: appcontroller.running = !appcontroller.running;
                    platformStyle: ButtonStyle {
                        fontWeight: Font.Bold
                        buttonWidth: 200
                    }
                }
                ToolButton {
                    text: "Stop"
                    onClicked: {
                        if(appcontroller.running)
                            queryStop.open();
                        else
                            appcontroller.reset();
                    }
                    platformStyle: ButtonStyle {
                        fontWeight: Font.Bold
                        buttonWidth: 200
                    }
                }
            }
            ToolIcon {
                platformIconId: "toolbar-view-menu";
                anchors.right: parent === undefined ? undefined : parent.right
                onClicked: (appMenu.status == DialogStatus.Closed) ? appMenu.open() : appMenu.close()
            }
        }
}
