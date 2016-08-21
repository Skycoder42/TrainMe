import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0

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

		Material.background: "transparent"
		Material.elevation: 0

		Layout.preferredWidth: implicitWidth

		onClicked: neutralClicked()
	}

	Item {
		id: spacer
		Layout.fillWidth: true
	}

	Button {
		id: negativeButton
		text: p.showNegativeButton ? negativeButtonText : ""
		visible: p.showNegativeButton

		Material.background: "transparent"
		Material.elevation: 0

		Layout.preferredWidth: implicitWidth

		onClicked: negativeClicked()
	}

	Button {
		id: positiveButton
		text: p.showPositiveButton ? positiveButtonText : ""
		visible: p.showPositiveButton

		Material.foreground: Material.primary
		Material.background: "transparent"
		Material.elevation: 0

		Layout.preferredWidth: implicitWidth

		onClicked: positiveClicked()
	}
}
