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
            //History { id: histPage }
            Preferences { id: prefPage }
        }

        tools: ToolBarLayout {
            id: tools
            anchors.bottom: parent.bottom
            ButtonRow {
                TabButton {
                    iconSource: "qrc:/human" + (appcontroller.inverted ? "-white.png" : ".png")
                    tab: mainPage
                }
                TabButton {
                    id: histButton
                    iconSource: "image://theme/icon-m-toolbar-history" + (appcontroller.inverted ? "-white" : "")
                    tab: histPage
                    onClicked: {
                        if(histPage)
                            histPage.destroy();
                        var component = Qt.createComponent("History.qml");
                        if (component.status == Component.Ready) {
                            var histPage = component.createObject(tabgroup, { id: histPage });
                            tabgroup.currentTab = histPage;
                            //histButton.tab = histPage;
                        }
                    }
                }
                TabButton {                    
                    iconSource: "image://theme/icon-m-toolbar-settings" + (appcontroller.inverted ? "-white" : "")
                    tab: prefPage
                }
            }
        }
    }
//    {
//    id: goalReachedNotification
//    }
    Component.onCompleted: {
        theme.inverted = appcontroller.inverted
    }
}
