import QtQuick 2.7
import QtQuick.Controls 2.0

ControlPage {
	id: motivatePage

	ProgressBar {
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top

		value: sammy.progress
	}

	Image {
		id: sammy
		anchors.centerIn: parent
		source: "https://s-media-cache-ak0.pinimg.com/236x/a1/70/38/a170380693fbd12787728de6718a5ed9.jpg"
	}
}
