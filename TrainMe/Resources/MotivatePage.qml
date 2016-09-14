import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import "../MessageBox"

ControlPage {
	id: motivatePage
	control: motivateControl

	FastBlur {
		anchors.centerIn: parent
		source: motivateImage
		radius: 42

		property size realSize: scaled(Qt.size(motivateImage.paintedWidth, motivateImage.paintedHeight),
									   Qt.size(parent.width, parent.height),
									   Qt.size(motivateImage.width, motivateImage.height))
		width: realSize.width + 28
		height: realSize.height + 28

		function scaled(contentSize, bigSize, itemSize) {
			var wFactor = bigSize.width / contentSize.width;
			var hFactor = bigSize.height / contentSize.height;
			var factor = Math.max(wFactor, hFactor);
			return Qt.size(itemSize.width * factor, itemSize.height * factor);
		}
	}

	ProgressBar {
		id: progressBar
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top

		value: motivateImage.progress
	}

	AnimatedImage {
		id: motivateImage
		anchors.centerIn: parent
		fillMode: Image.PreserveAspectFit

		width: Math.min(parent.width - 28, sourceSize.width)
		height: Math.min(parent.height - (reloadButton.height + 28) * 2, sourceSize.height)
	}

	Button {
		id: reloadButton
		anchors.top: motivateImage.bottom
		anchors.topMargin: 14 - (motivateImage.height - motivateImage.paintedHeight) / 2
		anchors.horizontalCenter: motivateImage.horizontalCenter

		text: qsTr("Motivate me more!")
		highlighted: true

		onClicked: motivateControl.reload()
	}

	Connections {
		target: motivateControl
		onImageLoaded: motivateImage.source = gifUrl
		onChangeTag: tagBox.show();
	}

	MessageBox {
		id: tagBox

		title: qsTr("Enter the gif tag")
		positiveButtonText: qsTr("Apply")
		negativeButtonText: qsTr("Cancel")

		function show() {
			tagBox.messageContentItem.text = motivateControl.gifTag;
			tagBox.open();
		}

		messageContent: TextField {
			id: contentField

			placeholderText: "motivation"
		}

		onPositiveAction: {
			motivateControl.gifTag = tagBox.messageContentItem.text;
			motivateControl.reload();
		}
	}
}
