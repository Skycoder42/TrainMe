import QtQuick 2.7
import QtQuick.Controls 2.0

Page {
	property var control

    property Menu moreMenu: Menu {}

    Component.onCompleted: {
        root.header.moreMenu = moreMenu;
        control.initialize();
    }
}
