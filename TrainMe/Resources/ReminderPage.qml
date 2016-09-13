import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import "../controls"

ControlPage {
	id: reminderPage
	control: reminderControl

	Flickable {
		id: flickable
		anchors.fill: parent

		contentHeight: mainContent.height
		enabled: reminderControl.supportsReminders

		ScrollBar.vertical: ScrollBar {}

		ColumnLayout {
			id: mainContent
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.top: parent.top
			spacing: 0

			Switch {
				id: activeBox
				Layout.margins: 14

				text: qsTr("Reminders Active")
				checked: reminderControl.remindersActive

				Binding {
					target: reminderControl
					property: "remindersActive"
					value: activeBox.checked
				}
			}

			Rectangle {
				id: seperator
				Layout.fillWidth: true
				height: 1
				color: globalStyleAccess.primaryColor
			}

			GridLayout {
				enabled: activeBox.checked
				columns: 2
				rows: 3
				rowSpacing: 14
				columnSpacing: 14
				Layout.margins: 14

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

				GroupBox {
					title: qsTr("Active Reminders")

					Layout.minimumHeight: implicitHeight
					Layout.maximumHeight: implicitHeight
					Layout.fillWidth: true
					Layout.columnSpan: 2

					ColumnLayout {
						anchors.fill: parent

						ListView {
							id: reminderList
							implicitHeight: heightHelper.height * reminderList.count;
							Layout.fillWidth: true

							model: reminderControl.reminderModel

							delegate: ReminderDelegate {
								id: delegate
								time: model.time
								intense: model.intense
								width: parent.width
							}

							//private
							ReminderDelegate {
								id: heightHelper
								visible: false
								enabled: false
							}
						}

						Button {
							text: qsTr("Add Reminder")
							Layout.alignment: Qt.AlignRight
							highlighted: true
						}
					}
				}
			}
		}
	}
}
