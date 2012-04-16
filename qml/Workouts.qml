import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    ListView {
        id: listView
        anchors.fill: parent
        model: historyModel

        delegate: Item {
            id: wrapper
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
                    width: 460
                    color: (appcontroller.inverted ? "#222222" : "#dddddd")
                    border.width: 1
                    Row {
                        spacing: 8
                        anchors.centerIn: parent
                        Label {
                            text: steps
                            font.pixelSize: 35
                            font.bold: true
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
        }
    }
    tools: ToolBarLayout {
        id: toolBarLayout1
        ToolIcon {
            anchors.left: parent.left
            iconId: "toolbar-back"
            onClicked: appWindow.pageStack.pop()
        }
    }
}
