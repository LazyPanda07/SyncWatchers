import 'dart:convert';

import 'package:http/http.dart' as http;

Future<void> _makeRequest({
  required Future<http.Response> Function(Uri url, {Map<String, String>? headers, Object? body, Encoding? encoding}) method,
  required Uri url,
  required Map<String, String>? headers,
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

Future<void> createRoom(Function(String response) onSuccess, Function(String errorMessage) onFail, Map<String, String> data) async => _makeRequest(
  method: http.post,
  url: Uri.parse("http://127.0.0.1:52000/rooms"),
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

Future<void> updateUserName(Function(String response) onSuccess, Function(String errorMessage) onFail) async => _makeRequest(
  method: http.post,
  url: Uri.parse("http://127.0.0.1:52000/rooms"),
  headers: {"Content-Type": "application/json"},
  body: jsonEncode({"name": "NewRoom"}),
  onSuccess: onSuccess,
  onFail: onFail,
);
