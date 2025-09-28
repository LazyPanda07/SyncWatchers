import 'package:flutter/material.dart';
import 'package:sync_watchers_client/widgets/user_name_screen.dart';

class RoomScreen extends StatelessWidget {
  const RoomScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final inviteController = TextEditingController();

    return Scaffold(
      appBar: AppBar(title: const Text("Home")),
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
                      Navigator.push(
                        context,
                        MaterialPageRoute(builder: (_) => const UsernameScreen()),
                      );
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
                    decoration: const InputDecoration(
                      hintText: "Invite Link",
                      border: OutlineInputBorder(),
                    ),
                  ),
                  const SizedBox(height: 8),
                  ElevatedButton(
                    onPressed: () {
                      final link = inviteController.text.trim();
                      if (link.isNotEmpty) {
                        Navigator.push(
                          context,
                          MaterialPageRoute(builder: (_) => const UsernameScreen()),
                        );
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
