import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

class ControlMenuWidget extends StatelessWidget {
  final String inviteLink;
  final VoidCallback onSelectVideo;

  const ControlMenuWidget({super.key, required this.inviteLink, required this.onSelectVideo});

  @override
  Widget build(BuildContext context) {
    final controller = TextEditingController(text: inviteLink);

    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        const Text("Invite Link:", style: TextStyle(fontWeight: FontWeight.bold)),
        Row(
          children: [
            Expanded(
              child: TextField(
                controller: controller,
                readOnly: true,
                decoration: const InputDecoration(border: OutlineInputBorder()),
              ),
            ),
            const SizedBox(width: 8),
            IconButton(
              icon: const Icon(Icons.copy),
              tooltip: "Copy invite link",
              onPressed: () {
                Clipboard.setData(ClipboardData(text: inviteLink));
                ScaffoldMessenger.of(context).showSnackBar(const SnackBar(content: Text("Copied to clipboard")));
              },
            ),
          ],
        ),
        const SizedBox(height: 16),
      ],
    );
  }
}
