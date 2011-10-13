import QtQuick 1.1
import com.meego 1.0

PageStackWindow {
    id: appWindow
//    platformStyle: PageStackWindowStyle {
//        inverted: true
//    }

    initialPage: Page { orientationLock: PageOrientation.LockPortrait }

    TabGroup {
          id: tabgroup
          currentTab: mainPage
          MainPage { id: mainPage }
          History { id: histPage }
          Preferences { id: prefPage }
    }

    ToolBar {
        id: mainToolBar
        anchors.bottom: parent.bottom
        tools: ToolBarLayout {
            id: tools
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
}
