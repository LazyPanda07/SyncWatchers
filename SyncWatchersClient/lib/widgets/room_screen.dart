import 'package:flutter/material.dart';
import 'package:modal_progress_hud_nsn/modal_progress_hud_nsn.dart';
import 'package:sync_watchers_client/widgets/user_name_screen.dart';

import '../web/requests.dart';

class RoomScreen extends StatefulWidget {
  const RoomScreen({super.key});

  @override
  State<RoomScreen> createState() => _RoomScreenState();
}

class _RoomScreenState extends State<RoomScreen> {
  bool _hasRequest = false;

  @override
  Widget build(BuildContext context) {
    final inviteController = TextEditingController();

    return ModalProgressHUD(
      inAsyncCall: _hasRequest,
      child: Scaffold(
        appBar: AppBar(title: const Text("Sync Watchers")),
        body: Padding(
          padding: const EdgeInsets.all(16.0),
          child: Row(
            children: [
              /// Left side: Create Room
              Expanded(
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    ElevatedButton(
                      onPressed: () {
                        setState(() {
                          _hasRequest = true;
                          createRoom(
                            callback: () => setState(() {
                              _hasRequest = false;
                            }),
                            onSuccess: (response) => Navigator.push(context, MaterialPageRoute(builder: (_) => UsernameScreen(inviteLink: response))),
                            onFail: (errorMessage) => print(errorMessage),
                          );
                        });
                      },
                      child: const Text("Create Room"),
                    ),
                  ],
                ),
              ),

              const SizedBox(width: 16),

              /// Right side: Join Room (TextField first, then button)
              Expanded(
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    TextField(
                      controller: inviteController,
                      decoration: const InputDecoration(hintText: "Invite Link", border: OutlineInputBorder()),
                    ),
                    const SizedBox(height: 8),
                    ElevatedButton(
                      onPressed: () {
                        final link = inviteController.text.trim();
                        if (link.isNotEmpty) {
                          Navigator.push(context, MaterialPageRoute(builder: (_) => UsernameScreen(inviteLink: "")));
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
      ),
    );
  }
}
