import QtQuick 1.1
import com.nokia.meego 1.1

Page {
    id: histPage
    orientationLock: PageOrientation.LockPortrait

    onStatusChanged: {
        if (status == PageStatus.Activating) {
            indicator.running = true;
        } else if(status == PageStatus.Active) {
            // don't reload history, just add missing entries
            // load on Component.onCompleted ?
            // what about BusyIndicator ?

            historyProvider.reloadHistory();

//            if(historyProvider.reloadHistory()) {
//                historyModel.clear();
//                while(historyProvider.hasNextEntry()) {
//                    historyModel.append(historyProvider.getNextEntry());
//                }
//            } else {
//                historyModel.set(0, historyProvider.getTodayEntry());
//            }



            indicator.running = false;
        }
//        else if(status == PageStatus.Deactivating) {
//            historyModel.clear();
//        }
    }

//    Connections {
//        target: historyProvider
//        entryAddedToList: {
//            console.log(h.steps);
//            historyModel.insert(0, h);
//        }
//    }

    function calcRateColor(distance) {
        var green = parseInt(distance / (appcontroller.daily / 0xFF));
        var red = 0;
        if(green > 0xFF)
            green = 0xFF;
        else
            red = 0xFF - green;
        var result = "#"
                + (red <= 0xF ? "0" : "") + red.toString(16) // red component
                + (green <= 0xF ? "0" : "") + green.toString(16) // green component
                + "00"; // no blue
        return result;
    }

//    ListModel {
//        id: historyModel
//    }

    Gradient {
        id: dateDark
        GradientStop {
            position: 0.00
            color: "#aaaaaa"
        }
        GradientStop {
            position: 0.50
            color: "#555555"
        }
        GradientStop {
            position: 1.00
            color: "#aaaaaa"
        }
    }
    Gradient {
        id: dateLight
        GradientStop {
            position: 0.00;
            color: "#ffffe0";
        }
        GradientStop {
            position: 0.50;
            color: "#dfdfc0";
        }
        GradientStop {
            position: 1.00;
            color: "#ffffe0";
        }
    }

    Dialog {
        id: detail
        anchors.fill: parent
        anchors.margins: 10
        property int fontSize: 40
        property string lColor: "lightyellow"

        property int day
        property string month
        property string rateColor
        property int steps
        property string time
        property int seconds
        property double distance
        property double calories

        title: Label {
            text: detail.day + " " + detail.month
            font.pixelSize: 60
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
            style: LabelStyle {
                textColor: "yellow"
            }
        }
        content: Column {
            spacing: 25
            anchors.topMargin: 25
            anchors.horizontalCenter: parent.horizontalCenter
            Grid {
                spacing: 25
                columns: 2
                anchors.horizontalCenter: parent.horizontalCenter
                Label {
                    text: "Steps"
                    font.pixelSize: detail.fontSize
                    style: LabelStyle {
                        textColor: detail.lColor
                    }
                }
                Label {
                    text: detail.steps
                    font.pixelSize: detail.fontSize
                    font.bold: true
                    style: LabelStyle {
                        textColor: detail.lColor
                    }
                }
                Label {
                    text: "Time"
                    font.pixelSize: detail.fontSize
                    style: LabelStyle {
                        textColor: detail.lColor
                    }
                }
                Label {
                    text: detail.time
                    font.pixelSize: detail.fontSize
                    font.bold: true
                    style: LabelStyle {
                        textColor: detail.lColor
                    }
                }
                Label {
                    text: "Distance"
                    font.pixelSize: detail.fontSize
                    style: LabelStyle {
                        textColor: detail.lColor
                    }
                }
                Label {
                    text: formatter.formatDistance(detail.distance)
                    font.pixelSize: detail.fontSize
                    font.bold: true
                    style: LabelStyle {
                        textColor: detail.lColor
                    }
                }
                Label {
                    text: "Avg speed"
                    font.pixelSize: detail.fontSize
                    style: LabelStyle {
                        textColor: detail.lColor
                    }
                }
                Label {
                    text: formatter.formatSpeed(appcontroller.calculateSpeed(detail.distance, detail.seconds))
                    font.pixelSize: detail.fontSize
                    font.bold: true
                    style: LabelStyle {
                        textColor: detail.lColor
                    }
                }
                Label {
                    text: "Calories"
                    font.pixelSize: detail.fontSize
                    style: LabelStyle {
                        textColor: detail.lColor
                    }
                }
                Label {
                    text: formatter.formatCalories(detail.calories)
                    font.pixelSize: detail.fontSize
                    font.bold: true
                    style: LabelStyle {
                        textColor: detail.lColor
                    }
                }
                Label {
                    text: "Rate"
                    font.pixelSize: detail.fontSize
                    style: LabelStyle {
                        textColor: detail.lColor
                    }
                }
                Label {
                    id: rate
                    text: formatter.formatPercent(appcontroller.calculateRate(detail.distance))
                    font.pixelSize: detail.fontSize
                    font.bold: true
                    style: LabelStyle {
                        textColor: detail.rateColor
                    }
                }
            }
//            Button {
//                text: "Workouts"
//                anchors.horizontalCenter: parent.horizontalCenter
//                onClicked: {
//                    var component = Qt.createComponent("qrc:/qml/Workouts.qml");
//                    if (component.status == Component.Ready)
//                        appWindow.pageStack.push(component);
//                    else
//                        console.log("Error loading component:", component.errorString());
//                    detail.close();
//                }
//            }
        }
    }

    TabGroup {
        id: tabGroup
        currentTab: total
        anchors.top: buttonRow.bottom
        height: parent.height - buttonRow.height
        Item {
            id:total
            anchors.centerIn: parent
            height: parent.height
            width: 430
            Column {
                spacing: 10
                anchors.centerIn: parent
                width: parent.width
                InfoBox {
                    id: totalSteps
                    title: "Total steps"
                    text: historyProvider.totalSteps
                    width: parent.width
                }
                InfoBox {
                    id: totalTime
                    title: "Total time tracked"
                    text: formatter.formatTime(historyProvider.totalTime)
                    width: parent.width
                }
                InfoBox {
                    id: totalDistance
                    title: "Total distance"
                    text: formatter.formatDistance(historyProvider.totalDistance)
                    width:parent.width
                }
                InfoBox {
                    id: totalCalories
                    title: "Total calories"
                    text: formatter.formatCalories(historyProvider.totalCalories)
                    width:parent.width
                }
                InfoBox {
                    id: avgSpeed
                    title: "Average speed"
                    text: formatter.formatSpeed(appcontroller.calculateSpeed(historyProvider.totalDistance, historyProvider.totalTime))
                    width:parent.width
                }
            }
        }

        Item {
            id: avg
            anchors.centerIn: parent
            height: parent.height
            width: 430
            Column {
                spacing: 10
                anchors.centerIn: parent
                width: parent.width
                InfoBox {
                    id: avgSteps
                    title: "Steps per day"
                    text: historyProvider.avgSteps
                    width: parent.width
                }
                InfoBox {
                    id: avgTime
                    title: "Time tracked per day"
                    text: formatter.formatTime(historyProvider.avgTime)
                    width: parent.width
                }
                InfoBox {
                    id: avgDistance
                    title: "Distance per day"
                    text: formatter.formatDistance(historyProvider.avgDistance)
                    width:parent.width
                }
                InfoBox {
                    id: avgCalories
                    title: "Calories per day"
                    text: formatter.formatCalories(historyProvider.avgCalories)
                    width:parent.width
                }
                InfoBox {
                    id: avgRate
                    title: "Average daily rate"
                    text: formatter.formatPercent(historyProvider.avgRate)
                    width:parent.width
                }
            }
        }

        Item {
            id: daily
            anchors.bottom: parent.bottom
            //anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height
            width: 480
            ListView {
                id: listView
                anchors.fill: parent
                model: historyModel

                section.property: "month"
                section.criteria: ViewSection.FullString
                section.delegate: sectionHeading

                delegate: Item {
                    id: wrapper
                    property string rateColor: calcRateColor(distance)
                    height: 70
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    Row {
                        spacing: 0
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        Rectangle {
                            height: 70
                            width: 70
                            //color: appcontroller.inverted ? "#666650" : "lightyellow"
                            gradient: appcontroller.inverted ? dateDark : dateLight
                            border.width: 1
                            Label {
                                id: dayLabel
                                anchors.centerIn: parent
                                text: day
                                font.bold: true
                                font.pixelSize: 50
                            }
                        }
                        Rectangle {
                            height: 70
                            width: 390
                            color: (appcontroller.inverted ? "#222222" : "#dddddd")
                            border.width: 1
                            Row {
                                spacing: 8
                                anchors.centerIn: parent
                                Label {
                                    text: steps
                                    font.pixelSize: 35
                                    font.bold: true
                                    style: LabelStyle {
                                        textColor: rateColor
                                    }
                                }
                                Label {
                                    text: "steps for"
                                    font.pixelSize: 30
                                    style: LabelStyle { textColor: appcontroller.inverted ? "lightgrey" : "#222222" }
                                }
                                Label {
                                    text: time
                                    font.pixelSize: 35
                                    font.bold: true
                                }
                            }
                        }
                    }
                    states: State {
                        name: "Current"
                        PropertyChanges { target: wrapper; opacity: 0.5 }
                    }
                    transitions: Transition {
                        NumberAnimation { properties: "opacity"; duration: 100 }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onEntered: wrapper.state = "Current";
                        onCanceled: wrapper.state = "";
                        onExited: wrapper.state = "";
                        onReleased: wrapper.state = "";
                        onClicked: {
                            detail.day = day;
                            detail.month = month;
                            detail.rateColor = rateColor;
                            detail.steps = steps;
                            detail.time = time;
                            detail.distance = distance;
                            detail.calories = calories;
                            detail.seconds = seconds;
                            detail.open();
                        }
                    }
                }
            }
            SectionScroller {
                listView: listView
            }
            ScrollDecorator {
                flickableItem: listView
            }
        }
    }

    Component {
        id: sectionHeading
        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 10
            border.width: 1
            radius: 5
            height: 30
            color: (appcontroller.inverted ? "#225511" : "#77bb66")
            Label {
                anchors.centerIn: parent
                text: section
                font.bold: true
            }
        }
    }

    ButtonRow {
        id: buttonRow
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        TabButton {
            text: "Totals"
            tab: total
        }
        TabButton {
            text: "Daily"
            tab: avg
        }
        TabButton {
            text: "History"
            tab: daily
        }
    }

    BusyIndicator {
        id: indicator
        platformStyle: BusyIndicatorStyle { size: "large" }
        running: false
        visible: running
        anchors.centerIn: parent
    }
}
