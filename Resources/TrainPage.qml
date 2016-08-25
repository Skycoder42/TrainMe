import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import "../controls"
import "../MessageBox"

ControlPage {
	id:trainPage
	control: trainControl

	Flickable {
		id: flickable
		anchors.fill: parent

		contentHeight: mainContent.height

		ScrollBar.vertical: ScrollBar {}

		Pane {
			id: mainContent
			width: flickable.width

			GridLayout {
				id: layout

				anchors.left: parent.left
				anchors.right: parent.right

				columnSpacing: 14
				rowSpacing: 14
				columns: root.isSmall ? 1 : 2

				GroupBox {
					Layout.fillWidth: true
					Layout.preferredHeight: implicitHeight
					Layout.fillHeight: true
					implicitWidth: 200

					title: qsTr("Strengh-Training")

					TrainListView {
						id: strengthList
						anchors.fill: parent
						model: trainControl.strengthModel
					}
				}

				GroupBox {
					Layout.fillWidth: true
					Layout.preferredHeight: implicitHeight
					Layout.fillHeight: true
					implicitWidth: 200

					title: qsTr("Agility-Training")

					TrainListView {
						id: agilityList
						anchors.fill: parent
						model: trainControl.agilityModel
					}
				}

				Button {
					id: doneButton
					Layout.preferredWidth: implicitWidth * 1.2
					anchors.horizontalCenter: parent.horizontalCenter

					enabled: trainControl.trainingAllowed
					text: trainControl.allDone ? qsTr("Training Done!") : qsTr("Training Failed!")
					highlighted: trainControl.allDone

					onClicked: {
						if(trainControl.allDone)
							trainControl.completeTraining();
						else
							failQuestion.open();
					}
				}
			}
		}
	}

	QuestionBox {
		id: failQuestion

		title: qsTr("Training failed?")
		message: qsTr("Did you really fail the training?!?Did you really fail the training?!?Did you really fail the training?!?Did you really fail the training?!?Did you really fail the training?!?Did you really fail the training?!?Did you really fail the training?!?Did you really fail the training?!?Did you really fail the training?!?Did you really fail the training?!?Did you really fail the training?!?Did you really fail the training?!?Did you really fail the training?!?Did you really fail the training?!?Did you really fail the training?!?Did you really fail the training?!?Did you really fail the training?!?Did you really fail the training?!?")
		positiveButtonText: qsTr("I failed!")

		onQuestionAnswered: {
			if(accepted)
				trainControl.completeTraining();
		}
	}
}
