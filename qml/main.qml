import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow
    initialPage: Page {
        orientationLock: PageOrientation.LockPortrait
        TabGroup {
            id: tabgroup
            currentTab: mainPage
            MainPage { id: mainPage }
            History { id: histPage }
            Preferences { id: prefPage }
        }

        tools: ToolBarLayout {
            id: tools
            anchors.bottom: parent.bottom
            ButtonRow {
                TabButton {
                    iconSource: "qrc:/human.png"
                    tab: mainPage
                }
                TabButton {
                    iconSource: "image://theme/icon-m-toolbar-history"
                    tab: histPage
                }
                TabButton {
                    iconSource: "image://theme/icon-m-toolbar-settings"
                    tab: prefPage
                }
            }
        }
    }
//    ToolBarLayout {
//        visible: false
//    }
//    ToolBar {
//        id: mainToolBar
//        anchors.bottom: parent.bottom
//        tools:
//    }
}
