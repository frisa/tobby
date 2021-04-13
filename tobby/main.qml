import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "qrc:/../imports"

ApplicationWindow {
    width: 800
    height: 500
    visible: true
    title: qsTr("tobby")
    opacity: 0.97
    GridLayout {
        id: layout
        anchors.fill: parent
        columns: 12
        rows: 12
        TreeView {
            Layout.row: 0
            Layout.column: 0
            Layout.rowSpan: 12
            Layout.columnSpan: 2
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: Layout.columnSpan
            Layout.preferredHeight: Layout.rowSpan
            model: dataModel
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
        ColumnLayout{
            Layout.row: 0
            Layout.column: 2
            Layout.rowSpan: 12
            Layout.columnSpan: 10
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: Layout.columnSpan
            Layout.preferredHeight: Layout.rowSpan
            TabBar {
                id: bar
                TabButton {
                    width: implicitWidth
                    text: qsTr("3D Space")
                }
                TabButton {
                    width: implicitWidth
                    text: qsTr("Home")
                }
                TabButton {
                    width: implicitWidth
                    text: qsTr("Help")
                }
            }
            StackLayout {
                Layout.columnSpan: 10
                Layout.fillWidth: true
                currentIndex: bar.currentIndex
                Item {
                    id: homeTab
                    Text{
                        text: "3D"
                    }
                }
                Item {
                    id: discoverTab
                    Text{
                        text: "HOME"
                    }
                }
                Item {
                    id: activityTab
                    Text{
                        text: "HELP"
                    }
                }
            }
        }
    }
    menuBar: MenuBar{
        Menu{
            title: qsTr("&File")
            Action {
                text: qsTr("&New...")
                icon.source: "qrc:/icons/icons/new.png"
            }
            Action {
                text: qsTr("&Open...")
                icon.source: "qrc:/icons/icons/open.png"
            }
            Action {
                text: qsTr("&Save")
                icon.source: "qrc:/icons/icons/save.png"
            }
            Action {
                text: qsTr("Save &As...")
                icon.source: "qrc:/icons/icons/saveall.png"
            }
            MenuSeparator { }
            Action {
                text: qsTr("&Quit")
                icon.source: "qrc:/icons/icons/exit.png"
            }
        }
        Menu{
            title: qsTr("&About")
            Action {
                text: qsTr("&Help")
                icon.source: "qrc:/icons/icons/help.png"
            }
        }
    }
    header: ToolBar {
            id: headerBar
            RowLayout{
                ToolButton {
                     icon.source: "qrc:/icons/icons/new.png"
                }
                ToolButton {
                     icon.source: "qrc:/icons/icons/open.png"
                }
                ToolButton {
                     icon.source: "qrc:/icons/icons/save.png"
                }
                ToolButton {
                     icon.source: "qrc:/icons/icons/saveall.png"
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
