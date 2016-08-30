import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0

CheckDelegate {
	id:delegate

	property alias itemName: delegate.text
	property int itemReps : 0
	property alias itemChecked: delegate.checked

	contentItem: RowLayout {
		Label {
			Layout.fillHeight: true
			Layout.fillWidth: true
			text: delegate.text
		}

		Label {
			id: repLabel
			Layout.fillHeight: true
			text: qsTr("Reps: <b>%1</b>").arg(delegate.itemReps)
		}

		Item {
			Layout.minimumWidth: implicitWidth
			Layout.maximumWidth: implicitWidth
			implicitWidth: delegate.indicator.width + 7
		}
    }
}
