import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Universal 2.0

AppBarButton {
    id: moreButton
    imageSource: "qrc:/icons/more.png"
    text: qsTr("More…")
    checkable: true
    checked: p.menuVisible
    visible: menu.contentModel.count > 0

    Universal.accent: "#B8B8B8"

    property Menu menu: Menu {}

    QtObject {
        id: p
        property bool menuVisible: false

        function menuShown() {
            p.menuVisible = true;
        }

        function menuHidden() {
            p.menuVisible = false;
            moreButton.enabled = false;
            clickBlockTimer.restart();
        }
    }

    Timer {
        id: clickBlockTimer
        interval: 100
        repeat: false
        running: false

        onTriggered: moreButton.enabled = true
    }

    onMenuChanged: {
        moreButton.menu.parent = moreButton;
        moreButton.menu.x = 0;
        if(app.testStyle("Universal"))
            moreButton.menu.y = Qt.binding(function(){return moreButton.height});
        else
            moreButton.menu.y = 0;
        moreButton.menu.opened.connect(p.menuShown);
        moreButton.menu.closed.connect(p.menuHidden);
    }

    onClicked: p.menuVisible ? moreButton.menu.close() : moreButton.menu.open()
}
