import QtQuick 2.7

MessageBox {
	id: errorBox

	property bool isFatal : false

	function showError(title, message, isFatal) {
		errorBox.title = title;
		errorBox.message = message;
		errorBox.isFatal = isFatal;
		errorBox.open();
	}

	positiveButtonText: isFatal ? qsTr("Exit") : qsTr("Ok")
	onClosed: {
		if(isFatal)
			Qt.quit();
	}
}
