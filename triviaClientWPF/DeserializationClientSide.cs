using System;
using System.Collections.Generic;
using System.Text;
using Newtonsoft.Json;

namespace triviaClientWPF.Response
{
    public static class DeserializationClientSide
    {
        public static T DeserializeMessage<T>(byte[] message)
        {
            if (message.Length < 5)
            {
                throw new ArgumentException("Message is too short.");
            }

            byte messageCode = message[0];
            List<byte> buffer = new List<byte>();
            int len = message.Length;
            buffer.Add((byte)((len >> 24) & 0xFF));
            buffer.Add((byte)((len >> 16) & 0xFF));
            buffer.Add((byte)((len >> 8) & 0xFF));
            buffer.Add((byte)(len & 0xFF));
            string jsonString = Encoding.UTF8.GetString(message, 5, len - 5);

            object response;
            switch (messageCode)
            {
                case (byte)ROOM_RESPONSE_CODES.loginCode:
                    response = DeserializeLoginResponse(jsonString);
                    break;
                case (byte)ROOM_RESPONSE_CODES.signupCode:
                    response = DeserializeSignupResponse(jsonString);
                    break;
                case (byte)ROOM_RESPONSE_CODES.errorCode:
                    response = DeserializeErrorResponse(jsonString);
                    break;
                case (byte)ROOM_RESPONSE_CODES.logoutCode:
                    response = DeserializeLogoutResponse(jsonString);
                    break;
                case (byte)ROOM_RESPONSE_CODES.joinRoomCode:
                    response = DeserializeJoinRoomResponse(jsonString);
                    break;
                case (byte)ROOM_RESPONSE_CODES.createRoomCode:
                    response = DeserializeCreateRoomResponse(jsonString);
                    break;
                case (byte)ROOM_RESPONSE_CODES.getRoomsCode:
                    response = DeserializeGetRoomsResponse(jsonString);
                    break;
                case (byte)ROOM_RESPONSE_CODES.getPlayersCode:
                    response = DeserializeGetPlayersInRoomResponse(jsonString);
                    break;
                case (byte)ROOM_RESPONSE_CODES.getHighScoreCode:
                    response = DeserializeGetHighScoreResponse(jsonString);
                    break;
                case (byte)ROOM_RESPONSE_CODES.getPersonalStatsCode:
                    response = DeserializeGetPersonalStatsResponse(jsonString);
                    break;
                case (byte)ROOM_RESPONSE_CODES.closeRoomCode:
                    response = DeserializeCloseRoomResponse(jsonString);
                    break;
                case (byte)ROOM_RESPONSE_CODES.startGameCode:
                    response = DeserializeStartGameResponse(jsonString);
                    break;
                case (byte)ROOM_RESPONSE_CODES.getRoomStateCode:
                    response = DeserializeGetRoomStateResponse(jsonString);
                    break;
                case (byte)ROOM_RESPONSE_CODES.leaveRoomCode:
                    response = DeserializeLeaveRoomResponse(jsonString);
                    break;
                default:
                    throw new ArgumentException("Unknown message code.");
            }

            if (response is T)
            {
                return (T)response;
            }
            else
            {
                throw new InvalidCastException($"Unable to cast object of type '{response.GetType()}' to type '{typeof(T)}'.");
            }
        }

        private static LoginResponse DeserializeLoginResponse(string jsonString)
        {
            return JsonConvert.DeserializeObject<LoginResponse>(jsonString);
        }

        private static SignupResponse DeserializeSignupResponse(string jsonString)
        {
            return JsonConvert.DeserializeObject<SignupResponse>(jsonString);
        }

        private static ErrorResponse DeserializeErrorResponse(string jsonString)
        {
            return JsonConvert.DeserializeObject<ErrorResponse>(jsonString);
        }

        private static LogoutResponse DeserializeLogoutResponse(string jsonString)
        {
            return JsonConvert.DeserializeObject<LogoutResponse>(jsonString);
        }

        private static JoinRoomResponse DeserializeJoinRoomResponse(string jsonString)
        {
            return JsonConvert.DeserializeObject<JoinRoomResponse>(jsonString);
        }

        private static CreateRoomResponse DeserializeCreateRoomResponse(string jsonString)
        {
            return JsonConvert.DeserializeObject<CreateRoomResponse>(jsonString);
        }

        private static GetRoomsResponse DeserializeGetRoomsResponse(string jsonString)
        {
            return JsonConvert.DeserializeObject<GetRoomsResponse>(jsonString);
        }

        private static GetPlayersInRoomResponse DeserializeGetPlayersInRoomResponse(string jsonString)
        {
            return JsonConvert.DeserializeObject<GetPlayersInRoomResponse>(jsonString);
        }

        private static GetHighScoreResponse DeserializeGetHighScoreResponse(string jsonString)
        {
            return JsonConvert.DeserializeObject<GetHighScoreResponse>(jsonString);
        }

        private static GetPersonalStatsResponse DeserializeGetPersonalStatsResponse(string jsonString)
        {
            return JsonConvert.DeserializeObject<GetPersonalStatsResponse>(jsonString);
        }

        private static CloseRoomResponse DeserializeCloseRoomResponse(string jsonString)
        {
            return JsonConvert.DeserializeObject<CloseRoomResponse>(jsonString);
        }

        private static StartGameResponse DeserializeStartGameResponse(string jsonString)
        {
            return JsonConvert.DeserializeObject<StartGameResponse>(jsonString);
        }

        private static GetRoomStateResponse DeserializeGetRoomStateResponse(string jsonString)
        {
            return JsonConvert.DeserializeObject<GetRoomStateResponse>(jsonString);
        }

        private static LeaveRoomResponse DeserializeLeaveRoomResponse(string jsonString)
        {
            return JsonConvert.DeserializeObject<LeaveRoomResponse>(jsonString);
        }


        public static bool isError(byte[] msg)
        {
            if (msg.Length == 0)
            {
                throw new ArgumentException("Message is too short.");
            }

            if (msg[0] == (byte)ROOM_RESPONSE_CODES.errorCode)
            {
                return true;
            }
            return false;
        }
    }
}
