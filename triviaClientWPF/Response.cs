using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace triviaClientWPF
{
    namespace Response
    {
        enum ROOM_RESPONSE_CODES
        {
            loginCode = 48, // '0'
            signupCode = 49,
            errorCode = 50,
            logoutCode = 51,
            joinRoomCode = 52,
            createRoomCode = 53,
            getRoomsCode = 54,
            getPlayersCode = 55,
            getHighScoreCode = 56,
            getPersonalStatsCode = 57,
            closeRoomCode = 58,
            startGameCode = 59,
            getRoomStateCode = 60,
            leaveRoomCode = 61
        };


    }

    enum STATUS_STATE
    {
        SUCCESS = 1,
        FAIL = 0
    }

    public enum ROOM_STATUS
    {
        waiting = 1,
        active = 2,
        unavailable = 3
    }

    struct LoginResponse
    {
        public STATUS_STATE status;
    }

    struct SignupResponse
    {
        public STATUS_STATE status;
    }

    struct ErrorResponse
    {
        public string message;
    }

    struct LogoutResponse
    {
        public STATUS_STATE status;
    }

    struct JoinRoomResponse
    {
        public STATUS_STATE status;
    }

    struct CreateRoomResponse
    {
        public STATUS_STATE status;
    }

    struct GetRoomsResponse
    {
        public STATUS_STATE status;
        public List<RoomData> rooms;
    }

    struct GetPlayersInRoomResponse
    {
        public List<string> players;
    }

    struct GetHighScoreResponse
    {
        public STATUS_STATE status;
        public List<string> statistics;
    }

    struct GetPersonalStatsResponse
    {
        public uint status;
        public List<string> statistics;
    }

    struct CloseRoomResponse
    {
        public STATUS_STATE status;
    }

    struct StartGameResponse
    {
        public STATUS_STATE status;
    }

    struct LeaveRoomResponse
    {
        public STATUS_STATE status;
    }

    struct GetRoomStateResponse
    {
        public STATUS_STATE status;
        public ROOM_STATUS roomState;
        public List<string> players;
        public uint questionCount;
        public uint answerTimeout;

    }


    public struct RoomData
    {
        public uint id;
        public string name;
        public uint maxPlayers;
        public uint numOfQuestionsInGame;
        public uint timePerQuestion;
        public ROOM_STATUS status;
    }
}
