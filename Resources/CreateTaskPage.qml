import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import "../controls"

ControlPage {
	id: createTaskPage
	control: createTaskControl

	Pane {
		anchors.fill: parent

		ColumnLayout {
			anchors.fill: parent
			spacing: 14

			Label {
				text: qsTr("Tasks to create: <b>%1</b>").arg(createTaskControl.freeTasks)
				font.pointSize: 16
				Layout.fillWidth: true
			}

			GroupBox {
				title: qsTr("Create a Task")
				Layout.fillWidth: true

				StackLayout {
					anchors.fill: parent
					currentIndex: createTaskControl.freeTasks > 0 ? 1 : 0

					Label {
						text: qsTr("<i>You can't create more tasks</i>")
						horizontalAlignment: Text.AlignHCenter
						verticalAlignment: Text.AlignVCenter
					}

					GridLayout {
						id: strengthMask
						columns: 2
						columnSpacing: 14
						rowSpacing: 14

						Label {
							text: qsTr("Task Name:")
						}

						TextField {
							id: nameField
							Layout.fillWidth: true
							placeholderText: qsTr("Enter the tasks display name")
							selectByMouse: true
						}

						Label {
							text: createTaskControl.nextIsAgility ? qsTr("Count:") : qsTr("Base Count:")
						}

						SpinBox {
							id: baseCountBox
							Layout.fillWidth: true
							from: 0
							value: 10
							editable: true
						}

						Label {
							visible: !createTaskControl.nextIsAgility
							text: qsTr("Increment Factor:")
						}

						DoubleSpinBox {
							id: factorBox
							visible: !createTaskControl.nextIsAgility
							dFrom: 0
							dTo: 100
							dStepSize: 1
							dValue: 1
							editable: true
							Layout.fillWidth: true
						}

						RowLayout {
							Layout.fillWidth: true
							Layout.columnSpan: 2

							Item {Layout.fillWidth: true}

							Button {
								Layout.alignment: Qt.AlignRight
								Layout.preferredWidth: implicitWidth * 1.2
								highlighted: true
								text: createTaskControl.nextIsAgility ? qsTr("Create <u>Agility</u> Task") : qsTr("Create <u>Strength</u> Task")
								enabled: nameField.text.length > 0

								onClicked: createTaskControl.createTask(nameField.text, baseCountBox.value, factorBox.value / 100)
							}
						}
					}
				}
			}

			Item {
				Layout.fillHeight: true
			}
		}
	}
}
