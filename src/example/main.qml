import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Window 2.0

Window {
    visible: true
    width: 300
    height: 200
    title: contextTitle

    readonly property string contextTitle: mainViewContext.title

    Action {
        id: doAsyncAction

        onTriggered: mainViewContext.doAsync()
    }

    Action {
        id: doLaterAction

        onTriggered: mainViewContext.doLater()
    }

    Label {
        text: contextTitle
        anchors.centerIn: parent
    }

    MouseArea {
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
            right: parent.horizontalCenter
        }

        onClicked: {
            doAsyncAction.trigger()
        }
    }

    MouseArea {
        anchors {
            top: parent.top
            left: parent.horizontalCenter
            bottom: parent.bottom
            right: parent.right
        }

        onClicked: {
            doLaterAction.trigger()
        }
    }
}
