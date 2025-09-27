import 'package:flutter/material.dart';
import 'package:sync_watchers_client/widgets/room_menu.dart';
import 'package:sync_watchers_client/widgets/upload_button.dart';
import 'package:sync_watchers_client/widgets/video_player.dart';
import 'package:sync_watchers_client/widgets/videos_list.dart';

/// Main screen layout
class VideoScreen extends StatelessWidget {
  final String role;

  const VideoScreen({super.key, required this.role});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Padding(
        padding: const EdgeInsets.all(16),
        child: Row(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            /// Left side (video + upload button)
            Expanded(
              flex: 2,
              child: Column(
                children: [
                  Expanded(child: VideoPlayerWidget()),
                  if (role != 'default') ...[const SizedBox(height: 16), const UploadButtonWidget()],
                ],
              ),
            ),

            const SizedBox(width: 16),

            /// Right side (menu + list)
            Expanded(
              flex: 1,
              child: Column(
                children: [
                  if (role == 'owner') ...[const ControlMenuWidget(), const SizedBox(height: 16)],
                  const DownloadListWidget(),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}
