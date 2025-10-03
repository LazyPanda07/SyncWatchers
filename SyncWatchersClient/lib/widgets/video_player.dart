import 'dart:io';

import 'package:flutter/material.dart';
import 'package:path_provider/path_provider.dart';
import 'package:sync_watchers_client/settings.dart';
import 'package:sync_watchers_client/web/events.dart';
import 'package:sync_watchers_client/web/requests.dart';
import 'package:sync_watchers_client/web_listener.dart';
import 'package:video_player/video_player.dart';
import 'package:window_manager/window_manager.dart';

class VideoPlayerWidget extends StatefulWidget {
  const VideoPlayerWidget({super.key});

  @override
  State<VideoPlayerWidget> createState() => _VideoPlayerWidgetState();
}

class _VideoPlayerWidgetState extends State<VideoPlayerWidget> implements WebListener {
  VideoPlayerController? _controller;

  @override
  void initState() {
    super.initState();

    EventsHandler.instance.addListener(this);
  }

  @override
  void dispose() {
    EventsHandler.instance.removeListener(this);

    _controller?.dispose();
    super.dispose();
  }

  void _enterFullscreen() {
    Navigator.of(context).push(MaterialPageRoute(builder: (context) => FullscreenVideoPlayer(controller: _controller!)));
  }

  @override
  Widget build(BuildContext context) {
    if (_controller == null) {
      return Container(
        color: Colors.black,
        child: const Center(child: CircularProgressIndicator()),
      );
    }

    return Container(
      color: Colors.black,
      child: _controller!.value.isInitialized
          ? Stack(
              alignment: Alignment.bottomCenter,
              children: [
                Center(
                  child: FittedBox(
                    fit: BoxFit.contain,
                    child: SizedBox(width: _controller!.value.size.width, height: _controller!.value.size.height, child: VideoPlayer(_controller!)),
                  ),
                ),
                VideoPlayerControls(controller: _controller!, onFullscreenToggle: _enterFullscreen, isFullscreen: false),
              ],
            )
          : const Center(child: CircularProgressIndicator()),
    );
  }

  @override
  Future<void> changeVideo(String videoName) async {
    if (_controller != null) {
      if (_controller!.value.isInitialized) {
        await _controller!.pause();
      }

      await _controller!.dispose();
    }

    Directory temporaryDirectory = await getTemporaryDirectory();
    String videoPath = "${temporaryDirectory.path}/$videoName";

    print("Open video from $videoPath");

    _controller = VideoPlayerController.file(File(videoPath))..initialize().whenComplete(() => setState(() {}));
  }

  @override
  Future<void> onInvite(String userName) async {}

  @override
  Future<void> onRoomDelete() async {}

  @override
  Future<void> onUpdateRole(String role) async {}

  @override
  Future<void> onUploadContent(String userUUID) async {}

  @override
  Future<void> onUserNameUpdate(String oldUserName, String newUserName) async {}

  @override
  Future<void> play(String userName) async {
    await _controller!.play();
  }

  @override
  Future<void> rewind(int offsetInSecondsFromStart) async {
    await _controller!.seekTo(Duration(seconds: offsetInSecondsFromStart));
  }

  @override
  Future<void> stop(String userName) async {
    await _controller!.pause();
  }
}

class FullscreenVideoPlayer extends StatelessWidget {
  final VideoPlayerController controller;

  const FullscreenVideoPlayer({super.key, required this.controller});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.black,
      body: SafeArea(
        child: Stack(
          alignment: Alignment.bottomCenter,
          children: [
            SizedBox.expand(
              child: FittedBox(
                fit: BoxFit.contain, // keep aspect ratio
                child: SizedBox(width: controller.value.size.width, height: controller.value.size.height, child: VideoPlayer(controller)),
              ),
            ),
            VideoPlayerControls(controller: controller, onFullscreenToggle: () => Navigator.pop(context), isFullscreen: true),
          ],
        ),
      ),
    );
  }
}

class VideoPlayerControls extends StatefulWidget {
  final VideoPlayerController controller;
  final VoidCallback onFullscreenToggle;
  final bool isFullscreen;

  const VideoPlayerControls({super.key, required this.controller, required this.onFullscreenToggle, required this.isFullscreen});

  @override
  State<VideoPlayerControls> createState() => _VideoPlayerControlsState();
}

class _VideoPlayerControlsState extends State<VideoPlayerControls> {
  double _volume = 0.4;

  @override
  void initState() {
    super.initState();
    widget.controller.setVolume(_volume);

    widget.controller.addListener(_update); // listen for play/pause updates
  }

  @override
  void dispose() {
    widget.controller.removeListener(_update);

    super.dispose();
  }

  void _update() {
    if (mounted) setState(() {});
  }

  @override
  Widget build(BuildContext context) {
    return Positioned(
      bottom: 16,
      left: 8,
      right: 8,
      child: Column(
        children: [
          GestureDetector(
            onTapUp: (details) async {
              final RenderBox box = context.findRenderObject() as RenderBox;
              final double dx = box.globalToLocal(details.globalPosition).dx;
              final double width = box.size.width;
              final Duration position = widget.controller.value.duration * (dx / width);
              final int seconds = position.inSeconds;
              final bool wasPlaying = widget.controller.value.isPlaying;

              stopRequest((response) {}, (errorMessage) {}, {"roomUUID": Settings.instance.roomUUID, "userName": Settings.instance.userName}).whenComplete(
                () => rewindRequest((response) {}, (errorMessage) {}, {"roomUUID": Settings.instance.roomUUID, "offset": "$seconds"}).whenComplete(() {
                  if (wasPlaying) {
                    playRequest((response) {}, (errorMessage) {}, {"roomUUID": Settings.instance.roomUUID, "userName": Settings.instance.userName});
                  }
                }),
              );
            },
            child: VideoProgressIndicator(
              widget.controller,
              allowScrubbing: false,
              colors: const VideoProgressColors(playedColor: Colors.red, bufferedColor: Colors.white, backgroundColor: Colors.grey),
            ),
          ),
          const SizedBox(height: 8),
          Row(
            children: [
              IconButton(
                icon: Icon(widget.controller.value.isPlaying ? Icons.pause : Icons.play_arrow, color: Colors.white),
                onPressed: () {
                  if (widget.controller.value.isPlaying) {
                    stopRequest((String response) {}, (String errorMessage) => print(errorMessage), {
                      "roomUUID": Settings.instance.roomUUID,
                      "userName": Settings.instance.userName,
                    });
                  } else {
                    playRequest((String response) {}, (String errorMessage) => print(errorMessage), {
                      "roomUUID": Settings.instance.roomUUID,
                      "userName": Settings.instance.userName,
                    });
                  }
                },
              ),
              IconButton(
                icon: const Icon(Icons.replay_10, color: Colors.white),
                onPressed: () {
                  final current = widget.controller.value.position;
                  widget.controller.seekTo(current - const Duration(seconds: 10));
                },
              ),
              Expanded(
                child: Slider(
                  min: 0,
                  max: 1,
                  value: _volume,
                  onChanged: (value) {
                    setState(() {
                      _volume = value;
                      widget.controller.setVolume(_volume);
                    });
                  },
                ),
              ),
              IconButton(
                icon: Icon(widget.isFullscreen ? Icons.fullscreen_exit : Icons.fullscreen, color: Colors.white),
                onPressed: () async {
                  if (widget.isFullscreen) {
                    // Exit fullscreen
                    await windowManager.setFullScreen(false);
                    setState(() {
                      Navigator.pop(context); // exit fullscreen route
                    });
                  } else {
                    // Enter fullscreen
                    await windowManager.setFullScreen(true);
                    widget.onFullscreenToggle(); // push fullscreen route
                  }
                },
              ),
            ],
          ),
        ],
      ),
    );
  }
}
