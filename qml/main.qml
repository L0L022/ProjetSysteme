import QtQml 2.2
import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: window
    visible: true
    title: qsTr("ProjetSysteme")
    width: 800
    height: 400

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")

            MenuItem {
                text: qsTr("Open a file")
                iconName: "document-open"
            }

            Menu {
                id: exampleMenu
                title: qsTr("Open an example")
                iconName: "template"

                Instantiator {
                    model: ["truc.off", "machin.off"]

                    MenuItem {
                        text: modelData
                        iconName: "template"
                    }

                    onObjectAdded: exampleMenu.insertItem(index, object)
                    onObjectRemoved: exampleMenu.removeItem(object)
                }
            }

            MenuSeparator {}

            MenuItem {
                text: qsTr("Save")
                iconName: "document-save"
            }

            MenuItem {
                text: qsTr("Close")
                iconName: "document-close"
            }

            MenuSeparator {}

            MenuItem {
                text: qsTr("Quit")
                iconName: "application-exit"
            }
        }

        Menu {
            title: qsTr("Help")

            MenuItem {
                text: qsTr("Manual")
                iconName: "help-contents"
            }

            MenuSeparator {}

            MenuItem {
                text: qsTr("About Qt")
                iconName: "help-about"
            }

            MenuItem {
                text: qsTr("About the application")
                iconName: "help-about"
            }
        }
    }

    toolBar: ToolBar {
        Row {
            anchors.fill: parent

            RowLayout {
                Label {
                    text: qsTr("Calcul method :")
                }

                ComboBox {
                    model: [ "Sequentiel", "OpenMP", "pThread" ]
                }
            }

            ToolButton {
                text: qsTr("Calculate")
            }

            ToolButton {
                text: "|"
                enabled: false
            }

            ToolButton {
                text: qsTr("Show vertex normal")
                checkable: true
            }

            ToolButton {
                text: qsTr("Show face normal")
                checkable: true
            }

            ToolButton {
                text: qsTr("Show vertices")
                checkable: true
            }

            ToolButton {
                text: qsTr("Show edges")
                checkable: true
            }
        }
    }

    statusBar: StatusBar {
        RowLayout {
            anchors.fill: parent
            Label { text: "1265 vertices | 456 faces" }
        }
    }
}
