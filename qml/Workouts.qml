import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    Text {
        id: name
        text: qsTr("text")
        color: "red"
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
