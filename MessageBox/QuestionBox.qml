import QtQuick 2.7

MessageBox {
	id: questionBox

	signal questionAnswered(bool accepted)

	function askQuestion(title, message, yesText, noText, resultAction) {
		questionBox.title = title;
		questionBox.message = message;
		if(typeof yesText == "string")
			questionBox.positiveButtonText = yesText;
		if(typeof noText == "string")
			questionBox.negativeButtonText = noText;
		if(typeof resultAction == "function")
			questionBox.questionAnswered.connect(resultAction);
		questionBox.open();
	}

	positiveButtonText: qsTr("Yes")
	negativeButtonText: qsTr("No")

	onPositiveAction: questionAnswered(true)
	onNegativeAction: questionAnswered(false)
}
