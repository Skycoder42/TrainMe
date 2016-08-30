import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ListView {
	implicitHeight: heightHelper.height * model.rowCount;

	clip: true
	ScrollBar.vertical: ScrollBar {}

	delegate: TrainItemDelegate {
		id:delegate
		itemName: model.name
		itemReps: model.repetitions
		itemChecked: model.checked

		width: parent.width

		Binding {
			target: model
			property: "checked"
			value: delegate.itemChecked
		}
	}

	//private
	TrainItemDelegate {
		id: heightHelper
		visible: false
		enabled: false
	}
}
