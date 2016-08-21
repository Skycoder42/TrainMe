import QtQuick 2.7
import QtQuick.Controls 2.0

MessageBox {
	id: progressBox

	property bool cancable: false

	signal canceled();

	function showProgress(title, message) {
		progressBox.title = title;
		progressBox.message = message;
		progressBox.open();
	}

	closePolicy: cancable ? Popup.CloseOnEscape : Popup.NoAutoClose
	messageContent: contentComponent
	negativeButtonText: cancable ? qsTr("Cancel") : undefined

	onNegativeAction: canceled();

	Component {
		id: contentComponent

		BusyIndicator {}
	}
}
