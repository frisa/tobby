import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "qrc:/../imports"

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("tobby")
    opacity: 1
    TreeView {
        anchors.fill: parent
        model: fileSystemModel

        delegate: TreeItemView {
            arrow: Label {
                font.family: "monospace"
                text: model.hasChildren ? ">" : " "
                rotation: model.isExpanded ? 90 : 0
                verticalAlignment: Label.AlignVCenter
            }

            item: Label {
                property var contextMenu: Menu {
                    y: parent.implicitHeight

                    MenuItem {
                        text: "Delete"
                    }

                    MenuItem {
                        text: "Add new"
                    }
                }

                text: model.display
                verticalAlignment: Label.AlignVCenter
            }
        }
    }
    menuBar: MenuBar{
        Menu{
            title: qsTr("&File")
            Action { text: qsTr("&New...") }
            Action { text: qsTr("&Open...") }
            Action { text: qsTr("&Save") }
            Action { text: qsTr("Save &As...") }
            MenuSeparator { }
            Action { text: qsTr("&Quit") }
        }
        Menu{
            title: qsTr("&About")
            Action { text: qsTr("&Help") }
        }
    }
    header: ToolBar {
            id: headerBar
            RowLayout{
                anchors.fill: parent
                ToolButton {
                     text: qsTr("New")
                     //onClicked: stack.pop()
                }
            }
    }
    footer: TabBar {
        id: footerBar
        width: parent.width
        TabButton {
            text: qsTr("First")
            width: implicitWidth
        }
    }
}
