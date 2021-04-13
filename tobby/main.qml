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
    opacity: 1
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
                    text: qsTr("Home")
                }
                TabButton {
                    text: qsTr("Discover")
                }
                TabButton {
                    text: qsTr("Activity")
                }
            }
            StackLayout {
                Layout.columnSpan: 10
                Layout.fillWidth: true
                currentIndex: bar.currentIndex
                Item {
                    id: homeTab
                    Rectangle{
                        anchors.fill: parent
                        color: 'blue'
                    }
                }
                Item {
                    id: discoverTab
                    Rectangle{
                        anchors.fill: parent
                        color: 'green'
                    }
                }
                Item {
                    id: activityTab
                    Rectangle{
                        anchors.fill: parent
                        color: 'red'
                    }
                }
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
