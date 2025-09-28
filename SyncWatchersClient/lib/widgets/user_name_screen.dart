import 'package:flutter/material.dart';
import 'package:modal_progress_hud_nsn/modal_progress_hud_nsn.dart';
import 'package:sync_watchers_client/widgets/video_screen.dart';

class UsernameScreen extends StatelessWidget {
  final String inviteLink;

  const UsernameScreen({super.key, required this.inviteLink});

  @override
  Widget build(BuildContext context) {
    final usernameController = TextEditingController();

    return ModalProgressHUD(
      inAsyncCall: false,
      child: Scaffold(
        // appBar: AppBar(title: const Text("Create Username")),
        appBar: AppBar(title: Text(inviteLink)),
        body: Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              TextField(
                controller: usernameController,
                decoration: const InputDecoration(hintText: "Enter username", border: OutlineInputBorder()),
              ),
              const SizedBox(height: 16),
              ElevatedButton(
                onPressed: () {
                  final username = usernameController.text.trim();
                  if (username.isNotEmpty) {
                    Navigator.push(context, MaterialPageRoute(builder: (_) => const VideoScreen(role: "owner")));
                  }
                },
                child: const Text("Create"),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
