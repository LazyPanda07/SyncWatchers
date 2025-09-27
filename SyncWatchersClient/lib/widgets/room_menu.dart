import 'package:flutter/material.dart';

class ControlMenuWidget extends StatelessWidget {
  const ControlMenuWidget({super.key});

  @override
  Widget build(BuildContext context) {
    return Container(
      color: Colors.yellow,
      padding: const EdgeInsets.all(12),
      child: Column(
        children: [
          TextField(
            decoration: const InputDecoration(
              labelText: "Invite Link",
              border: OutlineInputBorder(),
            ),
          ),
          const SizedBox(height: 8),
          ElevatedButton(
            onPressed: () {},
            child: const Text("Select Video"),
          ),
        ],
      ),
    );
  }
}