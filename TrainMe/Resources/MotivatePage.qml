import QtQuick 2.7
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

ControlPage {
	id: motivatePage
	control: motivateControl

	FastBlur {
		anchors.centerIn: parent
		source: motivateImage
		radius: 42

		property size realSize: scaled(motivateImage.sourceSize, Qt.size(parent.width, parent.height))
		width: realSize.width
		height: realSize.height

		function scaled(baseSize, bigSize) {
			var wFactor = bigSize.width / baseSize.width;
			var hFactor = bigSize.height / baseSize.height;
			var factor = Math.max(wFactor, hFactor);
			return Qt.size(baseSize.width * factor, baseSize.height * factor);
		}
	}

	ProgressBar {
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top

		value: motivateImage.progress
	}

	AnimatedImage {
		id: motivateImage
		anchors.centerIn: parent
	}

	Button {
		id: reloadButton

		anchors.horizontalCenter: parent.horizontalCenter
		anchors.top: motivateImage.bottom
		anchors.topMargin: 14

		text: qsTr("Motivate me more!")
		highlighted: true

		onClicked: motivateControl.reload()
	}

	Connections {
		target: motivateControl
		onImageLoaded: motivateImage.source = gifUrl
	}
}
