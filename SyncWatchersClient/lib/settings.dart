import 'dart:convert';
import 'dart:io';

import 'package:path_provider/path_provider.dart';

class Settings {
  static final Settings instance = Settings._internal();
  late final Map<String, dynamic> _settings;
  late String _userName;
  late String _roomUUID;

  Future<File> _getSettingsFile() async {
    File settings;
    const String settingsFileName = "sync_watchers_settings.json";

    if (Platform.isWindows) {
      settings = File(settingsFileName);
    } else if (Platform.isAndroid) {
      Directory dir = await getApplicationDocumentsDirectory();

      settings = File("${dir.path}/$settingsFileName");
    } else {
      throw UnimplementedError("Can't get path to settings file in current platform");
    }

    if (!settings.existsSync()) {
      settings.createSync();

      settings.writeAsStringSync(jsonEncode({"host": "127.0.0.1"}));
    }

    return settings;
  }

  Future<void> _updateSaveFile() async {
    File settingsFile = await _getSettingsFile();

    await settingsFile.writeAsString(jsonEncode(_settings));
  }

  Settings._internal() {
    _getSettingsFile().then((settingsFile) {
      _settings = jsonDecode(settingsFile.readAsStringSync());

      print(jsonEncode(_settings));
    });
  }

  set userName(String userName) => _userName = userName;

  String get userName => _userName;

  set roomUUID(String roomUUID) => _roomUUID = roomUUID;

  String get roomUUID => _roomUUID;

  Future<void> updateSettings(Map<String, dynamic> settings) async {
    settings.forEach((String key, dynamic value) => _settings[key] = value);

    await _updateSaveFile();
  }

  dynamic operator [](String key) => _settings[key];
}
