import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0

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
				enabled: false
				contentItem: Image {
					fillMode: Image.PreserveAspectFit
					horizontalAlignment: Image.AlignHCenter
					verticalAlignment: Image.AlignVCenter
					source: "qrc:/icons/menu.png"
				}
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

	BusyIndicator {
		anchors.centerIn: parent
		property int mSize: Math.max(implicitWidth, Math.min(parent.width, parent.height) * 0.2)
		width: mSize
		height: mSize
		visible: app.loading
	}

	Flickable {
		id: flickable
		anchors.fill: parent
		visible: !app.loading

		contentHeight: mainContent.height

		ScrollBar.vertical: ScrollBar {}

		Pane {
			id: mainContent
			width: flickable.width

			GridLayout {
				id: layout

				anchors.left: parent.left
				anchors.right: parent.right

				columnSpacing: 14
				rowSpacing: 14
				columns: root.isSmall ? 1 : 2

				GroupBox {
					id: ttt

					Layout.fillWidth: true
					Layout.preferredHeight: implicitHeight
					Layout.fillHeight: true
					implicitWidth: 200

					title: qsTr("Strengh-Training")

					TrainListView {
						id: strengthList
						anchors.fill: parent
						model: strengthModel
					}
				}

				GroupBox {
					Layout.fillWidth: true
					Layout.preferredHeight: implicitHeight
					Layout.fillHeight: true
					implicitWidth: 200

					title: qsTr("Agility-Training")

					TrainListView {
						id: agilityList
						anchors.fill: parent
						model: agilityModel
					}
				}

				GroupBox {
					Layout.fillWidth: true
					implicitHeight: root.height
					Layout.columnSpan: root.isSmall ? 1 : 2

					title: qsTr("Test")
				}
			}
		}
	}

	ErrorDialog {
		id:errorDialog

		Connections {
			target: app
			onErrorMessage : errorDialog.showError(title, message, isFatal)
		}
	}
}
