import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ItemDelegate {
    id: delegate

    property alias itemName: nameLabel.text
    property alias itemIncrement : incrementBox.value
	property alias itemAddTask: addTaskBox.checked

	hoverEnabled: true
	down: hovered
	contentItem: Item {
		id:mainContent
		implicitHeight: Math.max(nameLabel.implicitHeight, incrementBox.implicitHeight, addTaskBox.implicitHeight)

		Label {
			id: nameLabel
			anchors.verticalCenter: parent.verticalCenter
			anchors.left: parent.left
			width: 80
		}

		Item {
			id: centerWrapper
			anchors.verticalCenter: parent.verticalCenter
			anchors.left: nameLabel.right
			anchors.right: addTaskBox.left

			SpinBox {
				id: incrementBox
				anchors.centerIn: parent
				editable: true
				to: 42

				width: Math.min(implicitWidth, centerWrapper.width)
			}
		}

		CheckBox {
			id: addTaskBox
			anchors.verticalCenter: parent.verticalCenter
			anchors.right: parent.right
		}
	}
}
