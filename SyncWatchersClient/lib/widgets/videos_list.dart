import 'dart:async';
import 'dart:convert';

import 'package:flutter/material.dart';

import 'package:sync_watchers_client/web/events.dart';
import 'package:sync_watchers_client/web/requests.dart';
import 'package:sync_watchers_client/web_listener.dart';

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

  void _addVideos(Iterator<dynamic> it, String Function(Iterator<dynamic>) contentNameGetter, String Function(Iterator<dynamic>) userNameGetter) {
    while (it.moveNext()) {
      String contentName = contentNameGetter(it);

      if (_availableVideos.any((value) => value.videoName == contentName)) {
        continue;
      }

      _availableVideos.add(UploadedVideo(userName: userNameGetter(it), videoName: contentName));
    }
  }

  @override
  void initState() {
    super.initState();

    EventsHandler.instance.addListener(this);

    getRoomContent(
      (String response) {
        Map<String, dynamic> json = jsonDecode(response);
        List<dynamic> uploadedContent = json["uploadedContent"];

        _addVideos(uploadedContent.iterator, (it) => it.current["contentName"], (it) => it.current["userName"]);
      },
      (String errorMessage) => print(errorMessage),
      {"roomUUID": widget.responseData["roomUUID"]},
    ).whenComplete(
      () => _timer = Timer.periodic(const Duration(seconds: 1), (_) {
        setState(() {});
      }),
    );
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
  void dispose() {
    EventsHandler.instance.removeListener(this);

    super.dispose();
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

        _addVideos(uploadedContent.iterator, (it) => it.current, (it) => userName);
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
