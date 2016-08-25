import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Universal 2.0

ToolButton {
	id: toolButton
	implicitHeight: 56
	implicitWidth: 56
	hoverEnabled: true

	property alias imageSource: image.source
	contentItem: Item {
		id: imageWrapper
		implicitHeight: 56
		implicitWidth: 56

		Image {
			id: image
			anchors.centerIn: parent
			fillMode: Image.PreserveAspectFit
			horizontalAlignment: Image.AlignHCenter
			verticalAlignment: Image.AlignVCenter
			width: 36
			height: 36
		}
	}

	ToolTip.visible: toolButton.hovered
	ToolTip.text: toolButton.text
	ToolTip.delay: 500
	onPressed: ToolTip.toolTip.close()
}
