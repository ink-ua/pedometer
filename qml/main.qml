import QtQuick 1.1
import com.meego 1.0

PageStackWindow {
    id: appWindow
    //showToolBar: false

    platformStyle: PageStackWindowStyle {
        background: "qrc:/bg.png"
    }

    function openFile(file) {
        var component = Qt.createComponent(file)
        if (component.status == Component.Ready)
            pageStack.push(component);
        else
            console.log("Error loading component:", component.errorString());
    }

    initialPage: mainPage
    MainPage { id: mainPage }

    ToolBarLayout {
        id: commonTools
        visible: false
    }

    QueryDialog {
        id: queryQuit
        //icon: "image://theme/icon-l-contacts"
        titleText: "Quit"
        message: "Are you sure you want to stop pedometer and quit?"
        acceptButtonText: "Yes"
        rejectButtonText: "No"
        onAccepted: {
            appcontroller.save();
            Qt.quit();
        }
    }

    Menu {
        id: appMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: "History"
                onClicked: appWindow.openFile('History.qml')
            }
            MenuItem {
                text: "Preferences"
                onClicked: appWindow.openFile('Preferences.qml')
            }
            MenuItem {
                text: "Quit"
                onClicked: {
                    if(appcontroller.running)
                        queryQuit.open();
                    else {
                        appcontroller.save();
                        Qt.quit();
                    }
                }
            }
        }
    }
}
