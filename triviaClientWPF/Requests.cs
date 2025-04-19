using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.Json;

namespace triviaClientWPF
{
    namespace Requests
    {
        enum REQUEST_CODES {
            loginRequestCode = 1,
            signupRequestCode = 2,
            createRoomRequestCode = 3,
            getRoomsRequestCode = 4,
            getPlayersInRoomRequestCode = 5,
            joinRoomRequestCode = 6,
            getStatisticsRequestCode = 7,
            logoutRequestCode = 8,
            getHighScoreRequestCode = 9,
            closeRoomRequestCode = 10,
            startGameRequestCode = 11,
            getRoomStateRequestCode = 12,
            getQuestionRequestCode = 13,
            submitAnswerRequestCode = 14,
            getGameResultsRequestCode = 15,
            leaveGameRequestCode = 16
};
        public class LoginRequest
        {
            public string username { get; set; }
            public string password { get; set; }
        }

        public class SignupRequest
        {
            public string username { get; set; }
            public string password { get; set; }
            public string mail { get; set; }
        }

        public class CreateRoomRequest
        {
            public string roomName { get; set; }
            public uint maxUsers { get; set; }
            public uint questionCount { get; set; }
            public uint answerTimeout { get; set; }
        }

        public class GetPlayersInRoomRequest
        {
            public uint roomId { get; set; }
        }

        public class JoinRoomRequest
        {
            public uint roomId { get; set; }
        }

    }
}
