/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Sharing 0.1

Rectangle {
  id: customArea

  //The final QML file in your custom UI flow should provide the following
  //signals for the standard sharing UI to listen for

  signal cancel()
  signal shared(int shareid)
  signal shareError(string errMsg)

  anchors.fill: parent
  color: "grey"

  Text {
    id: infoText
    height: 60
    width: 200
    text: qsTr("Single sharing file - Are you sure?")
  }
  Text {
    id: btnYes
    height: 20
    width: 75
    anchors.top: infoText.bottom
    anchors.topMargin: 10
    anchors.left: infoText.left
    anchors.leftMargin: 10
    text: qsTr("Yes")
    MouseArea {
      anchors.fill: parent
      onClicked: {
        //trigger actual share action here...
	var x = 0, fileID = 0;

	//Demonstrates the ability to remove files from within the service-specific QML
	while (sharingObj.filesToShare.length > 1) {
	  sharingObj.removeFile(sharingObj.filesToShare(1));
	}


	//Iterate through all the files currently added to sharingObj
        for (x in sharingObj.filesToShare) {
            //Set custom properties on a per-file basis
            sharingObj.addHashEntryToFile(sharingObj.filesToShare[x], "title", "Picture #" + x);
            sharingObj.addHashEntryToFile(sharingObj.filesToShare[x], "x-my-param", "my-val");
	}
	var errMsg = "";
	//Actually perform the sharing operation
        var opID = sharingObj.shareSimple();
	if (opID == -1) {
          errMsg = sharingObj.getLastShareError();
	  customArea.shareError(errMsg);
	} else {
	  customArea.shared(opID); //Use opID from sharing client
	}
      }
    }
  }
  Text {
    id: btnNo
    height: 20
    width: 75
    anchors.top: infoText.bottom
    anchors.topMargin: 10
    anchors.right: infoText.right
    anchors.rightMargin: 10
    text: qsTr("No")
    MouseArea {
      anchors.fill: parent
      onClicked: {
        customArea.cancel()
      }
    }
  }
}
