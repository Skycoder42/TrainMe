import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import "../qml/MessageBox"

ApplicationWindow {
	id:root
	visible: true
	width: 640
	height: 480
	title: qsTr("Train Me")

	property bool isSmall: root.height > root.width

	Material.theme: Material.Light
	Material.primary: Material.Red
	Material.accent: Material.Cyan
	Material.foreground: "#212121"

	Universal.theme: Universal.Light
	Universal.accent: hasMainColor ? mainColor : Universal.Red//TODO via settings
	Universal.foreground: "#212121"

	header: ToolBar {
		Material.foreground: "#FFFFFF"

		RowLayout {
			spacing: 20
			anchors.fill: parent

			ToolButton {
				id: toolbar
				enabled: !app.loading
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
				text: qsTr("Train Me!")
				font.pointSize: 20
				elide: Label.ElideRight
				horizontalAlignment: Qt.AlignLeft
				verticalAlignment: Qt.AlignVCenter
				Layout.fillWidth: true
			}
		}
	}

	Drawer {
		id: drawer
		x: 0
		y: 100
		width: Math.min(root.width, root.height) *0.66
		height: root.height

		ListView {
			id: drawerListView
			currentIndex: -1
			anchors.fill: parent

			function setPage(index) {
				drawerListView.currentIndex = index;
				var item = drawerListView.model.get(index);
				stackView.replace(item.source);
			}

			delegate: ItemDelegate {
				width: parent.width
				text: model.title
				highlighted: ListView.isCurrentItem
				onClicked: {
					if (drawerListView.currentIndex !== index) {
						drawerListView.currentIndex = index;
						titleLabel.text = model.title;
						stackView.replace(model.source);
					}
					drawer.close();
				}
			}

			model: ListModel {
				ListElement { title: qsTr("Training!"); source: "qrc:/qml/controls/TrainPage.qml" }
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
		onLoadingChanged: {
			if(!app.loading)
				drawerListView.setPage(0);
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
