import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import "../controls"
import "../MessageBox"

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

								onDeleteClicked: reminderControl.removeReminder(index)
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

							onClicked: createReminderBox.show()
						}
					}
				}
			}
		}
	}

	MessageBox {
		id: createReminderBox

		title: qsTr("Create Reminder")
		positiveButtonText: qsTr("Create")
		negativeButtonText: qsTr("Cancel")

		function show() {
			messageContentItem.init();
			createReminderBox.open();
		}

		messageContent: ColumnLayout {
			spacing: 14

			readonly property date time: timeSpinner.numToDate(timeSpinner.value)
			readonly property alias intense: intenseBox.checked

			function init() {
				timeSpinner.value = timeSpinner.dateToNum(new Date());
			}

			SpinBox {
				id: timeSpinner
				Layout.fillWidth: true

				stepSize: 1
				from: 0
				to: 1439
				value: 0
				editable: true

				textFromValue: function(value, locale) {
					return numToDate(value).toLocaleTimeString(Locale.ShortFormat);
				}

				valueFromText: function(text, locale) {
					var date = Date.fromLocaleTimeString(Qt.locale(), text, Locale.ShortFormat);
					return dateToNum(date);
				}

				validator: RegExpValidator {
					regExp: /\d{2}:\d{2}/
				}

				function numToDate(number) {
					var date = new Date(2000, 1);
					date.setHours(Math.floor(number / 60));
					date.setMinutes(number % 60);
					return date;
				}

				function dateToNum(date) {
					return date.getHours() * 60 + date.getMinutes();
				}
			}

			Switch {
				id: intenseBox
				text: qsTr("Intense Notification")
			}
		}

		onPositiveAction: {
			reminderControl.createReminder(messageContentItem.time,
										   messageContentItem.intense);
		}
	}
}
