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
import MeeGo.Ux.Gestures 0.1

Item {

    id: shareContainer
    //anchors.fill: parent
    height: childrenRect.height
    width: childrenRect.width

    property alias shareCount: sharingObj.fileCount
    property alias shareType: sharingObj.shareType
    property alias itemsToShare: sharingObj.filesToShare
    property alias serviceTypes: sharingObj.serviceTypes
    property QtObject sharingObj: MeeGoUXSharingClientQmlObj {
        id: sharingObj
        shareType: MeeGoUXSharingClientQmlObj.ShareTypeImage;
    }
    property variant catModel: null
    property variant svcModel: null
    property variant svcPayload: null

    property variant localStorage: null

    property string loaderSource: ""
    property string loaderState: ""
    property int defaultMargin: 15


    signal sharingComplete()

    function addItem(name) {
        sharingObj.addFile(name);
    }

    function delItem(name) {
        sharingObj.removeFile(name);
    }

    function clearItems() {
        sharingObj.clearFiles();
    }

    function addItems(itemList) {
        sharingObj.addFiles(itemList);
    }

    function setParam(fileName, paramName, paramVal) {
        sharingObj.addHashEntryToFile(fileName, paramName, paramVal);
    }

    function getParam(fileName, paramName) {
        return sharingObj.getHashEntryForFile(fileName, paramName);
    }

    function getParamDefVal(fileName, paramName, defaultVal) {
        return sharingObj.getHashEntryForFile(fileName, paramName, defaultVal);
    }

    function showContextTypes(x, y) {
        if (sharingObj.fileCount == 0)
            return;
        shareContainer.catModel = sharingObj.serviceTypes;
        shareContainer.loaderState = "selectCat";
        mdlSurface.show();
        return;

        ctxServiceTypesAction.model = sharingObj.serviceTypes;
        ctxServiceTypes.myX = x;
        ctxServiceTypes.myY = y;
        ctxServiceTypes.setPosition(x, y);
        ctxServiceTypes.show();
    }

    function showContext(svcType, x, y) {
        if (sharingObj.fileCount == 0)
            return;
        sharingObj.serviceType = svcType;
        var services = sharingObj.serviceModel.getServiceTypesList();
        ctxServicesAction.payload = sharingObj.serviceModel;
        ctxServicesAction.model = services;

        shareContainer.svcModel = services;
        shareContainer.svcPayload = sharingObj.serviceModel;

        ctxServices.setPosition(x, y);
        if ((svcType == qsTr("Email")) && (services.length == 1)) {
            selectService(services[0], sharingObj.serviceModel);
        } else {
            shareContainer.loaderState = "selectService"
            if (!mdlSurface.visible)
                mdlSurface.show();
        }
//            ctxServices.show();
    }

    function selectService(svcDispName, svcModel) {
        var svcName = svcModel.getServiceNameFromDisplayName(svcDispName);
        sharingObj.serviceName = svcName;
        loaderSource = sharingObj.getCustomUIName("meego", "ux");
        shareContainer.loaderState = "customUI";
        if (!mdlSurface.visible)
            mdlSurface.show();
    }


    Translator {
        catalog: "meego-ux-sharing-qml-ui"
    }

    ContextMenu {
        id: ctxServiceTypes
        property int myX
        property int myY
        onRejected: ctxServiceTypes.hide()

        content: ActionMenu {
            id: ctxServiceTypesAction
            onTriggered: {
                sharingObj.serviceType = model[index];
                var services = sharingObj.serviceModel.getServiceTypesList();
                if ((model[index] == qsTr("Email")) && ( services.length == 1))
                    selectService(services[0], sharingObj.serviceModel);
                else
                    showContext(model[index], ctxServiceTypes.myX, ctxServiceTypes.myY);
                ctxServiceTypes.hide();
            }
        }
    }

    ContextMenu {
        id: ctxServices
        onRejected: ctxServices.hide()

        content: ActionMenu {
            id: ctxServicesAction

            onTriggered: {
                var svcDispName = model[index];
                selectService(model[index], payload);
                ctxServices.hide();
            }
        }
    }

    ModalFog {
        id: mdlSurface
        autoCenter: true
        modalSurface: BorderImage {
            id: dlgItem
            anchors.centerIn: parent
            border.top: 14
            border.left: 20
            border.right: 20
            border.bottom: 20

            source: "image://themedimage/images/notificationBox_bg"
            width: flickable.width
            height: flickable.height


            property QtObject sharingObj: shareContainer.sharingObj
            property string qmlSource: shareContainer.loaderSource
            property int shareID: -1;
            property int shareProgressL: 0;
            property string shareError: "";

            Connections {
                target: sharingObj
                onShareProgress: {
                    //For now, only worry about our most recent share op
                    console.log("Got share progress, service: " + serviceName + ", opid: " + opid + ", progress: " + progress + ", msg: " + message + ", curShareID: " + dlgItem.shareID);
                    if (opid == dlgItem.shareID) {
                        if (progress == -1) {
                            //Error dlg!
                            shareError = message;
                            customLoader.sourceComponent = errorDlg;
                        } else {
                            shareProgressL = progress;
                        }
                    }
                }
            }

            Connections {
                target: mdlSurface
                onClosed: {
                    dlgItem.shareID = -1;
                    dlgItem.shareProgressL = 0;
                    dlgItem.shareError = "";
                    shareContainer.sharingComplete();
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("BI filler MouseArea clicked!");
                }
            }

            Flickable {
                id: flickable
                width: Math.min(contentWidth, screenItem.topItem.width * 0.90)
                height: Math.min(contentHeight, screenItem.topItem.height * 0.90)
                anchors.centerIn: parent
                contentWidth: customLoader.item.width
                contentHeight: customLoader.item.height
                clip: true
                boundsBehavior: Flickable.StopAtBounds

                Loader {
                    id: customLoader
                    //source: dlgItem.qmlSource
                    anchors.centerIn: parent
                    onStatusChanged: {
                        if (customLoader.status == Loader.Ready) {
                            console.log('customLoader Loaded ' + source);
                            console.log('customLoader width/height:', item.width, item.height);
                        }
                    }
                    states: [
                        State {
                            name: "selectCat"
                            PropertyChanges { target: customLoader; sourceComponent: catSelectComp }
                        },
                        State {
                            name: "selectService"
                            PropertyChanges { target: customLoader; sourceComponent: serviceSelectComp }
                        },
                        State {
                            name: "customUI"
                            PropertyChanges { target: customLoader; source: dlgItem.qmlSource }
                        }
                    ]
                }

                Connections {
                    target: shareContainer
                    onLoaderStateChanged: {
                        customLoader.state = shareContainer.loaderState;
                    }
                }

                Connections {
                    target: customLoader.item
                    onCancel: {
                        customLoader.source = "";
                        mdlSurface.hide();
			shareContainer.sharingComplete();
                    }
                    onShared: {
                        console.log("Shared, with share ID " + shareid);
                        customLoader.source = "";
                        dlgItem.shareID = shareid;
                        if (dlgItem.shareID != -1) {
                            console.log("Progress dialog goes here...");
                            mdlSurface.hide();
                        } else {
                            mdlSurface.hide();
                            sharingObj.clearFiles();
                        }
			shareContainer.sharingComplete();
                    }
                    onShareError: {
                        console.log("Share error occured: " + errMsg);
                        customLoader.source = "";
                        shareError = errMsg;
//                        customLoader.sourceComponent = errorDlg;
                        sharingObj.clearFiles();
                        mdlSurface.hide();
			shareContainer.sharingComplete();
                    }
                }
            }

        }
    }

    TopItem {
        id: screenItem
    }

    Component {
        id: catSelectComp
        Item {
            height: childrenRect.height + (2 * shareContainer.defaultMargin)
            //Not sure why it's happening, but as soon as an item is selected,
            //the catList width is increasing by 15, which causes the dialog
            //to increase size, and everything shifts around - really ugly
            //Assigning instead of binding works around the issue for the moment...

            //width: childrenRect.width
            Component.onCompleted: {
                width = childrenRect.width
            }

            Text {
                id: shareByText
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: shareContainer.defaultMargin
                font.pixelSize: theme.fontPixelSizeLarge
                font.family: theme.fontFamily
                color: theme.fontColorNormal
                text: qsTr("Share by:")
            }

            ListView {
                id: catList
                anchors.top: shareByText.bottom
                anchors.margins: shareContainer.defaultMargin

                width: contentItem.childrenRect.width + (2 * shareContainer.defaultMargin)
                height: 200 //contentItem.width //childrenRect.height
                interactive: contentHeight > height
                Component.onCompleted: {
                    console.log("**** cL.width: ", width);
                }
                anchors.horizontalCenter: parent.horizontalCenter

                clip: true
                focus: true
                currentIndex: -1

                model: shareContainer.catModel

                delegate: BorderImage {
                    id: itemSurface

                    width: Math.max(childrenRect.width, 150)
                    height: 50
//                    anchors.left: parent.left
//                    anchors.leftMargin: 5
                    anchors.horizontalCenter: parent.horizontalCenter

                    border.left: 5; border.top: 5
                    border.right: 5; border.bottom: 5

                    source: "image://themedimage/images/media/music_row_" + (itemSurface.ListView.isCurrentItem ? "highlight_landscape" : "landscape")
                    Component.onCompleted: {
                        console.log("**** BI.oC, width: ", width);
                    }

                    Text {
                        text: modelData
                        font.pixelSize: theme.fontPixelSizeMediumLarge
                        font.family: theme.fontFamily
                        color: theme.fontColorNormal

                        elide: Text.ElideRight

                        anchors.left: parent.left
    //                    anchors.margins: 35
                        anchors.centerIn: parent
                        //anchors.verticalCenter: parent.verticalCenter
                    }

                    GestureArea {
                        anchors.fill: parent

                        Tap {
                            onFinished: {
                                catList.currentIndex = model.index;
                                console.log("**** BI on Selected width:", itemSurface.width);
                                console.log("**** BI on Selected catList.width:", catList.width);
                            }
                        }
                    }
                }
            }
            Button {
                anchors.top: catList.bottom
                anchors.margins: shareContainer.defaultMargin
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Ok")
                active: true
                //enabled: (catList.currentIndex != -1)
                onClicked: {
                    var index = catList.currentIndex;
                    var model = shareContainer.catModel;
                    shareContainer.sharingObj.serviceType = model[index];
                    var services = shareContainer.sharingObj.serviceModel.getServiceTypesList();
                    if ((model[index] == qsTr("Email")) && (services.length == 1))
                        selectService(services[0], shareContainer.sharingObj.serviceModel);
                    else
                        shareContainer.showContext(model[index], 0, 0);
                    catList.currentIndex = -1;
                }
            }
        }
    }

    Component {
        id: serviceSelectComp
        Item {
            height: childrenRect.height + (2 * shareContainer.defaultMargin)
            //Not sure why it's happening, but as soon as an item is selected,
            //the catList width is increasing by 15, which causes the dialog
            //to increase size, and everything shifts around - really ugly
            //Assigning instead of binding works around the issue for the moment...

            //width: childrenRect.width
            Component.onCompleted: {
                width = childrenRect.width
            }

            Text {
                id: svcSelectText
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: shareContainer.defaultMargin
                font.pixelSize: theme.fontPixelSizeLarge
                font.family: theme.fontFamily
                color: theme.fontColorNormal
                text: qsTr("Select service:")
            }

            ListView {
                id: svcList
                anchors.top: svcSelectText.bottom
                anchors.margins: shareContainer.defaultMargin

                width: contentItem.childrenRect.width + (2 * shareContainer.defaultMargin)
                height: 200 //contentItem.width //childrenRect.height
                interactive: contentHeight > height
                Component.onCompleted: {
                    console.log("**** cL.width: ", width);
                }
                anchors.horizontalCenter: parent.horizontalCenter

                clip: true
                focus: true
                currentIndex: -1

                model: shareContainer.svcModel

                delegate: BorderImage {
                    id: itemSurface

                    width: Math.max(childrenRect.width, 150)
                    height: 50
//                    anchors.left: parent.left
//                    anchors.leftMargin: 5
                    anchors.horizontalCenter: parent.horizontalCenter

                    border.left: 5; border.top: 5
                    border.right: 5; border.bottom: 5

                    source: "image://themedimage/images/media/music_row_" + (itemSurface.ListView.isCurrentItem ? "highlight_landscape" : "landscape")
                    Component.onCompleted: {
                        console.log("**** BI.oC, width: ", width);
                    }

                    Text {
                        text: modelData
                        font.pixelSize: theme.fontPixelSizeMediumLarge
                        font.family: theme.fontFamily
                        color: theme.fontColorNormal

                        elide: Text.ElideRight

                        anchors.left: parent.left
    //                    anchors.margins: 35
                        anchors.centerIn: parent
                        //anchors.verticalCenter: parent.verticalCenter
                    }

                    GestureArea {
                        anchors.fill: parent

                        Tap {
                            onFinished: {
                                svcList.currentIndex = model.index;
                                console.log("**** BI on Selected width:", itemSurface.width);
                                console.log("**** BI on Selected catList.width:", svcList.width);
                            }
                        }
                    }
                }
            }
            Button {
                anchors.top: svcList.bottom
                anchors.margins: shareContainer.defaultMargin
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Ok")
                active: true
                //enabled: (catList.currentIndex != -1)
                onClicked: {
                    var index = svcList.currentIndex;
                    var model = shareContainer.svcModel;
                    shareContainer.selectService(model[index], shareContainer.svcPayload);

                    //shareContainer.sharingObj.serviceType = model[index];
//                    var services = shareContainer.sharingObj.serviceModel.getServiceTypesList();
//                    if ((model[index] == qsTr("Email")) && (services.length == 1))
//                        selectService(services[0], shareContainer.sharingObj.serviceModel);
//                    else
//                        shareContainer.showContext(model[index], 0, 0);
                    svcList.currentIndex = -1;
                }
            }
        }
    }

    Component {
        id: errorDlg
        Item {
            id: errorItem
            height: childrenRect.height + 20
            width: childrenRect.width + 20
            Text {
                id: txtError
                anchors.top: parent.top
                anchors.topMargin: 10
                text: qsTr("An error occurred while sharing: %1").arg(dlgItem.shareError);
                anchors.left: parent.left
                anchors.leftMargin: 10
            }
            Button {
                text: qsTr("Ok")
                anchors.top: txtError.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    mdlSurface.hide();
                }
            }
            onHeightChanged: {
                customLoader.width = errorItem.width;
                customLoader.height = errorItem.height;
            }
        }
    }

    Component {
        id: progressDlg
        Item {
            id: progressItem
            height: childrenRect.height + 20
            width: childrenRect.width + 20

            Text {
                id: txtProgress
                anchors.top: parent.top
                anchors.topMargin: 10
                text: qsTr("Upload progress: %1%%").arg(shareProgressL);
                anchors.left: parent.left
                anchors.leftMargin: 10
            }
            Button {
                text: qsTr("Ok")
                anchors.top: txtProgress.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    mdlSurface.hide();
                }
            }
            onHeightChanged: {
                customLoader.width = progressItem.width;
                customLoader.height = progressItem.height;
            }
        }
    }
}

