import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: histPage
    orientationLock: PageOrientation.LockPortrait

    function calcRateColor(steps) {
        var green = parseInt((steps * appcontroller.stepLength) / (appcontroller.daily / 0xFF));
        var red = 0;
        if(green > 0xFF)
            green = 0xFF;
        else
            red = 0xFF - green;
        return "#" + (red <= 0xF ? "0" : "") + red.toString(16) + (green <= 0xF ? "0" : "") + green.toString(16) + "00";
    }

    TabGroup {
          id: tabGroup
          currentTab: total
          anchors.top: buttonRow.bottom
          anchors.topMargin: 20
          height: parent.height - buttonRow.height - anchors.topMargin
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
              width: 470
              ListView {
                  id: listView
                  anchors.fill: parent
                  model: historyModel
                  section.property: "month"
                  section.criteria: ViewSection.FullString
                  section.delegate: sectionHeading
                  delegate: Item {
                      property string rateColor: calcRateColor(steps)
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
                              color: appcontroller.inverted ? "#666650" : "lightyellow"
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
                              width: 380
                              color: (appcontroller.inverted ? "#222222" : "#dddddd")
                              border.width: 1
                              Row {
                                  spacing: 8
                                  anchors.centerIn: parent
                                  Label {
                                      text: steps
                                      font.pixelSize: 30
                                      font.bold: true
                                      style: LabelStyle {
                                        textColor: rateColor
                                      }
                                  }
                                  Label {
                                      text: "steps for"
                                      font.pixelSize: 28
                                      style: LabelStyle { textColor: appcontroller.inverted ? "lightgrey" : "#222222" }
                                  }
                                  Label {
                                      text: time
                                      font.pixelSize: 30
                                      font.bold: true
                                  }
                              }
                          }
                      }
                      MouseArea {
                        anchors.fill: parent
                        onClicked: detail.open();
                      }
                      Dialog {
                        id: detail
                        anchors.fill: parent
                        anchors.margins: 10
                        property int fontSize: 40
                        property string lColor: "lightyellow" // appcontroller.inverted ? "white" : "lightyellow"
                        title: Label {
                            text: day + " " + month
                            font.pixelSize: 60
                            font.bold: true
                            anchors.horizontalCenter: parent.horizontalCenter
                            style: LabelStyle {
                                textColor: "yellow" //appcontroller.inverted ? "lightyellow" : "yellow"
                            }
                        }
                        content: Grid {
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
                                text: steps
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
                                text: time
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
                                text: appcontroller.formatDistance(steps * appcontroller.stepLength)
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
                                text: appcontroller.formatDistance((steps * appcontroller.stepLength * 3600) / seconds) + "/h"
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
                                text: steps * appcontroller.calPerStep
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
                                text: appcontroller.formatPercent(steps * appcontroller.stepLength / (appcontroller.daily / 100.0))
                                font.pixelSize: detail.fontSize
                                font.bold: true
                                style: LabelStyle {
                                    textColor: rateColor
                                }
//                                LabelStyle {
//                                    textColor: detail.lColor
//                                }
//                                style: LabelStyle {
//                                    textColor: {
//                                      var green = parseInt(rate.text / (100.0 / 0xFF));
//                                      var red = 0;
//                                      if(green > 0xFF)
//                                          green = 0xFF;
//                                      else
//                                          red = 0xFF - green;
//                                      return "#" + (red <= 0xF ? "0" : "") + red.toString(16) + (green <= 0xF ? "0" : "") + green.toString(16) + "00";
//                                    }
//                                }
                            }
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
            tab: daily
        }
    }
}
