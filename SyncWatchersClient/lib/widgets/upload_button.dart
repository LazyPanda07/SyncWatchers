import 'package:flutter/material.dart';
import 'package:file_picker/file_picker.dart';

import '../web/requests.dart';

class UploadButtonWidget extends StatefulWidget {
  final Map<String, dynamic> responseData;

  const UploadButtonWidget({super.key, required this.responseData});

  @override
  State<UploadButtonWidget> createState() => _UploadButtonState();
}

class _UploadButtonState extends State<UploadButtonWidget> {
  late ScaffoldMessengerState _messenger;

  Future<void> _uploadContent(String contentName, String filePath) {
    return uploadContent(
          (String response) {
        _messenger.hideCurrentSnackBar();

        _messenger.showSnackBar(SnackBar(content: Text(response)));
      },
          (String errorMessage) {
        _messenger.hideCurrentSnackBar();

        _messenger.showSnackBar(SnackBar(content: Text(errorMessage)));
      },
      {"roomUUID": widget.responseData["roomUUID"], "userUUID": widget.responseData["userUUID"], "contentName": contentName},
      filePath,
    );
  }

  Future<void> _pickFile(BuildContext context) async {
    final result = await FilePicker.platform.pickFiles(type: FileType.custom, allowedExtensions: ['mp4', 'mov', 'avi']);

    if (result != null && result.files.isNotEmpty) {
      final filePath = result.files.single.path;
      if (filePath != null) {
        _messenger.showSnackBar(SnackBar(content: Text("Uploading: ${result.files.single.name}...")));

        _uploadContent(result.files.single.name, filePath);
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    _messenger = ScaffoldMessenger.of(context);

    return ElevatedButton.icon(onPressed: () => _pickFile(context), icon: const Icon(Icons.upload_file), label: const Text("Upload Video"));
  }
}
