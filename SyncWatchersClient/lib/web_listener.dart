abstract class WebListener {
  void onUploadContent(String userUUID);

  void onInvite(String userName);

  void onUpdateRole(String role);

  void onUserNameUpdate(String oldUserName, String newUserName);

  void onRoomDelete();

  void stop(String userName);

  void play(String userName);

  void rewind(int offsetInSecondsFromStart);

  void changeVideo(String videoName);
}
