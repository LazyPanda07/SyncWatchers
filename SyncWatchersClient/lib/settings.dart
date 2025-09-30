import 'dart:convert';
import 'dart:io';

class Settings {
  static final Settings instance = Settings._internal();
  late final Map<String, dynamic> _settings;

  Settings._internal() {
    File settings = File("settings.json");

    if (!settings.existsSync()) {
      settings.createSync();

      settings.writeAsStringSync(jsonEncode({"host": "26.55.125.90"}));
    }

    _settings = jsonDecode(settings.readAsStringSync());
  }

  dynamic operator [](String key) => _settings[key];

  void log() {
    print(jsonEncode(_settings));
  }
}
