import QtQml 2.2
import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import lib 1.0

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
                onTriggered: openFileDialog.open()
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
                onTriggered: Qt.quit()
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
            spacing: 3

            RowLayout {
                Label {
                    text: qsTr("Calcul method :")
                }

                ComboBox {
                    model: [ "Sequential", "OpenMP", "pThread" ]
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
            Label { text: qsTr("%1 vertices | %2 faces").arg(obj.nbVertices).arg(obj.nbFaces) }
        }
    }

    Object {
        id: obj
    }

    FileDialog {
        id: openFileDialog
        title: "Please choose an object"
        folder: shortcuts.home
        nameFilters: [ "OFF files (*.off)", "All files (*)" ]
        onAccepted: {
            console.log("Read OFF : "+fileUrl);
            obj.readOFF(fileUrl);
        }
    }

    Scene3D {
        id: scene3d
        anchors.fill: parent
        focus: true
        aspects: ["input", "logic"]
        cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

        Entity {
            id: sceneRoot

            Camera {
                id: camera
                projectionType: CameraLens.PerspectiveProjection
                fieldOfView: 45
                nearPlane : 0.1
                farPlane : 1000.0
                position: Qt.vector3d( 0.0, 0.0, 40.0 )
                upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
                viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
            }

            FirstPersonCameraController { camera: camera }

            components: [
                RenderSettings {
                    activeFrameGraph: ForwardRenderer {
                        camera: camera
                        clearColor: "transparent"
                    }
                },
                InputSettings { }
            ]

            PhongMaterial {
                id: material
            }

            TorusMesh {
                id: torusMesh
                radius: 5
                minorRadius: 1
                rings: 100
                slices: 20
            }

            Transform {
                id: torusTransform
                scale3D: Qt.vector3d(1.5, 1, 0.5)
                rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 45)
            }

            Entity {
                id: torusEntity
                components: [ torusMesh, material, torusTransform ]
            }
        }
    }
}
