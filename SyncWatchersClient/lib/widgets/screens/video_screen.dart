import 'dart:async';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:sync_watchers_client/web/events.dart';
import 'package:sync_watchers_client/web_listener.dart';
import 'package:sync_watchers_client/widgets/room_menu.dart';
import 'package:sync_watchers_client/widgets/upload_button.dart';
import 'package:sync_watchers_client/widgets/video_player.dart';
import 'package:sync_watchers_client/widgets/videos_list.dart';

class VideoScreen extends StatefulWidget {
  final Map<String, dynamic> responseData;

  const VideoScreen({super.key, required this.responseData});

  @override
  State<VideoScreen> createState() => _VideoScreenState();
}

class _VideoScreenState extends State<VideoScreen> {
  String get role => widget.responseData["role"];

  @override
  void initState() {
    super.initState();

    SystemChrome.setPreferredOrientations([DeviceOrientation.landscapeLeft, DeviceOrientation.landscapeRight]);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Padding(
        padding: const EdgeInsets.all(16),
        child: Row(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Expanded(
              flex: 2,
              child: Column(
                children: [
                  Expanded(child: VideoPlayerWidget()),
                  if (role != "default") ...[const SizedBox(height: 16), UploadButtonWidget(responseData: widget.responseData)],
                ],
              ),
            ),

            const SizedBox(width: 16),

            Expanded(
              flex: 1,
              child: Column(
                children: [
                  if (role == "owner") ...[ControlMenuWidget(inviteLink: widget.responseData["inviteLink"], onSelectVideo: () => print("Selected")), const SizedBox(height: 16)],
                  DownloadListWidget(responseData: widget.responseData),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }

  @override
  void dispose() {
    SystemChrome.setPreferredOrientations([DeviceOrientation.portraitUp, DeviceOrientation.portraitDown, DeviceOrientation.landscapeLeft, DeviceOrientation.landscapeRight]);

    super.dispose();
  }
}
