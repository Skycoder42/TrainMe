import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

RowLayout {
	spacing: 7

	property var positiveButtonText
	property var negativeButtonText
	property var neutralButtonText

	signal positiveClicked();
	signal negativeClicked();
	signal neutralClicked();

	QtObject {
		id: p

		property bool showPositiveButton: typeof positiveButtonText == "string"
		property bool showNegativeButton: typeof negativeButtonText == "string"
		property bool showNeutralButton: typeof neutralButtonText == "string"
	}

	Button {
		id: neutralButton
		text: p.showNeutralButton ? neutralButtonText : ""
		visible: p.showNeutralButton

		Layout.preferredWidth: 0
		Layout.fillWidth: true

		onClicked: neutralClicked()
	}

	Button {
		id: negativeButton
		text: p.showNegativeButton ? negativeButtonText : ""
		visible: p.showNegativeButton

		Layout.preferredWidth: 0
		Layout.fillWidth: true

		onClicked: negativeClicked()
	}

	Button {
		id: positiveButton
		text: p.showPositiveButton ? positiveButtonText : ""
		visible: p.showPositiveButton

		Layout.preferredWidth: 0
		Layout.fillWidth: true

		onClicked: positiveClicked()
	}
}
