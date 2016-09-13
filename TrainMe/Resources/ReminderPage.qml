import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ControlPage {
	id: reminderPage
	control: reminderControl

	Flickable {
		id: flickable
		anchors.fill: parent

		contentHeight: mainContent.height
		enabled: reminderControl.supportsReminders

		ScrollBar.vertical: ScrollBar {}

		Pane {
			id: mainContent
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.top: parent.top
			height: contentGroup.height

			GroupBox {
				id: contentGroup
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.top: parent.top

				label: Switch {
					id: activeBox
					text: qsTr("Reminders Active")
					checked: reminderControl.remindersActive

					Binding {
						target: reminderControl
						property: "remindersActive"
						value: activeBox.checked
					}
				}

				GridLayout {
					anchors.fill: parent
					enabled: activeBox.checked
					columns: 2
					rows: 3
					rowSpacing: 14
					columnSpacing: 14

					Label {
						text: qsTr("Gif search tag:")
					}

					TextField {
						id: searchTagField
						Layout.fillWidth: true

						placeholderText: "motivation"
						text: reminderControl.gifTag
						selectByMouse: true

						onEditingFinished: reminderControl.gifTag = searchTagField.text
					}

					ListView {
						id: reminderList
						implicitHeight: heightHelper.height * reminderList.count;
						Layout.minimumHeight: implicitHeight
						Layout.maximumHeight: implicitHeight
						Layout.fillWidth: true
						Layout.columnSpan: 2

						model: reminderControl.reminders

						delegate: ItemDelegate {
							text: qsTr("Remind%1 at: %2")
								  .arg(modelData.intense ? qsTr(" (intense)") : "")
								  .arg(modelData.time/*.toLocaleTimeString()*/)

							highlighted: modelData.intense
							down: false
							width: parent.width

							Component.onCompleted: {
								var data = modelData;
								console.log(modelData, modelData.property("time"), modelData.intense);
							}
						}

						//private
						ItemDelegate {
							id: heightHelper
							visible: false
							enabled: false
						}
					}

					CheckBox {
						id: permanentBox
						Layout.columnSpan: 2
						Layout.alignment: Qt.AlignLeft

						text: qsTr("Always show status icon")
						checked: reminderControl.permanent

						Binding {
							target: reminderControl
							property: "permanent"
							value: permanentBox.checked
						}
					}
				}
			}
		}
	}
}
