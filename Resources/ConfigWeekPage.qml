import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import "../controls"

ControlPage {
    id: configWeekPage

    ListView {
        id: weekConfigList

		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.bottom: scoreGroup.top
		clip: true

        ScrollBar.vertical: ScrollBar {}

        model: ListModel {
            ListElement {
                weekday: "Monday"
                increment: 0
                addTask: false
            }
            ListElement {
                weekday: "Tuesday"
                increment: 3
                addTask: false
            }
            ListElement {
                weekday: "Sunday"
                increment: 0
                addTask: true
            }
        }

		header: Item {
			width: parent.width
			height: rowLayout.implicitHeight + 14
			RowLayout {
				id: rowLayout
				width: parent.width
				height: parent.height
				spacing: 14

				Label {
					id: nameLabel
					Layout.fillWidth: true
					Layout.alignment: Qt.AlignVCenter
					Layout.preferredWidth: 200
					Layout.minimumWidth: nameLabel.implicitWidth
					text: qsTr("<i>Weekday</i>")
					leftPadding: 14
					horizontalAlignment: Qt.AlignLeft
				}

				Label {
					id: increaseLabel
					Layout.fillWidth: true
					Layout.alignment: Qt.AlignVCenter
					Layout.preferredWidth: 200
					Layout.minimumWidth: increaseLabel.implicitWidth
					text: qsTr("<i>Increase</i>")
					horizontalAlignment: Qt.AlignHCenter
				}

				Label {
					id: addLabel
					Layout.fillWidth: true
					Layout.alignment: Qt.AlignVCenter
					Layout.preferredWidth: 200
					Layout.minimumWidth: addLabel.implicitWidth
					text: qsTr("<i>Add Task</i>")
					horizontalAlignment: Qt.AlignHCenter
				}
			}

			Rectangle {
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.bottom: parent.bottom
				height: 1
				color: globalStyleAccess.accentColor
			}
		}

        delegate: WeekConfigDelegate {
            itemName: model.weekday
            itemIncrement: model.increment
            itemAddTask: model.addTask
            width: parent.width
        }
    }

	GroupBox {
		id: scoreGroup
		title: qsTr("Score Settings")

		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.margins: 14
	}
}
