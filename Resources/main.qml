import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import "../qml/MessageBox"

ApplicationWindow {
	id:root
	visible: true
	width: 700 * app.dp
	height: 500 * app.dp
	title: qsTr("Train Me")

	property bool isSmall: root.height > root.width

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
		height: 48 * app.dp

		RowLayout {
			spacing: 14 * app.dp
			anchors.fill: parent

			ToolButton {
				id: drawerButton
				enabled: false
				contentItem: Image {
					fillMode: Image.PreserveAspectFit
					horizontalAlignment: Image.AlignHCenter
					verticalAlignment: Image.AlignVCenter
					source: "qrc:/icons/menu.png"
				}
				onClicked: drawer.open()
			}

			Label {
				id: titleLabel
				font.pointSize: 20
				text: qsTr("Train Me")
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
		}
	}

	Drawer {
		id: drawer
		width: Math.min(300 * app.dp, root.width)
		height: root.height

		ListView {
			id: drawerListView
			currentIndex: -1
			anchors.fill: parent

			function setPage(index, model) {
				if(typeof model == "undefined")
					model = drawerListView.model.get(index);
				drawerListView.currentIndex = index;
				titleLabel.updateText(model.title);
				stackView.replace(model.source);
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
				ListElement { title: qsTr("Training!"); source: "qrc:/qml/controls/TrainPage.qml" }
				ListElement { title: qsTr("Manage Results"); source: "qrc:/qml/controls/ResultPage.qml" }
				ListElement { title: qsTr("Configure Weekplan"); source: "qrc:/qml/controls/ControlPage.qml" }
				ListElement { title: qsTr("Configure Tasks"); source: "qrc:/qml/controls/ControlPage.qml" }
				ListElement { title: qsTr("Configure Reminders"); source: "qrc:/qml/controls/ControlPage.qml" }
				ListElement { title: qsTr("Settings"); source: "qrc:/qml/controls/ControlPage.qml" }
				ListElement { title: qsTr("Motivate Me!"); source: "qrc:/qml/controls/ControlPage.qml" }
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

	ProgressBox {
		id: savingProgress

		title: qsTr("Operating")
		message: qsTr("Please wait, while the app loads/saves the data…")

		property int activeCounter: 0

		Connections {
			target: app.trainManager
			onOperationStarted: {
				if(savingProgress.activeCounter == 0)
					savingProgress.open();
				savingProgress.activeCounter++;
			}

			onOperationCompleted: {
				savingProgress.activeCounter--;
				if(savingProgress.activeCounter == 0)
					savingProgress.close();
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
