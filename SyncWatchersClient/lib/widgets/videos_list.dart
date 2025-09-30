import 'dart:async';
import 'dart:convert';

import 'package:flutter/material.dart';

import '../web/events.dart';
import '../web/requests.dart';
import '../web_listener.dart';

class UploadedVideo {
  final String userName;
  final String videoName;
  bool downloaded = false;

  UploadedVideo({required this.userName, required this.videoName});
}

class DownloadListWidget extends StatefulWidget {
  final Map<String, dynamic> responseData;

  const DownloadListWidget({super.key, required this.responseData});

  @override
  State<DownloadListWidget> createState() => _DownloadListState();
}

class _DownloadListState extends State<DownloadListWidget> implements WebListener {
  final List<UploadedVideo> _availableVideos = [];
  late Timer _timer;

  String get role => widget.responseData["role"];

  @override
  void initState() {
    super.initState();

    EventsHandler.instance.addListener(this);

    _timer = Timer.periodic(const Duration(seconds: 1), (_) {
      setState(() {});
    });
  }

  @override
  Widget build(BuildContext context) {
    return ListView.builder(
      shrinkWrap: true,
      itemCount: _availableVideos.length,
      itemBuilder: (context, index) {
        return Padding(
          padding: const EdgeInsets.symmetric(vertical: 4.0, horizontal: 8.0),
          child: Row(
            children: [
              Expanded(
                child: TextField(
                  onTap: role == "owner"
                      ? () async => await changeVideoRequest((String response) => print(response), (String errorMessage) => print(errorMessage), {
                          "roomUUID": widget.responseData["roomUUID"],
                          "videoName": _availableVideos[index].videoName,
                        })
                      : null,
                  controller: TextEditingController(text: _availableVideos[index].videoName),
                  readOnly: true,
                  decoration: const InputDecoration(border: OutlineInputBorder()),
                ),
              ),
              const SizedBox(width: 8),
              ElevatedButton(
                onPressed: _availableVideos[index].downloaded
                    ? null
                    : () async {
                        ScaffoldMessengerState messenger = ScaffoldMessenger.of(context);

                        messenger.hideCurrentSnackBar();
                        messenger.showSnackBar(SnackBar(content: Text("Download ${_availableVideos[index].videoName}")));

                        await downloadContent(
                          (String response) {
                            messenger.hideCurrentSnackBar();
                            messenger.showSnackBar(SnackBar(content: Text(response)));

                            _availableVideos[index].downloaded = true;
                          },
                          (String errorMessage) {
                            messenger.hideCurrentSnackBar();
                            messenger.showSnackBar(SnackBar(content: Text(errorMessage)));
                          },
                          {"roomUUID": widget.responseData["roomUUID"], "contentName": _availableVideos[index].videoName},
                        );
                      },
                child: _availableVideos[index].downloaded ? const Text("Downloaded") : const Text("Download"),
              ),
            ],
          ),
        );
      },
    );
  }

  @override
  Future<void> changeVideo(String videoName) async {}

  @override
  Future<void> onInvite(String userName) async {}

  @override
  Future<void> onRoomDelete() async {}

  @override
  Future<void> onUpdateRole(String role) async {}

  @override
  Future<void> onUploadContent(String userUUID) async {
    await getRoomInformation(
      (String response) {
        Map<String, dynamic> data = jsonDecode(response);
        String userName = data["name"];
        List<dynamic> uploadedContent = data["uploadedContent"];

        for (String contentName in uploadedContent) {
          if (_availableVideos.any((value) => value.videoName == contentName)) {
            continue;
          }

          _availableVideos.add(UploadedVideo(userName: userName, videoName: contentName));
        }
      },
      (String errorMessage) => print(errorMessage),
      {"userUUID": userUUID},
    );
  }

  @override
  Future<void> onUserNameUpdate(String oldUserName, String newUserName) async {}

  @override
  Future<void> play(String userName) async {}

  @override
  Future<void> rewind(int offsetInSecondsFromStart) async {}

  @override
  Future<void> stop(String userName) async {}
}
