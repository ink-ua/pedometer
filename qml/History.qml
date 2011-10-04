import QtQuick 1.0
import com.meego 1.0

Page {
    id: histPage
    tools: histTools
    anchors.margins: 20

    TabGroup {
          id: tabGroup
          currentTab: total

          Item {
              id:total
              anchors.centerIn: parent
              height: parent.height
              width: 320
              InfoBox {
                  id: totalSteps
                  title: "Total steps"
                  text: appcontroller.totalSteps
                  width:parent.width
                  anchors.left: parent.left
                  anchors.top: parent.top
                  anchors.leftMargin: 10
                  anchors.topMargin: 10
              }
              InfoBox {
                  id: totalTime
                  title: "Total time"
                  text: appcontroller.totalTime
                  width:parent.width
                  anchors.left: parent.left
                  anchors.top: totalSteps.bottom
                  anchors.leftMargin: 10
                  anchors.topMargin: 10
              }
              InfoBox {
                  id: totalDistance
                  title: "Total distance"
                  text: appcontroller.formatDistance(appcontroller.totalSteps * appcontroller.stepLength)
                  width:parent.width
                  anchors.left: parent.left
                  anchors.top: totalTime.bottom
                  anchors.leftMargin: 10
                  anchors.topMargin: 10
              }
          }

          Item {
              id: daily
              anchors.centerIn: parent
              height: parent.height
              width: 320
              ListView {
                  id: listView
                  anchors.fill: parent
                  model: historyModel
                  section.property: "date"
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
                      //width: parent.width - 20
                      //radius: 5
                      Column {
                          anchors.verticalCenter: parent.verticalCenter
                          anchors.left: parent.left
                          anchors.leftMargin: 10
                          //anchors.margins: 10
                          //spacing: 3
                          Row {
                              spacing: 10
                              Label {
                                  text: "Steps:"
                                  font.bold: true
                              }
                              Label {
                                  text: steps
                              }
                          }
                          Row {
                              spacing: 10
                              Label {
                                  text: "Time:"
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

    ToolBarLayout {
        id: histTools
        ToolIcon {
            iconId: "toolbar-back";
            onClicked: { appMenu.close(); pageStack.pop(); }
        }
        ButtonRow {
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
}
