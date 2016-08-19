import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Page {
	id:trainPage

	Flickable {
		id: flickable
		anchors.fill: parent

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

				Button {
					id: doneButton
					Layout.preferredWidth: implicitWidth * 1.2
					anchors.horizontalCenter: parent.horizontalCenter

					readonly property bool isAllDone: strengthModel.allDone && agilityModel.allDone

					text: isAllDone ? qsTr("Training Done!") : qsTr("Training Failed!")
					highlighted: isAllDone
				}
			}
		}
	}
}
