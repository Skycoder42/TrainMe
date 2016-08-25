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

			hoverEnabled: true
			highlighted: (pressed && resultControl.isEditable(index)) || editBox.currentIndex == index
			down: (hovered || pressed) && !highlighted

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

	MessageBox {
		id: editBox

		title: qsTr("Set a result")
		messageContent: contentComponent
		positiveButtonText: qsTr("Save")
		negativeButtonText: qsTr("Cancel")

		property int currentIndex: -1
		property int comboValue: -1

		function showBox(modelIndex) {
			editBox.currentIndex = modelIndex;
			editBox.messageContentItem.currentIndex = 1;
			editBox.open();
		}

		Component {
			id: contentComponent

			ComboBox {
				id: comboBox
				textRole: "key"

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

		onPositiveAction: resultControl.updateResult(editBox.currentIndex, editBox.comboValue)

		onClosed: editBox.currentIndex = -1
	}
}
