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
//						text: reminderControl.gifTag

//						Binding {
//							target: reminderControl
//							property: "gifTag"
//							value: searchTagField.text
//						}
					}

					ListView {
						id: reminderList
						//implicitHeight: heightHelper.height * model.rowCount;
						implicitHeight: 42
						Layout.minimumHeight: implicitHeight
						Layout.maximumHeight: implicitHeight
						Layout.fillWidth: true
						Layout.columnSpan: 2

						delegate: ItemDelegate {
//							text: qsTr("Remind%1 at: %2")
//								  .arg(model.intense ? qsTr(" (intense)") : "")
//								  .arg(model.time/*.toLocaleTimeString()*/)

//							highlighted: model.intense
//							down: false
//							width: parent.width
						}

						//private
						ItemDelegate {
							id: heightHelper
							visible: false
							enabled: false
							text: "test"
						}
					}

					CheckBox {
						id: permanentBox
						Layout.columnSpan: 2
						Layout.alignment: Qt.AlignLeft

						text: qsTr("Always show status icon")
//						checked: reminderControl.permanent

//						Binding {
//							target: reminderControl
//							property: "permanent"
//							value: permanentBox.checked
//						}
					}
				}
			}
		}
	}
}
