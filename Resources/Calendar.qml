import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import Qt.labs.calendar 1.0

ListView {
	id: calenderList

	property date firstDate: new Date(1970, 0, 1)
	property date lastDate: new Date(9999, 11, 31)
	property date currentDate: calenderList.today();

	function today() {
		var cDate = new Date();
		cDate.setHours(0, 0, 0, 0, 0);
		return cDate;
	}

	QtObject {
		id: p

		property bool skipNextFocus: false

		function focusDate() {
			if(skipNextFocus)
				skipNextFocus = false;
			else {
				calenderList.positionViewAtIndex(calendarModel.indexOf(calenderList.currentDate),
												 ListView.SnapPosition);
			}
		}
	}

	Component.onCompleted: p.focusDate()
	onCurrentDateChanged: p.focusDate()

	implicitWidth: 300
	implicitHeight: 200

	snapMode: ListView.SnapOneItem
	orientation: ListView.Horizontal
	highlightRangeMode: ListView.StrictlyEnforceRange
	clip: true

	model: CalendarModel {
		id: calendarModel
		from: calenderList.firstDate
		to: calenderList.lastDate
	}

	delegate: GridLayout {
		width: calenderList.width
		height: calenderList.height
		columns: 4
		rows: 4

		Label {
			Layout.row: 0
			Layout.column: 1
			Layout.columnSpan: 2
			Layout.fillWidth: true
			horizontalAlignment: Text.AlignHCenter
			font.bold: true
			text: grid.title
		}

		ToolButton {
			Layout.row: 1
			Layout.column: 1
			Layout.fillHeight: true
			Layout.fillWidth: true
			Layout.preferredHeight: 36
			Layout.preferredWidth: 36
			flat: true

			contentItem: Item {
				Image {
					anchors.centerIn: parent
					width: 24
					height: 24
					source: "qrc:/icons/today.png"
				}
			}

			onClicked: calenderList.currentDate = calenderList.today()
		}

		DayOfWeekRow {
			Layout.row: 1
			Layout.column: 2
			Layout.fillWidth: true
		}

		ToolButton {
			Layout.row: 0
			Layout.column: 0
			Layout.rowSpan: 3
			Layout.fillHeight: true
			Layout.preferredHeight: 36
			Layout.preferredWidth: 36
			flat: true

			contentItem: Item {
				Image {
					anchors.centerIn: parent
					width: 24
					height: 24
					source: "qrc:/icons/navigateLeft.png"
				}
			}

			onClicked: calenderList.decrementCurrentIndex()
		}

		WeekNumberColumn {
			Layout.row: 2
			Layout.column: 1
			month: model.month
			year: model.year
			Layout.fillHeight: true
		}

		MonthGrid {
			id: grid
			Layout.row: 2
			Layout.column: 2
			month: model.month
			year: model.year
			Layout.fillWidth: true
			Layout.fillHeight: true

			delegate: Label {
				id: dayDelegate

				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				opacity: model.month === grid.month ? 1 : 0.5
				text: model.day
				font: grid.font

				background: Rectangle {
					readonly property double size: Math.max(dayDelegate.width, dayDelegate.height) * 1.2

					anchors.centerIn: parent
					height: size
					width: size

					color: globalStyleAccess.accentColor
					opacity: model.date.getTime() === calenderList.currentDate.getTime() ? 0.5 : 0
					radius: size / 2

					Behavior on opacity {
						NumberAnimation {
							duration: 750
							easing.type: Easing.OutBack
						}
					}
				}

				MouseArea {
					anchors.fill: parent

					onClicked: {
						if(model.month < grid.month) {
							calenderList.decrementCurrentIndex();
							p.skipNextFocus = true;
						} else if(model.month > grid.month) {
							calenderList.incrementCurrentIndex();
							p.skipNextFocus = true;
						}
						calenderList.currentDate = model.date;
					}
				}
			}
		}

		ToolButton {
			Layout.row: 0
			Layout.column: 3
			Layout.rowSpan: 3
			Layout.fillHeight: true
			Layout.preferredHeight: 36
			Layout.preferredWidth: 36
			flat: true

			contentItem: Item {
				Image {
					anchors.centerIn: parent
					width: 24
					height: 24
					source: "qrc:/icons/navigateRight.png"
				}
			}

			onClicked: calenderList.incrementCurrentIndex()
		}

		Item {
			Layout.row: 3
			Layout.column: 0
			Layout.columnSpan: 4
			height: 7
		}
	}

	ScrollIndicator.horizontal: ScrollIndicator {}
}
