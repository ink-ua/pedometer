import QtQuick 1.0
import com.nokia.meego 1.0

Page {
    id: histPage
    //anchors.margins: 20
    orientationLock: PageOrientation.LockPortrait

    TabGroup {
          id: tabGroup
          currentTab: total
          anchors.top: buttonRow.bottom
          anchors.topMargin: 20
          height: parent.height - buttonRow.height
          Item {
              id:total
              anchors.centerIn: parent
              height: parent.height
              width: 430
              Column {
                  spacing: 20
                  anchors.topMargin: 30
                  width: parent.width
                  InfoBox {
                      id: totalSteps
                      title: "Total steps"
                      text: appcontroller.totalSteps
                      width: parent.width
                  }
                  InfoBox {
                      id: totalTime
                      title: "Total time"
                      text: appcontroller.formatTime(appcontroller.totalTime)
                      width: parent.width
                  }
                  InfoBox {
                      id: totalDistance
                      title: "Total distance"
                      text: appcontroller.formatDistance(appcontroller.totalSteps * appcontroller.stepLength)
                      width:parent.width
                  }
                  InfoBox {
                      id: totalCalories
                      title: "Total calories"
                      text: appcontroller.calTotal
                      width:parent.width
                  }
                  InfoBox {
                      id: avgSpeed
                      title: "Average speed"
                      text: appcontroller.formatDistance((appcontroller.totalSteps * appcontroller.stepLength * 3600) / appcontroller.totalTime) + "/h"
                      width:parent.width
                  }
              }
          }

          Item {
              id: daily
              anchors.bottom: parent.bottom
              anchors.horizontalCenter: parent.horizontalCenter
              height: parent.height
              width: 450
              ListView {
                  id: listView
                  anchors.fill: parent
                  model: historyModel
                  section.property: "month" //"intMonth"
                  section.criteria: ViewSection.FullString
                  section.delegate: sectionHeading
                  delegate: Item {
                      height: 70
                      anchors.left: parent.left
                      anchors.leftMargin: 5
                      anchors.right: parent.right
                      anchors.rightMargin: 5
                      //color: "lightyellow"
                      //border.width: 1
                      Row {
                          spacing: 0
                          anchors.verticalCenter: parent.verticalCenter
                          anchors.left: parent.left
                          Rectangle {
                              height: 70
                              width: 70
                              color: "lightyellow"
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
                              width: 360
                              color: "lightyellow"
                              border.width: 1
                              Row {
                                  spacing: 8
                                  anchors.centerIn: parent
                                  Label {
                                      text: steps
                                      font.pixelSize: 30
                                      font.bold: true
                                      style: LabelStyle {
                                          textColor: {
                                            var green = parseInt((steps * appcontroller.stepLength) / (appcontroller.daily / 0xFF));
                                            var red = 0;
                                            if(green > 0xFF)
                                                green = 0xFF;
                                            else
                                                red = 0xFF - green;
                                            return "#" + (red <= 0xF ? "0" : "") + red.toString(16) + (green <= 0xF ? "0" : "") + green.toString(16) + "00";
                                          }
                                      }
                                  }
                                  Label {
                                      text: "steps for"
                                      font.pixelSize: 28
                                      style: LabelStyle { textColor: "#555555" }
                                  }
                                  Label {
                                      text: time
                                      font.pixelSize: 30
                                      font.bold: true
                                  }
                              }
                          }
//                          Column {
//                              Label {
//                                  text: "Steps"
//                                  font.bold: true
//                              }
//                              Label {
//                                  text: steps
//                              }
//                          }
//                          Column {
//                              Label {
//                                  text: "Time"
//                                  font.bold: true
//                              }
//                              Label {
//                                  text: time
//                              }
//                          }
                      }
                  }
              }
              SectionScroller {
                  listView: listView
              }
          }
    }

    Component {
        id: sectionHeading
        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 10
            //anchors.bottomMargin: 5
            //width: parent.width
            border.width: 1
            radius: 5
            height: 30
            color: "lightsteelblue"
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
            tab: daily
        }
    }
}
