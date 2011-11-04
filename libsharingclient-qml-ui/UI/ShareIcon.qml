/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import Sharing 0.1
import MeeGo.Components 0.1

ShareObj {
    width: shareImg.width
    height: shareImg.height

    id: shareContainer

    snrPrefix: "ShareIcon"

    signal shareAllClicked();

    property bool shareAll: false
    property int mouseX
    property int mouseY

    function doShareAll() {
        if (shareCount == 0)
            return;
        showContextTypes(0, 0);
    }

    TopItem {
        id: topItem
    }

    SaveRestoreState {
        id: mySnRIcn
        onSaveRequired: {
            setValue(snrPrefix + "_shareAll", (shareAll ? 1 : 0));
        }
        Component.onCompleted: {
            if (restoreRequired) {
                var val;
                val = Math.floor(value(snrPrefix + "_shareAll", 0));
                remove(snrPrefix + "_shareAll");
                if (val)
                    shareAll = true;
                else
                    shareAll = false;
            }
        }
    }

    IconButton {
        id: shareImg
        icon: "image://themedimage/images/media/icn_share_up"
        iconDown: "image://themedimage/images/media/icn_share_dn"

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
                parent.showContextTypes(0, 0);
            }
        }
    }
}
