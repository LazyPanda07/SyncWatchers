import 'dart:convert';
import 'dart:io';

import 'package:http/http.dart' as http;
import 'package:path_provider/path_provider.dart';

import '../settings.dart';

Future<void> _makeRequest({
  required Future<http.Response> Function(Uri url, {Map<String, String>? headers, Object? body, Encoding? encoding}) method,
  required Uri url,
  Map<String, String>? headers,
  required Object? body,
  required Function(String response) onSuccess,
  required Function(String errorMessage) onFail,
}) async {
  try {
    final http.Response response = await method(url, headers: headers, body: body);

    if (response.statusCode >= 400) {
      onFail(response.body);
    } else {
      onSuccess(response.body);
    }
  } catch (e) {
    onFail(e.toString());
  }
}

Future<void> _makeGetRequest({
  required Future<http.Response> Function(Uri url, {Map<String, String>? headers}) method,
  required Uri url,
  required Function(String response) onSuccess,
  required Function(String errorMessage) onFail,
}) async {
  try {
    final http.Response response = await method(url);

    if (response.statusCode >= 400) {
      onFail(response.body);
    } else {
      onSuccess(response.body);
    }
  } catch (e) {
    onFail(e.toString());
  }
}

Future<void> createRoom(Function(String response) onSuccess, Function(String errorMessage) onFail, Map<String, String> data) async => _makeRequest(
  method: http.post,
  url: Uri.parse("http://${Settings.instance["host"]}:52000/rooms"),
  headers: {"Content-Type": "application/json"},
  body: jsonEncode({"name": data["roomName"]}),
  onSuccess: onSuccess,
  onFail: onFail,
);

Future<void> joinRoom(Function(String response) onSuccess, Function(String errorMessage) onFail, Map<String, String> data) async => _makeRequest(
  method: http.put,
  url: Uri.parse(data["inviteLink"]!),
  headers: {"Content-Type": "application/json"},
  body: jsonEncode({"role": data["role"]}),
  onSuccess: onSuccess,
  onFail: onFail,
);

Future<void> updateUserName(Function(String response) onSuccess, Function(String errorMessage) onFail, Map<String, String> data) async => _makeRequest(
  method: http.patch,
  url: Uri.parse("http://${Settings.instance["host"]}:52000/users"),
  headers: {"Content-Type": "application/json"},
  body: jsonEncode({"userUUID": data["userUUID"], "newUserName": data["newUserName"]}),
  onSuccess: onSuccess,
  onFail: onFail,
);

Future<void> getRoomInformation(Function(String response) onSuccess, Function(String errorMessage) onFail, Map<String, String> data) async =>
    _makeGetRequest(method: http.get, url: Uri.parse("http://${Settings.instance["host"]}:52000/users?user_uuid=${data["userUUID"]}"), onSuccess: onSuccess, onFail: onFail);

Future<void> uploadContent(Function(String response) onSuccess, Function(String errorMessage) onFail, Map<String, String> data, String filePath) async {
  try {
    final File file = File(filePath);
    final int length = await file.length();

    final HttpClient client = HttpClient()..maxConnectionsPerHost = 1;
    final HttpClientRequest request = await client.putUrl(
      Uri.parse("http://${Settings.instance["host"]}:52000/upload/${data["roomUUID"]}/${data["userUUID"]}/${data["contentName"]}"),
    );

    request.headers.contentType = ContentType.binary;
    request.contentLength = length;

    await for (final chunk in file.openRead()) {
      request.add(chunk);
    }

    await request.flush();

    final HttpClientResponse response = await request.close();
    final String body = await utf8.decodeStream(response);

    if (response.statusCode >= 400) {
      onFail(body);
    } else {
      onSuccess(body);
    }
  } catch (e) {
    onFail(e.toString());
  }
}

Future<void> downloadContent(Function(String response) onSuccess, Function(String errorMessage) onFail, Map<String, String> data) async {
  try {
    final String contentName = data["contentName"]!;
    final request = await HttpClient().getUrl(Uri.parse("http://${Settings.instance["host"]}:52000/download/${data["roomUUID"]}/$contentName"));
    final response = await request.close();

    if (response.statusCode == 200) {
      Directory temporaryDirectory = await getTemporaryDirectory();

      final file = File("${temporaryDirectory.path}/$contentName");
      final sink = file.openWrite();

      await response.pipe(sink);

      await sink.close();

      onSuccess("Download completed: $contentName");
    } else {
      onFail("Failed to download file: $contentName");
    }
  } catch (e) {
    onFail(e.toString());
  }
}

Future<void> changeVideoRequest(Function(String response) onSuccess, Function(String errorMessage) onFail, Map<String, String> data) async => _makeRequest(
  method: http.post,
  url: Uri.parse("http://${Settings.instance["host"]}:52000/video/change_video"),
  headers: {"Content-Type": "application/json"},
  body: jsonEncode({"roomUUID": data["roomUUID"], "videoName": data["videoName"]}),
  onSuccess: onSuccess,
  onFail: onFail,
);
