import 'package:flutter/material.dart';
import 'package:sync_watchers_client/widgets/room_menu.dart';
import 'package:sync_watchers_client/widgets/upload_button.dart';
import 'package:sync_watchers_client/widgets/video_player.dart';
import 'package:sync_watchers_client/widgets/videos_list.dart';

class VideoScreen extends StatelessWidget {
  final Map<String, dynamic> responseData;

  const VideoScreen({super.key, required this.responseData});

  String get role => responseData["role"];

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
                  if (role != "default") ...[const SizedBox(height: 16), UploadButtonWidget(onFileSelected: (String? path) => print(path))],
                ],
              ),
            ),

            const SizedBox(width: 16),

            Expanded(
              flex: 1,
              child: Column(
                children: [
                  if (role == "owner") ...[ControlMenuWidget(inviteLink: responseData["inviteLink"], onSelectVideo: () => print("Selected")), const SizedBox(height: 16)],
                  const DownloadListWidget(items: ["first", "second", "third"]),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}
