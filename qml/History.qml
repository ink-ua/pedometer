import QtQuick 1.0
import com.meego 1.0

Page {
    id: histPage
    //tools: histTools
    anchors.margins: 20
    orientationLock: PageOrientation.LockPortrait

    ButtonRow {
        id: buttonRow
        anchors.top: parent.top
        TabButton {
            text: "Totals"
            tab: total
        }
        TabButton {
            text: "Daily"
            tab: daily
        }
    }

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
              height: parent.height
              width: 430
              ListView {
                  id: listView
                  anchors.fill: parent
                  model: historyModel
                  section.property: "intMonth"
                  section.criteria: ViewSection.FullString
                  section.delegate: sectionHeading
                  delegate: Rectangle {
                      height: 50
                      anchors.left: parent.left
                      anchors.leftMargin: 10
                      anchors.right: parent.right
                      anchors.rightMargin: 20
                      color: "lightyellow"
                      border.width: 1
                      Row {
                          spacing: 10
                          anchors.verticalCenter: parent.verticalCenter
                          anchors.left: parent.left
                          anchors.leftMargin: 10
                          Label {
                              text: day
                              font.bold: true
                              font.pixelSize: 40;
                          }
                          Column {
                              Label {
                                  text: "Steps"
                                  font.bold: true
                              }
                              Label {
                                  text: steps
                              }
                          }
                          Column {
                              Label {
                                  text: "Time"
                                  font.bold: true
                              }
                              Label {
                                  text: time
                              }
                          }
                      }
                  }
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
}
