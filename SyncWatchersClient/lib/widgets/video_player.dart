import 'dart:io';

import 'package:flutter/material.dart';
import 'package:path_provider/path_provider.dart';
import 'package:sync_watchers_client/web/events.dart';
import 'package:sync_watchers_client/web_listener.dart';
import 'package:video_player/video_player.dart';
import 'package:window_manager/window_manager.dart';

class VideoPlayerWidget extends StatefulWidget {
  const VideoPlayerWidget({super.key});

  @override
  State<VideoPlayerWidget> createState() => _VideoPlayerWidgetState();
}

class _VideoPlayerWidgetState extends State<VideoPlayerWidget> implements WebListener {
  late VideoPlayerController _controller;

  @override
  void initState() {
    super.initState();

    EventsHandler.instance.addListener(this);

    _controller = VideoPlayerController.asset("")
      ..initialize().whenComplete(() {
        setState(() {});
      });
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  void _enterFullscreen() {
    Navigator.of(context).push(MaterialPageRoute(builder: (context) => FullscreenVideoPlayer(controller: _controller)));
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      color: Colors.black,
      child: _controller.value.isInitialized
          ? Stack(
              alignment: Alignment.bottomCenter,
              children: [
                Center(
                  child: FittedBox(
                    fit: BoxFit.contain,
                    child: SizedBox(width: _controller.value.size.width, height: _controller.value.size.height, child: VideoPlayer(_controller)),
                  ),
                ),
                VideoPlayerControls(controller: _controller, onFullscreenToggle: _enterFullscreen, isFullscreen: false),
              ],
            )
          : const Center(child: CircularProgressIndicator()),
    );
  }

  @override
  Future<void> changeVideo(String videoName) async {
    if (_controller.value.isInitialized) {
      await _controller.pause();
      await _controller.dispose();
    }

    Directory temporaryDirectory = await getTemporaryDirectory();

    _controller = VideoPlayerController.file(File("${temporaryDirectory.path}/$videoName"))..initialize().whenComplete(() => setState(() {}));
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
  Future<void> play(String userName) async {}

  @override
  Future<void> rewind(int offsetInSecondsFromStart) async {}

  @override
  Future<void> stop(String userName) async {}
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
  double _volume = 1.0;

  @override
  void initState() {
    super.initState();
    _volume = widget.controller.value.volume;
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
          VideoProgressIndicator(
            widget.controller,
            allowScrubbing: true,
            colors: const VideoProgressColors(playedColor: Colors.blue, bufferedColor: Colors.grey, backgroundColor: Colors.black),
          ),
          const SizedBox(height: 8),
          Row(
            children: [
              IconButton(
                icon: Icon(widget.controller.value.isPlaying ? Icons.pause : Icons.play_arrow, color: Colors.white),
                onPressed: () {
                  if (widget.controller.value.isPlaying) {
                    widget.controller.pause();
                  } else {
                    widget.controller.play();
                  }
                },
              ),
              IconButton(
                icon: const Icon(Icons.stop, color: Colors.white),
                onPressed: () {
                  widget.controller.pause();
                  widget.controller.seekTo(Duration.zero);
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
