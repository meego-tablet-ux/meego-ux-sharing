/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Rectangle {
  id: customArea

  anchors.fill: parent
  color: "grey"
  Text {
    id: infoText
    height: 60
    width: 200
    //sharingObj is the interface to use in your custom QML to get access to the
    //sharing functionality and the list of files to share.
    //It is an implementation of the MeeGoUXSharingClientQmlObj class, and you can
    //access all of the Q_PROPERTY properties and Q_INVOKABLE member methods
    //exposed by the class. For further details on the class, please see the
    //meegouxsharingclientqmlobj.h file, included in the libmeegouxsharingqml-devel package.
    text: qsTr("Share Type: %1\nFile Count: %2").arg(sharingObj.shareTypeName, sharingObj.filesToShare.length)
  }
  ListView {
    id: fileList
    width: 200
    height: 300
    anchors.top: infoText.bottom
    anchors.topMargin: 5
    model: sharingObj.filesToShare
    delegate: Rectangle {
      width: parent.width
      height: 20
      Text {
          anchors.fill: parent
	  text: modelData
      }
      MouseArea {
        anchors.fill: parent
        onClicked: {
	  //If you need to have multiple custom UI steps in your flow, just set customLoader.source to the next QML file you want to load, when you're ready to load it.
          customLoader.source = "/usr/lib/qt4/imports/MeeGo/Sharing/plugins/example/foo_image_multi_step2.qml"
        }
      }
    }
  }
}
