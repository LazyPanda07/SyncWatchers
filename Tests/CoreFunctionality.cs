using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Net.Http.Json;

[assembly: CollectionBehavior(DisableTestParallelization = true)]

namespace Tests
{
	public class DataFixture
	{
		public string FirstUserUuid { get; set; }
		public string RoomUuid { get; set; }

		public DataFixture()
		{
			FirstUserUuid = "";
			RoomUuid = "";
		}
	}

	[TestCaseOrderer("Tests.PriorityOrderer", "Tests")]
	public class CoreFunctionality(DataFixture dataFixture) : IClassFixture<DataFixture>
	{
		private readonly DataFixture _dataFixture = dataFixture;
		private static readonly HttpClient _client = new();
		private readonly string _firstUserName = "user";
		private readonly string _roomName = "test_room";

		private static string GetRoomBaseUrl()
		{
			return "http://127.0.0.1:50200/rooms";
		}

		private static string GetUsersBaseUrl(string uuid)
		{
			return $"http://127.0.0.1:50200/users/{uuid}";
		}

		private static T GetWithAssertion<T>(T? value)
		{
			Assert.NotNull(value);

			return value!;
		}

		[Fact]
		[TestPriority(0)]
		public async Task CreateRoom()
		{
			var data = new
			{
				roomName = _roomName
			};

			using HttpResponseMessage response = await _client.PostAsJsonAsync(GetRoomBaseUrl(), data);

			Assert.Equal(System.Net.HttpStatusCode.OK, response.StatusCode);

			JToken result = GetWithAssertion(JsonConvert.DeserializeObject<JToken>(await response.Content.ReadAsStringAsync()));

			_dataFixture.RoomUuid = GetWithAssertion(GetWithAssertion(result["roomUUID"]).Value<string>());
		}

		[Fact]
		[TestPriority(1)]
		public async Task CreateUser()
		{
			var userData = new
			{
				name = _firstUserName,
				roomUUID = _dataFixture.RoomUuid
			};

			using HttpResponseMessage response = await _client.PutAsJsonAsync(GetRoomBaseUrl(), userData);

			Assert.Equal(System.Net.HttpStatusCode.OK, response.StatusCode);

			JToken result = GetWithAssertion(JsonConvert.DeserializeObject<JToken>(await response.Content.ReadAsStringAsync()));

			_dataFixture.FirstUserUuid = GetWithAssertion(GetWithAssertion(result["uuid"]).Value<string>());
		}

		[Fact]
		[TestPriority(2)]
		public async Task GetRoom()
		{
			using HttpResponseMessage response = await _client.GetAsync($"{GetRoomBaseUrl()}?uuid={_dataFixture.RoomUuid}");

			Assert.Equal(System.Net.HttpStatusCode.OK, response.StatusCode);

			JToken result = GetWithAssertion(JsonConvert.DeserializeObject<JToken>(await response.Content.ReadAsStringAsync()));

			Assert.Equal(_roomName, GetWithAssertion(GetWithAssertion(result["roomName"]).Value<string>()));
		}

		[Fact]
		[TestPriority(3)]
		public async Task JoinRoom()
		{
			var roomData = new
			{
				roomUUID = _dataFixture.RoomUuid
			};

			using HttpResponseMessage response = await _client.PutAsJsonAsync(GetUsersBaseUrl(_dataFixture.FirstUserUuid), roomData);

			Assert.Equal(System.Net.HttpStatusCode.OK, response.StatusCode);
		}

		[Fact]
		[TestPriority(4)]
		public async Task RoomUsers()
		{
			using HttpResponseMessage response = await _client.GetAsync(GetUsersBaseUrl(_dataFixture.FirstUserUuid));

			Assert.Equal(System.Net.HttpStatusCode.OK, response.StatusCode);

			string data = await response.Content.ReadAsStringAsync();

			JToken result = GetWithAssertion(JsonConvert.DeserializeObject<JToken>(data));

			JArray users = GetWithAssertion(GetWithAssertion(result["users"]).Value<JArray>());

			Assert.Single(users);

			Assert.Equal(_firstUserName, GetWithAssertion(users[0].Value<string>()));
		}
	}
}
