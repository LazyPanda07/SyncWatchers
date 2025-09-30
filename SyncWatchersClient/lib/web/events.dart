import 'dart:convert';
import 'dart:io';
import 'dart:typed_data';

import 'package:sync_watchers_client/web_listener.dart';

enum _Events { onUploadContent, onInvite, onUpdateRole, onUserNameUpdate, onRoomDelete, stop, play, rewind, changeVideo }

enum _TypeFromBytes { int32, uint32 }

class EventsHandler {
  static final EventsHandler instance = EventsHandler._internal();
  final List<WebListener> _listeners = [];

  EventsHandler._internal();

  static int _bytesToInt(Uint8List bytes, {_TypeFromBytes type = _TypeFromBytes.int32}) {
    switch (type) {
      case _TypeFromBytes.int32:
        return ByteData.sublistView(bytes).getInt32(0, Endian.little);

      case _TypeFromBytes.uint32:
        return ByteData.sublistView(bytes).getUint32(0, Endian.little);
    }
  }

  static int _calculateEventSize(Uint8List data) {
    Uint8List lengthBytes = data.sublist(1, 5);

    return 1 + 4 + EventsHandler._bytesToInt(lengthBytes); // event + int + length
  }

  void _parseEvents(BytesBuilder buffer) {
    while (true) {
      final Uint8List bytes = buffer.toBytes();

      if (bytes.isEmpty) {
        return;
      }

      final _Events eventType = _Events.values[bytes[0]];
      int requiredLength = 0;

      switch (eventType) {
        case _Events.onUploadContent:
          requiredLength = 1 + 36; // event + UUID

          break;

        case _Events.onInvite:
        case _Events.onUpdateRole:
        case _Events.onUserNameUpdate:
        case _Events.stop:
        case _Events.play:
        case _Events.changeVideo:
          // event + int
          if (bytes.length < 5) {
            return;
          }

          requiredLength = EventsHandler._calculateEventSize(bytes);

          break;

        case _Events.onRoomDelete:
          requiredLength = 1; // event

          break;

        case _Events.rewind:
          requiredLength = 5; // event + uint

          break;
      }

      if (bytes.length < requiredLength) {
        return;
      }

      final Uint8List message = bytes.sublist(0, requiredLength);

      _handleEvent(eventType, message.sublist(1)); // skip event byte

      final Uint8List remaining = bytes.sublist(requiredLength);

      buffer.clear();

      buffer.add(remaining);
    }
  }

  void _handleEvent(_Events eventType, Uint8List payload) {
    for (WebListener listener in _listeners) {
      switch (eventType) {
        case _Events.onUploadContent:
          listener.onUploadContent(utf8.decode(payload));

          break;

        case _Events.onInvite:
          listener.onInvite(utf8.decode(Uint8List.view(payload.buffer, 4)));

          break;

        case _Events.onUpdateRole:
          listener.onUpdateRole(utf8.decode(Uint8List.view(payload.buffer, 4)));

          break;

        case _Events.onUserNameUpdate:
          String temp = utf8.decode(Uint8List.view(payload.buffer, 4));
          List<String> userNames = temp.split(' ');

          listener.onUserNameUpdate(userNames[0], userNames[1]);

          break;

        case _Events.onRoomDelete:
          listener.onRoomDelete();

          break;

        case _Events.stop:
          listener.stop(utf8.decode(Uint8List.view(payload.buffer, 4)));

          break;

        case _Events.play:
          listener.play(utf8.decode(Uint8List.view(payload.buffer, 4)));

          break;

        case _Events.rewind:
          listener.rewind(EventsHandler._bytesToInt(payload, type: _TypeFromBytes.uint32));

          break;

        case _Events.changeVideo:
          listener.changeVideo(utf8.decode(Uint8List.view(payload.buffer, 4)));

          break;
      }
    }
  }

  Future<void> _eventLoop(String roomUUID) async {
    final Socket socket = await Socket.connect("127.0.0.1", 52001);

    socket.write(roomUUID);
    socket.flush();

    final BytesBuilder buffer = BytesBuilder();

    socket.listen(
      (List<int> data) {
        buffer.add(data);

        _parseEvents(buffer);
      },
      onError: (error) => print("Socket error: $error"),
      onDone: () => socket.close(),
    );

    await socket.done;
  }

  void startEventLoop(String roomUUID) {
    _eventLoop(roomUUID);
  }

  void addListener(WebListener listener) {
    _listeners.add(listener);
  }
}
