import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Popup {
	id: messageBox

	property alias title: titleLabel.text
	property alias message: contentLabel.text
	property alias messageContent: content.sourceComponent
	property alias messageContentItem: content.item
	property alias positiveButtonText: buttonBox.positiveButtonText
	property alias negativeButtonText: buttonBox.negativeButtonText
	property alias neutralButtonText: buttonBox.neutralButtonText

	signal positiveAction();
	signal negativeAction(bool escapePressed);
	signal neutralAction();

	function showMessage(title, message, positiveButton, negativeButton, neutralButton) {
		messageBox.title = title;
		messageBox.message = message;

		if(typeof positiveButton == "string")
			messageBox.positiveButtonText = positiveButton;
		else if(typeof positiveButton == "object") {
			if(typeof positiveButton.text == "string")
				messageBox.positiveButtonText = positiveButton.text;
			if(typeof positiveButton.action == "function")
				messageBox.positiveAction.connect(positiveButton.action);
		}

		if(typeof negativeButton == "string")
			messageBox.negativeButtonText = negativeButton;
		else if(typeof negativeButton == "object") {
			if(typeof negativeButton.text == "string")
				messageBox.negativeButtonText = negativeButton.text;
			if(typeof negativeButton.action == "function")
				messageBox.negativeAction.connect(negativeButton.action);
		}

		if(typeof neutralButton == "string")
			messageBox.neutralButtonText = neutralButton;
		else if(typeof neutralButton == "object") {
			if(typeof neutralButton.text == "string")
				messageBox.neutralButtonText = neutralButton.text;
			if(typeof neutralButton.action == "function")
				messageBox.neutralAction.connect(neutralButton.action);
		}

		messageBox.open();
	}

	x: (parent.width - width) / 2
	y: (parent.height - height) / 2
	modal: true
	focus: true
	closePolicy: Popup.CloseOnEscape

	onOpened: p.closedByAction = false;
	onClosed: {
		if(!p.closedByAction)
			negativeAction(true);
	}

	QtObject {
		id: p

		property int boxWidth: Math.min(250, messageBox.parent.width - 42)
		property bool closedByAction : false
		property bool hasText: contentLabel.text ? contentLabel.text.length > 0 : false
	}

	contentItem: ColumnLayout {
		id: settingsColumn
		spacing: 14

		Label {
			id: titleLabel

			Layout.minimumWidth: p.boxWidth
			Layout.maximumWidth: p.boxWidth

			font.bold: true
			elide: Text.ElideRight
			maximumLineCount: 1
		}

		RowLayout {
			id: contentRow

			spacing: 14
			Layout.minimumWidth: p.boxWidth
			Layout.maximumWidth: p.boxWidth

			Loader {
				id: content

				Layout.preferredWidth: content.item ? content.item.implicitWidth : 0
				Layout.maximumWidth: p.boxWidth
				Layout.fillWidth: !p.hasText
			}

			Label {
				id: contentLabel				
				visible: p.hasText

				wrapMode: Text.Wrap
				Layout.maximumWidth: p.boxWidth
				Layout.fillWidth: true
			}
		}

		ButtonBox {
			id: buttonBox

			Layout.minimumWidth: p.boxWidth
			Layout.maximumWidth: p.boxWidth

			onPositiveClicked: {
				p.closedByAction = true;
				positiveAction();
				messageBox.close();
			}

			onNegativeClicked: {
				p.closedByAction = true;
				negativeAction(false);
				messageBox.close();
			}

			onNeutralClicked: {
				p.closedByAction = true;
				neutralAction();
				messageBox.close();
			}
		}
	}
}
