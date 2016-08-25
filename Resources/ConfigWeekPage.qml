import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import "../controls"

ControlPage {
    id: configWeekPage

    ListView {
        id: weekConfigList

        anchors.fill: parent
        ScrollBar.vertical: ScrollBar {}

        model: ListModel {
            ListElement {
                weekday: "Monday"
                increment: 0
                addTask: false
            }
            ListElement {
                weekday: "Tuesday"
                increment: 3
                addTask: false
            }
            ListElement {
                weekday: "Sunday"
                increment: 0
                addTask: true
            }
        }

        header: Label {text: "#00FF00";}

        delegate: WeekConfigDelegate {
            itemName: model.weekday
            itemIncrement: model.increment
            itemAddTask: model.addTask
            width: parent.width
        }
    }
}
