from typing import List

import requests
import unittest


def get_room_base_url():
    return "http://127.0.0.1:50200/rooms"


def get_users_base_url(uuid: str):
    return f"http://127.0.0.1:50200/users/{uuid}"


class CoreFunctionality(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.first_user_name = "first"

    def test_create_room(self):
        room_name = {
            "roomName": "test_room"
        }

        result = requests.post(get_room_base_url(), json=room_name)

        self.assertEqual(result.status_code, 200)

        self.room_name = room_name["roomName"]
        self.room_uuid = result.json()["roomUUID"]

    def test_create_user(self):
        user_data = {
            "name": self.first_user_name,
            "roomUUID": self.room_uuid
        }

        result = requests.put(get_room_base_url(), json=user_data)

        self.assertEqual(result.status_code, 200)

        self.first_user_uuid = result.json()["uuid"]

    def test_get_room(self):
        room = {
            "uuid": self.room_uuid
        }

        result = requests.get(get_room_base_url(), json=room)

        self.assertEqual(result.status_code, 200)
        self.assertEqual(result.json()["roomName"], self.room_name)

    def test_join_room(self):
        room_data = {
            "roomUUID": self.room_uuid,
        }

        result = requests.put(get_users_base_url(self.first_user_uuid), json=room_data)

        self.assertEqual(result.status_code, 200)

    def test_room_users(self):
        result = requests.get(get_users_base_url(self.first_user_uuid))

        self.assertEqual(result.status_code, 200)

        users: List[str] = result.json()["users"]

        self.assertEqual(len(users), 1)
        self.assertEqual(users[0], self.first_user_name)


if __name__ == '__main__':
    unittest.main()
