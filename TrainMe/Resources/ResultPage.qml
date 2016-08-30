import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Universal 2.0
import "../controls"
import "../MessageBox"

ControlPage {
	id: resultPage

	control: resultControl

	ListView {
		id: trainList

		anchors.fill: parent
		ScrollBar.vertical: ScrollBar {}

		model: resultControl.resultList

		delegate: ItemDelegate {
			id: itemDelegate
			width: parent.width
			text: modelData

			highlighted: editBox.currentIndex == index
			down: pressed && !highlighted && resultControl.isEditable(index)

			onClicked: {
				if(resultControl.isEditable(index))
					editBox.showBox(index);
			}
		}

		onModelChanged: rePosTimer.restart()

		Timer {
			id: rePosTimer
			running: false
			repeat: false
			interval: 100

			onTriggered: {
				var index = resultControl.firstDirtyIndex();
				if(index === -1)
					trainList.positionViewAtEnd();
				else
					trainList.positionViewAtIndex(index, ListView.Center);
			}
		}
	}

	Connections {
		target: resultControl
		onCreateNewEntry: editBox.showBoxCreate();
	}

	MessageBox {
		id: editBox

		title: qsTr("Set a result")
		positiveButtonText: qsTr("Save")
		negativeButtonText: qsTr("Cancel")

		property int currentIndex: -1
		property int comboValue: -1
		property bool isNewDate: false
		property date currentDate

		function showBox(modelIndex) {
			editBox.currentIndex = modelIndex;
			editBox.messageContentItem.currentIndex = 1;
			editBox.isNewDate = false;
			editBox.open();
		}

		function showBoxCreate() {
			editBox.currentIndex = -1;
			editBox.messageContentItem.currentIndex = 1;
			editBox.messageContentItem.resetDate();
			editBox.isNewDate = true;
			editBox.open();
		}

		onPositiveAction: {
			if(editBox.isNewDate)
				resultControl.createResult(editBox.currentDate, editBox.comboValue);
			else
				resultControl.updateResult(editBox.currentIndex, editBox.comboValue);
		}

		onClosed: editBox.currentIndex = -1

		messageContent: ColumnLayout {
			spacing: 14

			property alias currentIndex: comboBox.currentIndex

			function resetDate() {
				cal.currentDate = cal.today();
			}

			Calendar {
				id: cal
				visible: editBox.isNewDate
				Layout.fillWidth: true

				onCurrentDateChanged: editBox.currentDate = cal.currentDate
			}

			ComboBox {
				id: comboBox
				textRole: "key"
				Layout.fillWidth: true

				model: ListModel {
					id: resultModel
				}

				onCurrentIndexChanged: editBox.comboValue = model.get(comboBox.currentIndex).value

				Component.onCompleted: {
					for(var i = 0; i < 6; i++)
						resultModel.append({"key": app.trainManager.trResult(i), "value": i});

					if(app.testStyle("Universal")) {
						popup.y = Qt.binding(function() {
							return -1 * comboBox.currentIndex * referenceDelegate.implicitHeight
						});
					}
				}

				//reference
				ItemDelegate {
					id:referenceDelegate
					visible: false
					text: "placeholder"
				}
			}
		}
	}
}
