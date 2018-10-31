using System.Collections.Generic;
using NUnit.Framework;
using huestream;
using System.Linq;
using System;
using System.IO;
using System.Text;
using System.Threading;
using Newtonsoft.Json.Linq;

namespace huestream_tests
{
    public class TestBase
    {
        protected const String APPLICATION_NAME = "CSharpIntegrationTests";
        protected const String PERSISTENCE_LOCATION = "bridge.json";

        private String IPv4_ADDRESS = "192.168.1.51";
        private String BRIDGE_ID = "001788fffe1ffd08";
        private String TCP_PORT = "60202";
        private String UDP_PORT = "60202";
        private String SSL_PORT = "61202";
        
        protected const int CONNECTION_TIMEOUT_MS = 3000;
        protected const int DISCOVERY_TIMEOUT_MS = 15000;
        protected const int LIGHTS_COUNT = 4;

        protected FeedbackMessageHandler _message_handler;
        protected HueStream _hue_stream;
        protected Bridge _bridge;
        protected IBridgeWrapper _bridgeWrapperHelper;

        protected Light _frontLeftLight;
        protected Light _frontRightLight;
        protected Light _rearLeftLight;
        protected Light _rearRightLight;
        protected List<Light> _allLights;

        public TestBase()
        {
            if (TestContext.Parameters.Count > 0)
            {
                IPv4_ADDRESS = TestContext.Parameters["hue_ip"];
                BRIDGE_ID = TestContext.Parameters["hue_bridge_id"];
                UDP_PORT = TestContext.Parameters["hue_streaming_port"];
                SSL_PORT = TestContext.Parameters["hue_https_port"];
                TCP_PORT = TestContext.Parameters["hue_http_port"];
            }
        }

        public Bridge InitializeBridge()
        {
            if (GetUser().Length == 0 || GetClientKey().Length == 0)
            {
                CreateUser();
            }

            Bridge result = new Bridge(BRIDGE_ID, IPv4_ADDRESS, true, new BridgeSettings());

            result.SetUser(GetUser());
            result.SetClientKey(GetClientKey());
            result.SetTcpPort(TCP_PORT);
            result.SetSslPort(SSL_PORT);
            result.EnableSsl();
            
            return result;
        }

        public IBridgeWrapper InitializeBridgeWrapper()
        {
            if (GetUser().Length == 0 || GetClientKey().Length == 0)
            {
                CreateUser();
            }

            BridgeWrapperBuilder builder = new BridgeWrapperBuilder();
            builder.WithUserName(GetUser())
                   .WithClientKey(GetClientKey())
                   .WithIPv4Address(IPv4_ADDRESS)
                   .WithTcpPort(TCP_PORT)
                   .WithBridgeId(BRIDGE_ID);

            return builder.Build();
        }
        
        protected void InitializeBridgeResources()
        {
            int entertainmentGroupId = _bridgeWrapperHelper.GetEntertainmentGroupId();
            List<ILightID> lights = _bridgeWrapperHelper.GetLLCLightsIDs();

            Assert.IsTrue(lights.Count >= LIGHTS_COUNT);
            if (lights.Count > LIGHTS_COUNT)
            {
                lights = lights.GetRange(0, LIGHTS_COUNT);
            }

            InitializeLights(lights, entertainmentGroupId);
            _bridge.SelectGroup(entertainmentGroupId.ToString());
        }

        private void InitializeLights(List<ILightID> lights, int entertainmentGroupId)
        {
            Assert.AreEqual(LIGHTS_COUNT, lights.Count, "Amount of lights is not equal to LIGHTS_COUNT");

            _frontLeftLight = new Light(Light.Position.FrontLeft, lights[0]);
            _frontRightLight = new Light(Light.Position.FrontRight, lights[1]);
            _rearLeftLight = new Light(Light.Position.RearLeft, lights[2]);
            _rearRightLight = new Light(Light.Position.RearRight, lights[3]);

            _allLights = new List<Light>
            {
                _frontRightLight,
                _frontLeftLight,
                _rearRightLight,
                _rearLeftLight
            };
            _bridgeWrapperHelper.IncludeLightsIntoGroup(lights, entertainmentGroupId);
            _bridgeWrapperHelper.SetLightsCoordinates(entertainmentGroupId, LightsAsLightCoordinates());
        }

        private List<ILightCoordinate> LightsAsLightCoordinates()
        {
            return _allLights.Select(x => x.AsLightCoordinate()).ToList();
        }

        protected HueStream CreateStream(StreamingMode streamingMode)
        {
            StreamSettings streamSettings = new StreamSettings();
            streamSettings.SetStreamingPort(int.Parse(UDP_PORT));

            Config config = new Config(APPLICATION_NAME, Environment.OSVersion.Platform.ToString(), new PersistenceEncryptionKey("encryption_key"));
            config.SetStreamSettings(streamSettings);
            config.SetStreamingMode(streamingMode);
            return new HueStream(config);
        }

        protected void CleanupUser()
        {
            if (_user == "")
            {
                return;
            }
            
            _user = "";
            _clientKey = "";
            _bridgeWrapperHelper.CleanUpUser();
        }

        private String GetUser()
        {
            return _user;
        }

        private String GetClientKey()
        {
            return _clientKey;
        }
        
        protected void CreateUser()
        {
            PushLink();

            StringBuilder urlBuilder = new StringBuilder();
            urlBuilder.Append("http://")
                    .Append(IPv4_ADDRESS)
                    .Append(":")
                    .Append(TCP_PORT)
                    .Append("/api");

            String REQUEST_BODY = "{\"devicetype\": \"" + APPLICATION_NAME + "\", \"generateclientkey\":true}";

            JArray response = Network.PerformUpdateRequest(urlBuilder.ToString(), REQUEST_BODY, Network.UPDATE_REQUEST.POST);
            Assert.NotNull(response, "Push link response is null");

            JToken responseRoot = response[0];
            Assert.NotNull(responseRoot, "Wrong structure of incoming response");

            var successNode = responseRoot["success"];
            Assert.NotNull(successNode, "Creating group was not successful");

            _user = (string)successNode["username"];
            Assert.NotNull(_user, "User name is null");
            Assert.False(_user.Length == 0, "User name is empty");

            _clientKey = (string)successNode["clientkey"];
            Assert.NotNull(_clientKey, "Client key is null");
            Assert.False(_clientKey.Length == 0, "Client key is empty");
        }

        protected void PushLink(bool enable = true)
        {
            StringBuilder urlBuilder = new StringBuilder();
            urlBuilder.Append("http://")
                    .Append(IPv4_ADDRESS)
                    .Append(":")
                    .Append(TCP_PORT)
                    .Append("/api/0/config");

            String REQUEST_BODY = "{\"linkbutton\":" + (enable ? "true" : "false") + "}";

            JArray response = Network.PerformUpdateRequest(urlBuilder.ToString(), REQUEST_BODY, Network.UPDATE_REQUEST.PUT);
            Assert.NotNull(response, "Push link response is null");

            JToken responseRoot = response[0];
            Assert.NotNull(responseRoot, "Wrong structure of incoming response");

            var successNode = responseRoot["success"];
            Assert.NotNull(successNode, "Push link was not successful");
        }

        protected void ClearPersistenceData()
        {
            if (File.Exists(PERSISTENCE_LOCATION))
            {
                File.Delete(PERSISTENCE_LOCATION);
            }
        }

        protected WaitHandle GetWaitHandleForMessage(FeedbackMessage.Id message)
        {
            var messageObserver = new FeedbackMessageObserver();

            var waitHandle = new AutoResetEvent(false);
            messageObserver.SetEventWaitHandle(waitHandle);
            _hue_stream.RegisterFeedbackHandler(messageObserver);

            messageObserver.StartListening(message);

            return waitHandle;
        }


        private String _user = "";
        private String _clientKey = "";
    }
}
