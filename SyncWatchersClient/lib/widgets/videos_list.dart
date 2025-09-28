import 'package:flutter/material.dart';

class DownloadListWidget extends StatelessWidget {
  final List<String> items;

  const DownloadListWidget({super.key, required this.items});

  @override
  Widget build(BuildContext context) {
    return ListView.builder(
      shrinkWrap: true,
      itemCount: items.length,
      itemBuilder: (context, index) {
        return Padding(
          padding: const EdgeInsets.symmetric(vertical: 4.0, horizontal: 8.0),
          child: Row(
            children: [
              /// Read-only TextField
              Expanded(
                child: TextField(
                  controller: TextEditingController(text: items[index]),
                  readOnly: true,
                  decoration: const InputDecoration(
                    border: OutlineInputBorder(),
                  ),
                ),
              ),
              const SizedBox(width: 8),
              ElevatedButton(
                onPressed: () {
                  // TODO: Add download logic
                },
                child: const Text("Download"),
              ),
            ],
          ),
        );
      },
    );
  }
}
