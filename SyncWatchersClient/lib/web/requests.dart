import 'dart:convert';

import 'package:flutter/services.dart';
import 'package:http/http.dart' as http;

Future<void> createRoom({required Function() callback, required Function(String response) onSuccess, required Function(String errorMessage) onFail}) async {
  try {
    final Uri url = Uri.parse("http://127.0.0.1:52000/rooms");

    http.Response response = await http.post(url, headers: {"Content-Type": "application/json"}, body: jsonEncode({"name": "NewRoom"}));

    if (response.statusCode >= 400) {
      onFail(response.body);
    } else {
      onSuccess(response.body);
    }
  } catch (e) {
    print(e);
  }

  callback();
}
