import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ItemDelegate {
    id: delegate

    property alias itemName: nameLabel.text
    property alias itemIncrement : incrementBox.value
    property alias itemAddTask: addTaskBox.checked

    contentItem: RowLayout {
        spacing: 14

        Label {
            id: nameLabel
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.preferredWidth: 0
        }

        Item {
            id: spinWrapper
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredWidth: 0
            implicitHeight: incrementBox.implicitHeight

            SpinBox {
                id: incrementBox
                anchors.centerIn: parent
                editable: true
                to: 42
            }
        }

        Item {
            id: checkWrapper
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredWidth: 0
            implicitHeight: addTaskBox.implicitHeight

            CheckBox {
                id: addTaskBox
                anchors.centerIn: parent
            }
        }
    }
}
