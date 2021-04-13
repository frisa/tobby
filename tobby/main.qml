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
//        TreeView {
//            Layout.row: 0
//            Layout.rowSpan: 1
//            Layout.column: 0
//            anchors.fill: parent
//            model: dataModel
//            delegate: TreeItemView {
//                arrow: Label {
//                    font.family: "monospace"
//                    text: model.hasChildren ? ">" : " "
//                    rotation: model.isExpanded ? 90 : 0
//                    verticalAlignment: Label.AlignVCenter
//                }
//                item: Label {
//                    property var contextMenu: Menu {
//                        y: parent.implicitHeight
//                        MenuItem {
//                            text: "Delete"
//                        }
//                        MenuItem {
//                            text: "Add new"
//                        }
//                    }
//                    text: model.display
//                    verticalAlignment: Label.AlignVCenter
//                }
//            }
//        }
        Rectangle{
            Layout.row: 0
            Layout.column: 0
            Layout.rowSpan: 12
            Layout.columnSpan: 2
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: Layout.columnSpan
            Layout.preferredHeight: Layout.rowSpan
            color: 'blue'
        }
        Rectangle{
            Layout.row: 0
            Layout.column: 2
            Layout.rowSpan: 10
            Layout.columnSpan: 10
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: Layout.columnSpan
            Layout.preferredHeight: Layout.rowSpan
            color: 'red'
        }
        Rectangle{
            Layout.row: 10
            Layout.column: 2
            Layout.rowSpan: 2
            Layout.columnSpan: 10
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: Layout.columnSpan
            Layout.preferredHeight: Layout.rowSpan
            color: 'green'
        }
//        TabBar {
//            Layout.row: 0
//            Layout.column: 1
//            id: bar
//            //width: parent.width
//            TabButton {
//                text: qsTr("Home")
//            }
//            TabButton {
//                text: qsTr("Discover")
//            }
//            TabButton {
//                text: qsTr("Activity")
//            }
//        }
//        StackLayout {
//            Layout.row: 1
//            Layout.column: 1
//            //width: parent.width
//            currentIndex: bar.currentIndex
//            Item {
//                id: homeTab
//            }
//            Item {
//                id: discoverTab
//            }
//            Item {
//                id: activityTab
//            }
//        }
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
