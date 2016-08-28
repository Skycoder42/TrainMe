import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import "../controls"

ControlPage {
    id: configWeekPage
    control: configWeekControl

	RowLayout {
		id: rowLayout
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top
		height: rowLayout.implicitHeight + 14
		spacing: 14

		Label {
			id: nameLabel
			Layout.fillWidth: true
			Layout.alignment: Qt.AlignVCenter
			Layout.preferredWidth: 200
			//Layout.minimumWidth: nameLabel.implicitWidth
			text: qsTr("<i>Weekday</i>")
			leftPadding: 14
			horizontalAlignment: Qt.AlignLeft
		}

		Label {
			id: increaseLabel
			Layout.fillWidth: true
			Layout.alignment: Qt.AlignVCenter
			Layout.preferredWidth: 200
			//Layout.minimumWidth: increaseLabel.implicitWidth
			text: qsTr("<i>Increase</i>")
			horizontalAlignment: Qt.AlignHCenter
		}

		Label {
			id: addLabel
			Layout.fillWidth: true
			Layout.alignment: Qt.AlignVCenter
			Layout.preferredWidth: 200
			//Layout.minimumWidth: addLabel.implicitWidth
			text: qsTr("<i>Add Task</i>")
			horizontalAlignment: Qt.AlignHCenter
		}
	}

	Rectangle {
		id: headerLine
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: rowLayout.bottom
		height: 1
		color: globalStyleAccess.accentColor
	}

    ListView {
        id: weekConfigList

		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: headerLine.bottom
		anchors.bottom: parent.bottom
		clip: true

        ScrollBar.vertical: ScrollBar {}

		model: configWeekControl.configModel

        delegate: WeekConfigDelegate {
			id: delegate
            itemName: model.weekday
            itemIncrement: model.increment
            itemAddTask: model.addTask
            width: parent.width

			Binding {
				target: model
				property: "increment"
				value: delegate.itemIncrement
			}
			Binding {
				target: model
				property: "addTask"
				value: delegate.itemAddTask
			}
        }

		footer: ColumnLayout {
			spacing: 0
			width: parent.width
			height: implicitHeight

			Rectangle {
				height: 1
				color: globalStyleAccess.accentColor
				Layout.fillWidth: true
			}

			GroupBox {
				id: scoreGroup
				title: qsTr("Train Settings")
				Layout.fillWidth: true
				Layout.margins: 14

				GridLayout {
					rowSpacing: 14
					columnSpacing: 14
                    columns: root.isSmall ? 2 : 3

					Label {
						text: qsTr("Penalty Factor:")
					}

					SpinBox {
						id: penaltyBox
						from: 1
						to: 1000
						stepSize: 5
						editable: true
						Layout.fillWidth: true
						value: configWeekControl.penaltyFactor * 100

						validator: DoubleValidator {
							bottom: 0.01
							top: 10.0
						}

						textFromValue: function(value, locale) {
							return Number(value / 100).toLocaleString(locale, 'f', 2);
						}

						valueFromText: function(text, locale) {
							return Number.fromLocaleString(locale, text) * 100;
						}

						Binding {
							target: configWeekControl
							property: "penaltyFactor"
							value: penaltyBox.value / 100.0
						}
					}

                    Item {
                        Layout.fillWidth: true
                        visible: root.isSmall
                    }

                    CheckBox {
                        id: agilityPenaltyBox
                        Layout.fillWidth: true
                        text: qsTr("Agility Penalties")
						checked: configWeekControl.agilityPenalties

						Binding {
							target: configWeekControl
							property: "agilityPenalties"
							value: agilityPenaltyBox.checked
						}
                    }

					Label {
						text: qsTr("Free Days:")
					}

					SpinBox {
						id: maxFreeBox
						from: 0
						to: 365
						editable: true
						Layout.fillWidth: true
						value: configWeekControl.maxFreeDays

						Binding {
							target: configWeekControl
							property: "maxFreeDays"
							value: maxFreeBox.value
						}
                    }

                    Item {
                        Layout.fillWidth: true
                        visible: !root.isSmall
                    }
				}
			}
		}
	}
}
