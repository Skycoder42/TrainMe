import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Universal 2.0

Popup {
	id: questionDialog
	x: (parent.width - width) / 2
	y: (parent.height - height) / 2

	modal: true
	focus: true
	closePolicy: Popup.CloseOnEscape

	signal yesSelected()
	signal noSelected()

	function showQuestion(title, message, yesText, noText) {
		titleLabel.text = title;
		contentLabel.text = message;
		yesButton.text = yesText !== undefined ? yesText : p.defaultYes
		noButton.text = noText !== undefined ? noText : p.defaultNo
		questionDialog.open();
	}

	onClosed: {
		if(p.isFatal)
			Qt.quit();
	}

	QtObject {
		id: p

		readonly property string defaultYes: qsTr("Yes")
		readonly property string defaultNo: qsTr("No")
		property int maxWidth: Math.min(questionDialog.parent.width, questionDialog.parent.height) * 0.9
		property int minWidth: questionDialog.parent.width / 2
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
			id: buttonRow
			Layout.fillWidth: true

			Button {
				id: noButton
				text: p.defaultNo
				onClicked: questionDialog.close()
				Layout.preferredWidth: 0
				Layout.fillWidth: true
			}

			Button {
				id: yesButton
				text: p.defaultYes
				onClicked: questionDialog.close()
				Layout.preferredWidth: 0
				Layout.fillWidth: true
			}
		}
	}
}
