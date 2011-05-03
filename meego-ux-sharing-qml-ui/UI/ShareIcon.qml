/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Sharing 0.1
import MeeGo.Components 0.1

ShareObj {
    width: shareImg.width
    height: shareImg.height

    id: shareContainer

    signal shareAllClicked();

    property bool shareAll: false

    function doShareAll() {
        if (shareCount == 0)
            return;
        var pos = topItem.topItem.mapFromItem(shareImg, shareImg.x+shareImg.width, shareImg.y);
        showContextTypes(pos.x, pos.y);
    }

    TopItem {
        id: topItem
    }

    IconButton {
        id: shareImg
        icon: "image://theme/media/icn_share_up"
        iconDown: "image://theme/media/icn_share_dn"

        anchors.top: parent.top
        anchors.left: parent.left

        enabled: {
            if ((shareCount != 0) || shareAll)
                return true;
            return false;
        }

        onClicked: {
            if (shareAll && shareCount == 0) {
                shareContainer.shareAllClicked();
            } else {
                var pos = topItem.topItem.mapFromItem(shareImg, shareImg.x+shareImg.width, shareImg.y);
                parent.showContextTypes(pos.x, pos.y);
            }
        }
    }
}
