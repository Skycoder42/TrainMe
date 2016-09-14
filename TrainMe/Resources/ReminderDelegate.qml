import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ItemDelegate {
	id: delegate

	property date time: new Date()
	property bool intense: false

	signal deleteClicked

	text: qsTr("At <b>%1</b> %2")
		  .arg(delegate.time.toLocaleTimeString(Locale.ShortFormat))
		  .arg(delegate.intense ? qsTr("(intense)") : "")

	highlighted: delegate.intense
	down: false

	contentItem: RowLayout {
		spacing: 0

		Label {
			Layout.fillWidth: true
			text: delegate.text
			verticalAlignment: Qt.AlignVCenter
			elide: Text.ElideRight
		}

		ToolButton {
			id: deleteButton
			Layout.preferredHeight: 36
			Layout.preferredWidth: 36
			flat: true

			contentItem: Item {
				Image {
					anchors.centerIn: parent
					width: 24
					height: 24
					source: "qrc:/icons/delete.png"
				}
			}

			onClicked: delegate.deleteClicked()
		}
	}
}
