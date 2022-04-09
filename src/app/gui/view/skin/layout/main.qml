import QtQuick 1.0
import com.nokia.symbian 1.0

ApplicationWindow {
	id: root
	Component { id: demoPage; DemoPage {} }
	Component.onCompleted: pageStack.push(demoPage)
}
