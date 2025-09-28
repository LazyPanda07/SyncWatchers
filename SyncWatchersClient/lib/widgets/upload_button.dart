import 'package:flutter/material.dart';
import 'package:file_picker/file_picker.dart';

class UploadButtonWidget extends StatelessWidget {
  final void Function(String path)? onFileSelected;

  const UploadButtonWidget({super.key, this.onFileSelected});

  Future<void> _pickFile(BuildContext context) async {
    final result = await FilePicker.platform.pickFiles(type: FileType.custom, allowedExtensions: ['mp4', 'mov', 'avi']);

    if (result != null && result.files.isNotEmpty) {
      final filePath = result.files.single.path;
      if (filePath != null) {
        onFileSelected?.call(filePath);

        ScaffoldMessenger.of(context).showSnackBar(SnackBar(content: Text("Selected: ${result.files.single.name}")));
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    return ElevatedButton.icon(onPressed: () => _pickFile(context), icon: const Icon(Icons.upload_file), label: const Text("Upload Video"));
  }
}
