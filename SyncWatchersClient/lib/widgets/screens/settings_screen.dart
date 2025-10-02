import 'package:flutter/material.dart';
import 'package:sync_watchers_client/settings.dart';

class SettingsScreen extends StatefulWidget {
  const SettingsScreen({super.key});

  @override
  State<StatefulWidget> createState() => _SettingsState();
}

class _SettingsState extends State<SettingsScreen> {
  final TextEditingController _hostController = TextEditingController();

  Future<void> _saveSettings() async {
    Map<String, dynamic> updatedSettings = {"host": _hostController.text.trim()};

    await Settings.instance.updateSettings(updatedSettings);

    setState(() {
      Navigator.pop(context);
    });
  }

  @override
  void initState() {
    super.initState();

    _hostController.text = Settings.instance["host"];
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text("Settings")),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text("Host URL:", style: TextStyle(fontSize: 16)),
            const SizedBox(height: 8),
            TextField(
              controller: _hostController,
              decoration: const InputDecoration(border: OutlineInputBorder(), hintText: "Enter host (e.g. https://myserver.com or ip address)"),
            ),
            const SizedBox(height: 20),
            ElevatedButton(onPressed: _saveSettings, child: const Text("Save")),
          ],
        ),
      ),
    );
  }
}
