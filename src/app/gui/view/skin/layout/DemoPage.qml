import QtQuick 1.0
import com.nokia.symbian 1.0

Page {
	id: root
	
	tools: ToolBarLayout {
		ToolButton {
			flat: true
            iconSource: "toolbar-back"
			onClicked: Qt.quit()
		}
	}
}
