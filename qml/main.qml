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
            Label { text: qsTr("%1 vertices | %2 faces | centroid : %3 | min : %4 | max : %5").arg(obj.nbVertices).arg(obj.nbFaces).arg(obj.centroid.toString()).arg(obj.min.toString()).arg(obj.max.toString()) }
        }
    }

    FileDialog {
        id: openFileDialog
        title: "Please choose an object"
        folder: shortcuts.home
        nameFilters: [ "OFF files (*.off)", "All files (*)" ]
        onAccepted: {
            console.log("Read OFF : "+fileUrl);
            obj.readOFF(fileUrl);
            obj.load();
            //sphereMesh.source = fileUrl;
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
                id: camer
                projectionType: CameraLens.PerspectiveProjection
                fieldOfView: 22
                nearPlane : 0.01
                farPlane : 1000.0
                position: Qt.vector3d( 0.0, 0.0, -10.0 )
                upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
                viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
            }

            FirstPersonCameraController { camera: camer }

            components: [
                RenderSettings {
                    activeFrameGraph: ForwardRenderer {
                        id: renderer
                        camera: camer
                        clearColor: "grey"

                        RenderStateSet {
                            CullFace {
                                id: cullFace
                                mode: CullFace.NoCulling
                            }

                            FrontFace {
                                id: frontFace
                                direction: FrontFace.ClockWise
                            }
                        }
                    }
                },
                InputSettings { }
            ]


            Object {
                id: obj
            }

            SphereMesh {
                id: me
                radius: 3
            }

            WireframeMaterial {
                id: wireframeMaterial
                effect: WireframeEffect {}
                ambient: Qt.rgba( 0.2, 0.0, 0.0, 1.0 )
                diffuse: Qt.rgba( 0.8, 0.0, 0.0, 1.0 )
            }

            Entity {
                id: objEntity

                components: [ obj, wireframeMaterial ]
            }
        }
    }
}
