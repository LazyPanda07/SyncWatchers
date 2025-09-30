abstract class WebListener {
  Future<void> onUploadContent(String userUUID);

  Future<void> onInvite(String userName);

  Future<void> onUpdateRole(String role);

  Future<void> onUserNameUpdate(String oldUserName, String newUserName);

  Future<void> onRoomDelete();

  Future<void> stop(String userName);

  Future<void> play(String userName);

  Future<void> rewind(int offsetInSecondsFromStart);

  Future<void> changeVideo(String videoName);
}
