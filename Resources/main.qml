import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import "controls"
import "../qml/MessageBox"

ApplicationWindow {
	id:root
	visible: true
	width: 700
	height: 500
	title: qsTr("Train Me!")

	readonly property bool isSmall: root.height > root.width

	Material.theme: Material.Light
	Material.primary: Material.color(Material.Red, Material.Shade500)
	Material.accent: Material.color(Material.Cyan, Material.Shade500)
	Material.foreground: "#212121"

	Universal.theme: Universal.Light
	Universal.accent: hasMainColor ? mainColor : Universal.color(Universal.Red)//TODO via settings
	Universal.foreground: "#212121"

	header: ToolBar {
		id: toolbar
		Material.foreground: "#FFFFFF"
		height: 56

        property alias moreMenu: moreButton.menu

		RowLayout {
			spacing: 0
			anchors.fill: parent

			AppBarButton {
				id: drawerButton
				enabled: false
                imageSource: "qrc:/icons/menu.png"
				text: qsTr("Open Menu")
				onClicked: drawer.open()
			}

			Label {
				id: titleLabel
				font.pointSize: 20
				font.bold: app.testStyle("Material")
				text: qsTr("Train Me!")
				elide: Label.ElideRight
				horizontalAlignment: Qt.AlignLeft
				verticalAlignment: Qt.AlignVCenter
				Layout.fillWidth: true

				function updateText(text) {
					newText = text;
					changeLabelAnimation.start();
				}

				property string newText;

				SequentialAnimation {
					id: changeLabelAnimation
					running: false
					NumberAnimation {
						target: titleLabel
						property: "opacity"
						to: 0.0
						duration: 200
						easing.type:  Easing.InQuad
					}
					PropertyAnimation {
						target: titleLabel
						property: "text"
						to: titleLabel.newText
						duration: 0
					}
					NumberAnimation {
						target: titleLabel
						property: "opacity"
						to: 1.0
						duration: 200
						easing.type: Easing.OutQuad
					}
				}
			}

            AppBarMenuButton {
                id: moreButton
            }
		}
	}

	Drawer {
		id: drawer
		width: Math.min(300, root.width)
		height: root.height

		ListView {
			id: drawerListView
			currentIndex: -1
			anchors.fill: parent

			function setPage(index, model) {
				if(typeof model == "undefined")
					model = drawerListView.model.get(index);
				stackView.replace(model.source);
				drawerListView.currentIndex = index;
				titleLabel.updateText(model.title);
			}

			delegate: ItemDelegate {
				width: parent.width
				text: model.title
				highlighted: ListView.isCurrentItem
				onClicked: {
					if (drawerListView.currentIndex !== index)
						drawerListView.setPage(index, model);
					drawer.close();
				}
			}

			model: ListModel {
				ListElement { title: qsTr("Training!"); source: "qrc:/qml/pages/TrainPage.qml" }
				ListElement { title: qsTr("Manage Results"); source: "qrc:/qml/pages/ResultPage.qml" }
				ListElement { title: qsTr("Create Tasks"); source: "qrc:/qml/pages/CreateTaskPage.qml" }
				ListElement { title: qsTr("Configure Weekplan"); source: "qrc:/qml/pages/ConfigWeekPage.qml" }
				ListElement { title: qsTr("Configure Reminders"); }//source: "qrc:/qml/pages/ControlPage.qml" }
				ListElement { title: qsTr("Settings"); }//source: "qrc:/qml/pages/ControlPage.qml" }
				ListElement { title: qsTr("Motivate Me!"); }//source: "qrc:/qml/pages/MotivatePage.qml" }
				ListElement { title: qsTr("God Mode"); }//source: "qrc:/qml/pages/ControlPage.qml" }
			}

			ScrollIndicator.vertical: ScrollIndicator { }
		}
	}

	StackView {
		id: stackView
		anchors.fill: parent

		initialItem: BusyIndicator {
			anchors.centerIn: parent
			property int mSize: Math.max(implicitWidth, Math.min(root.width, root.height) * 0.2)
			width: mSize
			height: mSize
		}
	}

	Connections {
		target: app
		onStartupCompleted: {
			drawerListView.setPage(startIndex);
			drawerButton.enabled = true;
		}
	}
	Component.onCompleted: app.trainManager.initManager()

	StyleAccess {
		id: globalStyleAccess
	}

	ProgressBox {
		id: savingProgress

		title: qsTr("Operating")
		message: qsTr("Please wait, while the app loads/saves the data…")

		property int activeCounter: 0

		Timer {
			id: showUpTimer
			interval: 250
			running: false
			repeat: false

			onTriggered: savingProgress.open()
		}

		Connections {
			target: app.trainManager
			onOperationStarted: {
				if(savingProgress.activeCounter == 0)
					showUpTimer.start();
				savingProgress.activeCounter++;
			}

			onOperationCompleted: {
				savingProgress.activeCounter--;
				if(savingProgress.activeCounter == 0) {
					showUpTimer.stop();
					savingProgress.close();
				}
			}
		}
	}

	ErrorBox {
		id:errorDialog

		Connections {
			target: app
			onErrorMessage : errorDialog.showError(title, message, isFatal)
		}
	}
}
