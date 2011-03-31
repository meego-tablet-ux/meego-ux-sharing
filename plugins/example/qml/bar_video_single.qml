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
  color: "BLACK"
  Text {
    height: 200
    width: 200
    text: qsTr("BAR VIDEO SINGLE!!!")
    color: "yellow"
  }
}
