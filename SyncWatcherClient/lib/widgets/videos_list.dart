import 'package:flutter/material.dart';

/// Bottom right: List with text fields and download buttons
class DownloadListWidget extends StatelessWidget {
  const DownloadListWidget({super.key});

  @override
  Widget build(BuildContext context) {
    return Expanded(
      child: Container(
        color: Colors.green,
        child: ListView.builder(
          itemCount: 5, // sample list
          itemBuilder: (context, index) {
            return ListTile(
              title: TextField(
                decoration: InputDecoration(
                  labelText: "Item ${index + 1}",
                  border: const OutlineInputBorder(),
                ),
              ),
              trailing: ElevatedButton(
                onPressed: () {},
                child: const Text("Download"),
              ),
            );
          },
        ),
      ),
    );
  }
}