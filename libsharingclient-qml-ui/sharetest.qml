import Qt 4.7
import MeeGo.Components 0.1
import Sharing 0.1
import Sharing.UI 0.1

Window {
    id: window

    Theme {
        id: theme
    }

    fullContent: true
    fullScreen: true

    bookMenuModel: ["MainPage"]
    bookMenuPayload: [mainPage]

    Component.onCompleted: {
        switchBook(mainPage)
    }


    Component{
      id: mainPage
      AppPage {
        anchors.fill: parent


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
                sharing.shareType = SharingClientQmlObj.ShareTypeVideo;
                shareObj.shareType = SharingClientQmlObj.ShareTypeVideo;
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
                sharing.shareType = SharingClientQmlObj.ShareTypeImage;
                shareObj.shareType = SharingClientQmlObj.ShareTypeImage;
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
                sharing.shareType = SharingClientQmlObj.ShareTypeAudio;
                shareObj.shareType = SharingClientQmlObj.ShareTypeAudio;
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


        InfoBar {
            id: infoBar
            text: "This is an InfoBar! Now with really really really really really really long text!!!"
            anchors.top: videoBtn.top
            anchors.left: clearSharing.right
            anchors.leftMargin: 20
        }

        Button {
            id: showHideInfo
            text: "Toggle InfoBar"
            anchors.top: videoBtn.top
            anchors.left: infoBar.right
            anchors.leftMargin: 5
            property bool ibVisible: false
            onClicked: {
                if (ibVisible)
                    infoBar.hide();
                else
                    infoBar.show();
                ibVisible = !ibVisible;
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
            color: "blue"
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
                onPressAndHold: {
                    var ctxList = ["First entry", "Second Entry"]
                    var x;
                    var svcTypes = shareObj.serviceTypes;
                    for (x in svcTypes) {
                        console.log("Adding share type " + svcTypes[x] + "to array " + ctxList);
                        ctxList = ctxList.concat(svcTypes[x]);
                    }
                    objContextAction.model = ctxList;
                    var pos = mapToItem(topItem.topItem, rect1.width, rect1.height/2)
                    objContext.myX = pos.x;
                    objContext.myY = pos.y;
                    objContext.setPosition(pos.x, pos.y)
                    objContext.show();

                }
            }

            ContextMenu {
                id: objContext
                property int myX
                property int myY

                onRejected: objContext.hide()
                content: ActionMenu {
                    id: objContextAction

                    onTriggered: {
                        var svcTypes = shareObj.serviceTypes;
                        var didShare = false;
                        for (x in svcTypes) {
                            if (model[index] == svcTypes[x]) {
                                shareObj.showContext(model[index], objContext.myX, objContext.myY);
                                didShare = true;
                                break;
                            }
                        }
                        if (!didShare) {
                            //Handle other context menu options here...
                        }
                        objContext.hide();
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
                    var pos = mapToItem(topItem.topItem, mouse.x, mouse.y)
                    ctxShare.setPosition(pos.x, pos.y);
                    ctxShare.myX = pos.x;
                    ctxShare.myY = pos.y;
                    ctxShareAction.model = ctxList;
                    ctxShare.show();
                }
            }
            ContextMenu {
                id: ctxShare
                onRejected: ctxShare.hide()
                property int myX
                property int myY

                content: ActionMenu {
                    id: ctxShareAction

                    onTriggered: {
                        if (model[index] == "Share") {
                            shareObj.showContextTypes(ctxShare.myX, ctxShare.myY);
                        } else {
                            console.log("Didn't get to Share in context: '" + model[index] + "'");
                        }

                        ctxShare.hide()
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

}
