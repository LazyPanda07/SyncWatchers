import 'package:flutter/material.dart';
import 'package:sync_watchers_client/settings.dart';
import 'package:sync_watchers_client/web/events.dart';
import 'package:sync_watchers_client/web/requests.dart';
import 'package:sync_watchers_client/widgets/screens/video_screen.dart';

class UsernameScreen extends StatefulWidget {
  final Map<String, dynamic> responseData;

  const UsernameScreen({super.key, required this.responseData});

  @override
  State<UsernameScreen> createState() => _UsernameScreenState();
}

class _UsernameScreenState extends State<UsernameScreen> {
  final TextEditingController _userNameController = TextEditingController();
  late ScaffoldMessengerState _messenger;
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

  @override
  void initState() {
    super.initState();

    Settings.instance.userName = widget.responseData["userName"];
    Settings.instance.roomUUID = widget.responseData["roomUUID"];

    _userNameController.text = Settings.instance.userName;

    EventsHandler.instance.startEventLoop(Settings.instance.roomUUID);
  }

  Future<void> _updateUserName() async {
    _showSnackBar(SnackBar(content: Text("Update user name..."), onVisible: () async => await _updateUserNameRequest()));
  }

  Future<void> _updateUserNameRequest() async {
    await getRoomInformation((String) {}, (String) {}, {"userUUID": widget.responseData["userUUID"]});

    return updateUserName(
      (String response) {
        Settings.instance.userName = _userNameController.text;

        widget.responseData["userName"] = Settings.instance.userName;

        Navigator.push(context, MaterialPageRoute(builder: (_) => VideoScreen(responseData: widget.responseData)));
      },
      (String errorMessage) {
        _showSnackBar(SnackBar(content: Text(errorMessage)));
      },
      {"userUUID": widget.responseData["userUUID"], "newUserName": _userNameController.text},
    );
  }

  @override
  Widget build(BuildContext context) {
    _messenger = ScaffoldMessenger.of(context);

    return Scaffold(
      appBar: AppBar(title: const Text("Create Username")),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            TextField(
              controller: _userNameController,
              decoration: const InputDecoration(hintText: "Enter user name", border: OutlineInputBorder()),
            ),
            const SizedBox(height: 16),
            ElevatedButton(
              onPressed: () async {
                if (_currentSnackBar == null && _userNameController.text.isNotEmpty) {
                  _updateUserName();
                }
              },
              child: const Text("Create"),
            ),
          ],
        ),
      ),
    );
  }
}
