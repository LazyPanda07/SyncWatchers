import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:sync_watchers_client/widgets/user_name_screen.dart';

import '../web/requests.dart';

class RoomScreen extends StatefulWidget {
  const RoomScreen({super.key});

  @override
  State<RoomScreen> createState() => _RoomScreenState();
}

class _RoomScreenState extends State<RoomScreen> {
  late ScaffoldMessengerState _messenger;
  final TextEditingController _roomNameController = TextEditingController();
  final TextEditingController _inviteController = TextEditingController();
  SnackBar? _currentSnackBar;

  void _showSnackBar(SnackBar? snackBar) {
    if (_currentSnackBar != null) {
      _messenger.hideCurrentSnackBar();
    }

    _currentSnackBar = snackBar;

    if (snackBar != null) {
      _messenger.showSnackBar(snackBar).closed.whenComplete(() => _currentSnackBar = null);
    }
  }

  Future<void> _createAndJoinToRoom(String roomName) async {
    _showSnackBar(const SnackBar(content: Text("Creating room...")));

    return createRoom(
      (String response) async {
        _showSnackBar(SnackBar(content: Text("Joining to created room..."), onVisible: () async => await _joinToRoom(jsonDecode(response)["inviteLink"], "owner")));
      },
      (String errorMessage) {
        _showSnackBar(SnackBar(content: Text(errorMessage)));
      },
      {"roomName": roomName},
    );
  }

  Future<void> _joinToRoom(String link, String role) async {
    return joinRoom(
      (String response) {
        final Map<String, dynamic> responseData = jsonDecode(response);

        responseData["inviteLink"] = link;

        Navigator.push(context, MaterialPageRoute(builder: (_) => UsernameScreen(responseData: responseData)));
      },
      (String errorMessage) {
        _showSnackBar(SnackBar(content: Text(errorMessage)));
      },
      {"inviteLink": link, "role": role},
    );
  }

  @override
  Widget build(BuildContext context) {
    _messenger = ScaffoldMessenger.of(context);

    return Scaffold(
      appBar: AppBar(title: const Text("Sync Watchers")),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Row(
          children: [
            Expanded(
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  TextField(
                    controller: _roomNameController,
                    decoration: const InputDecoration(hintText: "Room Name", border: OutlineInputBorder()),
                  ),
                  const SizedBox(height: 8),
                  ElevatedButton(
                    onPressed: () {
                      if (_currentSnackBar == null) {
                        _createAndJoinToRoom(_roomNameController.text);
                      }
                    },
                    child: const Text("Create Room"),
                  ),
                ],
              ),
            ),

            const SizedBox(width: 16),

            Expanded(
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  TextField(
                    controller: _inviteController,
                    decoration: const InputDecoration(hintText: "Invite Link", border: OutlineInputBorder()),
                  ),
                  const SizedBox(height: 8),
                  ElevatedButton(
                    onPressed: () async {
                      if (_currentSnackBar == null) {
                        _showSnackBar(SnackBar(content: Text("Joining to room"), onVisible: () async => _joinToRoom(_inviteController.text, "default")));
                      }
                    },
                    child: const Text("Join Room"),
                  ),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}
