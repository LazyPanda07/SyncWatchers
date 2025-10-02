import 'dart:convert';
import 'dart:io';

class Settings {
  static final Settings instance = Settings._internal();
  late final Map<String, dynamic> _settings;
  late String _userName;
  late String _roomUUID;

  Settings._internal() {
    if (Platform.isWindows) {
      File settings = File("settings.json");

      if (!settings.existsSync()) {
        settings.createSync();

        settings.writeAsStringSync(jsonEncode({"host": "127.0.0.1"}));
      }

      _settings = jsonDecode(settings.readAsStringSync());
    } else {
      // TODO: settings file

      _settings = {"host": "127.0.0.1"};
    }
  }

  set userName(String userName) => _userName = userName;

  String get userName => _userName;

  set roomUUID(String roomUUID) => _roomUUID = roomUUID;

  String get roomUUID => _roomUUID;

  void log() {
    print(jsonEncode(_settings));
  }

  dynamic operator [](String key) => _settings[key];
}
