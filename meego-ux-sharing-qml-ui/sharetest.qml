import Qt 4.7
import MeeGo.Components 0.1
import MeeGo.Sharing 0.1
import MeeGo.Sharing.UI 0.1
import MeeGo.Labs.Components 0.1 as Labs

Labs.Window {
//    width: 640
//    height: 480
    //color: "grey"
    id: scene

    Item {
        parent: scene.content


        TopItem {
            id: topItem
        }

        Button {
            id: videoBtn
            width: 100
            height: 50
            anchors.top: parent.top
            anchors.topMargin: 200
            anchors.left: parent.left
            text: "Video"
            onClicked: {
                sharing.shareType = MeegoSharingClientQmlObj.ShareTypeVideo;
                shareObj.shareType = MeegoSharingClientQmlObj.ShareTypeVideo;
            }
        }

        Button {
            id: imageBtn
            width: 100
            height: 50
            anchors.top: videoBtn.top
            anchors.left: videoBtn.right
            anchors.leftMargin: 5
            text: "Image"
            onClicked: {
                sharing.shareType = MeegoSharingClientQmlObj.ShareTypeImage;
                shareObj.shareType = MeegoSharingClientQmlObj.ShareTypeImage;
            }
        }

        Button {
            id: audioBtn
            width: 100
            height: 50
            anchors.top: videoBtn.top
            anchors.left: imageBtn.right
            anchors.leftMargin: 5
            text: "Audio"
            onClicked: {
                sharing.shareType = MeegoSharingClientQmlObj.ShareTypeAudio;
                shareObj.shareType = MeegoSharingClientQmlObj.ShareTypeAudio;
            }
        }

        Button {
            id: addFile
            width: 100
            height: 50
            anchors.top: videoBtn.top
            anchors.left: audioBtn.right
            anchors.leftMargin: 5
            text: "Add File"
            onClicked: {
                sharing.addItem("/blah.jpg"); //Share Button
                shareObj.addItem("/blah.jpg"); //Share Context Menu
                shareIcn.addItem("/blah.jpg"); //Share Context Menu
            }
        }

        Button {
            id: toggleShareAll
            width: 100
            height: 50
            anchors.top: videoBtn.top
            anchors.left:  addFile.right
            anchors.leftMargin: 5
            text: {
                if (sharing.shareAll)
                    return "Disable\nShare All";
                else
                    return "Enable\nShare All";
            }
            onClicked: {
                sharing.shareAll = !sharing.shareAll;
            }

        }

        Button {
            id: clearSharing
            width: 100
            height: 50
            anchors.top: videoBtn.top
            anchors.left: toggleShareAll.right
            anchors.leftMargin: 5
            text: "Clear Items"
            onClicked: {
                sharing.clearItems();
                shareObj.clearItems();
            }
        }

        Button {
            id: toggleVisible
            width: 100
            height: 50
            anchors.top: videoBtn.bottom
            anchors.topMargin: 5
            anchors.left: imageBtn.left
            text: "Toggle\n Visible"
            onClicked: {
                sharing.visible = !sharing.visible;
            }
        }

        Rectangle {
            id: rect1
            color: "white"
            width: 100
            height: 50
            anchors.top: toggleVisible.top
            anchors.left: toggleVisible.right
            anchors.leftMargin: 5

            Text {
                id: textTypes
                text: "Context Menu w/ types in menu"
                anchors.fill: parent
            }
            MouseArea {
                anchors.fill: parent
                onClicked: textTypes.text = topItem.topItem.parent.parent.toString()
                onPressAndHold: {
                    var ctxList = ["First entry", "Second Entry"]
                    var x;
                    var svcTypes = shareObj.serviceTypes;
                    for (x in svcTypes) {
                        console.log("Adding share type " + svcTypes[x] + "to array " + ctxList);
                        ctxList = ctxList.concat(svcTypes[x]);
                    }
                    objContext.model = ctxList;
                    objContext.visible = true;
                }
            }

            Labs.ContextMenu {
                id: objContext
                mouseX: parent.x + parent.width
                mouseY: parent.y + (parent.height/2)
                onTriggered: {
                    var svcTypes = shareObj.serviceTypes;
                    var didShare = false;
                    for (x in svcTypes) {
                        if (model[index] == svcTypes[x]) {
                            shareObj.showContext(model[index], mouseX, mouseY);
                            didShare = true;
                            break;
                        }
                    }
                    if (!didShare) {
                        //Handle other context menu options here...
                    }
                }

            }

        }

        Rectangle {
            id: rect2
            anchors.left: rect1.left
            anchors.top: rect1.bottom
            anchors.topMargin: 20
            color: "green"
            width: 100
            height: 50
            Text {
                text: "Context Menu from Share ctx menu"
                anchors.fill: parent
            }
            MouseArea {
                anchors.fill: parent
                onPressAndHold: {
                    var ctxList = ["First entry", "Second Entry", "Share"]
                    var pos = topItem.topItem.mapFromItem(rect2, mouse.x, mouse.y)
                    ctxShare.mouseX = pos.x;
                    ctxShare.mouseY = pos.y;
                    ctxShare.model = ctxList;
                    ctxShare.visible = true;
                }
            }
            Labs.ContextMenu {
                id: ctxShare

                onTriggered: {
                    if (model[index] == "Share") {
                        shareObj.showContextTypes(mouseX, mouseY);
                    }
                }

            }

        }

        ShareObj {
            id: shareObj
        }

        ShareButton {
            id: sharing
            shareAll: false
            anchors.top: toggleVisible.bottom
            anchors.topMargin: 10
            anchors.left: imageBtn.left
            onShareAllClicked: {
                sharing.addItem("/foo/bar/blah.jpg");
                sharing.addItem("/home/test/myPicture.jpg");
                sharing.doShareAll();
            }
        }
	ShareIcon {
	    id: shareIcn
	    anchors.top: sharing.bottom
	    anchors.topMargin: 75
	    anchors.left: sharing.left
	}

        Component.onCompleted: {
            var i = 0;
            for (i = 0; i < 2; ++i) {
                //sharing.addItem("/home/testing/myFile" + i + ".jpg" );
            }
            console.log("File count: " + sharing.itemsToShare.length);

            var x = 0;
            for (x in sharing.itemsToShare) {
                console.log(sharing.itemsToShare[x]);
            }
        }
    }

}
