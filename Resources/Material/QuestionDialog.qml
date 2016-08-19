import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0

Popup {
	id: errorDialog
	x: (parent.width - width) / 2
	y: (parent.height - height) / 2

	modal: true
	focus: true
	closePolicy: Popup.CloseOnEscape

	function showError(title, message, isFatal) {
		titleLabel.text = title;
		contentLabel.text = message;
		p.isFatal = isFatal;
		errorDialog.open();
	}

	onClosed: {
		if(p.isFatal)
			Qt.quit();
	}

	QtObject {
		id: p

		property bool isFatal : false
		property int maxWidth: Math.min(errorDialog.parent.width, errorDialog.parent.height) * 0.9
		property int minWidth: errorDialog.parent.width / 2
	}

	contentItem: ColumnLayout {
		id: settingsColumn
		spacing: 20

		Label {
			id: titleLabel

			Layout.minimumWidth: p.minWidth
			Layout.maximumWidth: p.maxWidth

			font.bold: true
			elide: Text.ElideRight
			maximumLineCount: 1
		}

		Label {
			id: contentLabel

			Layout.minimumWidth: p.minWidth
			Layout.maximumWidth: p.maxWidth

			wrapMode: Text.Wrap
		}


		RowLayout {
			spacing: 10

			Item {
				id: spacer
				Layout.fillWidth: true
			}

			Button {
				id: okButton
				text: p.isFatal ? qsTr("Exit") : qsTr("Ok")
				onClicked: errorDialog.close()

				Material.foreground: Material.primary
				Material.background: "transparent"
				Material.elevation: 0

				Layout.preferredWidth: implicitWidth
			}
		}
	}
}
