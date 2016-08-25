import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0

Control {
	readonly property color accentColor: {
		if(app.testStyle("Material"))
			return Material.accent;
		else if(app.testStyle("Universal"))
			return Universal.accent;
		else
			return "transparent"
	}
}
