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
    id: window
    width: 1024
    height: 700

    MeeGoUXSharingClientQmlObj {
        id: sharingObj
    }

    Component.onCompleted: {
        var i = 0;
        for (i = 0; i < 2; ++i) {
            sharingObj.addFile("/home/testing/myFile" + i + ".jpg" );
        }
        console.log("File count: " + sharingObj.filesToShare.length);

        var x = 0;
        for (x in sharingObj.filesToShare) {
            console.log(sharingObj.filesToShare[x]);
        }
    }

//    fullscreen: true
//    showtoolbar: true

    Rectangle {
        id: main
        width: parent.width
        height: parent.height //- window.toolbarheight
        color: "grey"


        Item {
            id: buttonrow
            height: 20
            width: 100
            Rectangle {
                id: btnImage
                anchors.left: parent.left
                height: parent.height
                width: parent.width
                Text {
                    text: qsTr("Image")
                    anchors.fill: parent
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        sharingObj.shareType = MeeGoUXSharingClientQmlObj.ShareTypeImage
                        serviceTypeList.model = sharingObj.serviceTypes
                    }
                }
            }
            Rectangle {
                id: btnVideo
                anchors.left: btnImage.right
                height: parent.height
                width: parent.width
                Text {
                    text: qsTr("Video")
                    anchors.fill: parent
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        sharingObj.shareType = MeeGoUXSharingClientQmlObj.ShareTypeVideo
                        serviceTypeList.model = sharingObj.serviceTypes
                    }
                }
            }
            Rectangle {
                id: btnAudio
                anchors.left: btnVideo.right
                height: parent.height
                width: parent.width
                Text {
                    text: qsTr("Audio")
                    anchors.fill: parent
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        sharingObj.shareType = MeeGoUXSharingClientQmlObj.ShareTypeAudio
                        serviceTypeList.model = sharingObj.serviceTypes
                    }
                }
            }
            Rectangle {
                id: btnText
                anchors.left: btnAudio.right
                height: parent.height
                width: parent.width
                Text {
                    text: qsTr("Text")
                    anchors.fill: parent
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        sharingObj.clearFiles();
                        sharingObj.addFile("/tmp/body.txt");
                        sharingObj.addHashEntryToFile("/tmp/body.txt", "subject", "This is a test subject!");
                        sharingObj.shareType = MeeGoUXSharingClientQmlObj.ShareTypeText
                        serviceTypeList.model = sharingObj.serviceTypes
                    }
                }
            }
            Rectangle {
                id: btnVCard
                anchors.left: btnText.right
                height: parent.height
                width: parent.width
                Text {
                    text: qsTr("vCard")
                    anchors.fill: parent
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        sharingObj.clearFiles();
                        sharingObj.addFile("/tmp/vcard.vcf");
                        sharingObj.addHashEntryToFile("/tmp/vcard.vcf", "subject", "This is a vcard subject!");
                        sharingObj.shareType = MeeGoUXSharingClientQmlObj.ShareTypeVCard
                        serviceTypeList.model = sharingObj.serviceTypes
                    }
                }
            }
        }

        Item {
            id:scene1
            width: 600
            height: parent.height - buttonrow.height - 2
            anchors.top: buttonrow.bottom
            anchors.topMargin: 2

            ListView {
                id: serviceTypeList
                width: 100
                height: 100
                delegate: Rectangle {
                    height: 20
                    width: 100

                    Text {
                        anchors.fill: parent;
                        text: modelData;
                        color: "red"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                sharingObj.serviceType = parent.text
                                servicesList.model = sharingObj.serviceModel
                            }
                        }
                    }
                }
            }
            ListView {
                anchors.left: serviceTypeList.right
                anchors.leftMargin: 10
                id: servicesList
                width: 200
                height: 100
                delegate: Rectangle {
                    height: 20
                    width: parent.width

                    Text {
                        anchors.fill: parent
                        text: displayName
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            sharingObj.serviceName = name
                            customLoader.source = sharingObj.getCustomUIName("IVI", "myImaginaryDevice")
                        }
                    }
                }
            }

            Rectangle {
                id: clientArea
                anchors.top: serviceTypeList.bottom
                anchors.topMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 20
                height: 500
                width: 500
                color: "blue"

                Loader {
                    id: customLoader
                    anchors.fill: parent
                    onStatusChanged: if (customLoader.status == Loader.Ready) console.log('Loaded ' + source)
                }

                Connections {
                    target: customLoader.item
                    onCancel: {
                        customLoader.source = "";
                        //sharingObj.clearFiles();
                    }
                    onShared: {
                        console.log("Shared, with share ID " + shareid);
                        customLoader.source = "";
                        sharingObj.clearFiles();
			sharingObj.addFile("/usr/share/bar/foo.jpg");
                    }
                    onShareError: {
                        console.log("Share error occured: " + errMsg);
                        customLoader.source = "";
                        sharingObj.clearFiles();
                    }
                }
            }

            Rectangle {
                id: fileListArea
                anchors.left: clientArea.right
                anchors.leftMargin: 10
                width: 200
                height: 400
                ListView {
                    id: fileList
                    anchors.fill: parent
                    model: sharingObj.filesToShare
                    delegate: Rectangle {
                        width: parent.width
                        height: 20
                        Text {
                            text: modelData
                            anchors.fill: parent
                        }
                    }
                }
            }

        }
    }
}

