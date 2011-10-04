import QtQuick 1.0
import com.meego 1.0

Page {
    id: prefPage
    tools: prefTools

    Flickable {
        id: flickable
        anchors.centerIn: parent
        flickableDirection: Flickable.HorizontalFlick
        anchors.topMargin: 10
        anchors.leftMargin: 10

        Column {
            spacing: 10
            anchors.centerIn: parent
            Row {
                spacing: 10
                Label {
                    id:lblStep
                    text: "Step length (m): "
                    platformStyle: LabelStyle {
                        textColor: "white"
                    }
                }
                TextField {
                    id: stepLength
                    text: appcontroller.stepLength
                    anchors.verticalCenter: lblStep.verticalCenter
                }
            }
            Row {
                spacing: 10
                Label {
                    id:lblDaily
                    text: "Daily distance (km): "
                    platformStyle: LabelStyle {
                        textColor: "white"
                    }
                }
                TextField {
                    id: daily
                    text: appcontroller.daily / 1000.0
                    anchors.verticalCenter: lblDaily.verticalCenter
                }
            }
            CheckBox{
                text: "Pause when stop walking"
                checked: appcontroller.pauseWalking
            }
        }
    }

    ToolBarLayout {
            id: prefTools
            ToolIcon {
                iconId: "toolbar-back";
                onClicked: { appMenu.close(); pageStack.pop(); }
            }
            ToolButtonRow {
                ToolButton {
                    text:"Save"
                    onClicked: {
                        appcontroller.stepLength = stepLength.text;
                        appcontroller.daily = daily.text * 1000.0;
                    }
                }
            }
        }
}
