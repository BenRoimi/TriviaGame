using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using triviaClientWPF.Requests;
using triviaClientWPF.Response;

namespace triviaClientWPF
{
    public partial class CreateRoomPage : Page
    {
        private Communicator m_communicator;
        public CreateRoomPage(Communicator communicator)
        {
            InitializeComponent();
            m_communicator = communicator;
       
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {

            string roomName = RoomName.Text;
            uint numbarOfPlayers = uint.Parse(NumberOfPlayers.Text);
            uint timePerQustion = uint.Parse(TimePerQuestion.Text);

            CreateRoomRequest roomRequest = new CreateRoomRequest();
            roomRequest.roomName = roomName;
            roomRequest.answerTimeout = timePerQustion;
            roomRequest.maxUsers = numbarOfPlayers;
            roomRequest.questionCount = 10;

            byte createRoomRequestCode = (byte)REQUEST_CODES.createRoomRequestCode;
            byte[] serializedRequest = SerializationClientSide<LoginRequest>.SerializeRequest(roomRequest, createRoomRequestCode);

            m_communicator.SendBytes(serializedRequest);
            byte[] msg = m_communicator.ReceiveBytes();

            if (DeserializationClientSide.isError(msg) == true)
            {
                ErrorResponse Error = DeserializationClientSide.DeserializeMessage<ErrorResponse>(msg);
                MessageBoxResult result = MessageBox.Show(Error.message, "Confirmation", MessageBoxButton.OK, MessageBoxImage.Question);
                //back to the menu
                NavigateToPage(new MainWindow(true));
            }
            else
            {
                CreateRoomResponse createRoomResponse = DeserializationClientSide.DeserializeMessage<CreateRoomResponse>(msg);
                if (createRoomResponse.status == STATUS_STATE.SUCCESS)
                {
                    NavigateToPage(new waitingRoomsForAdmin(m_communicator));
                }
            }
           
        }
        private void NavigateToPage(Page page)
        {
            NavigationService.Navigate(page);
            NavigationService.RemoveBackEntry();
        }



        private void GoBack_Click(object sender, RoutedEventArgs e)
        {
            if (NavigationService.CanGoBack)
            {
                NavigationService.GoBack();
            }
        }
    }
}