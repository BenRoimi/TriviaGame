using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using triviaClientWPF.Requests;
using triviaClientWPF.Response;

namespace triviaClientWPF
{
    partial class waitingRoomsForUser : Page
    {
        private Communicator m_communicator;
        private Thread _checkRoomState;
        private ObservableCollection<string> m_userList = new ObservableCollection<string>();

        public waitingRoomsForUser(Communicator communicator)
        {
            InitializeComponent();
            _checkRoomState = new Thread(new ThreadStart(checkStatus));
            _checkRoomState.Start();
            m_communicator = communicator;
        }
        private void Exit_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            byte leaveRoomRequestCode = (byte)REQUEST_CODES.leaveGameRequestCode;
            byte[] serializedRequest = SerializationClientSide<object>.SerializeRequest(0, leaveRoomRequestCode);

            m_communicator.SendBytes(serializedRequest);
            byte[] msg = m_communicator.ReceiveBytes();

            LeaveRoomResponse leaveRoomResponse = DeserializationClientSide.DeserializeMessage<LeaveRoomResponse>(msg);
            if (leaveRoomResponse.status == STATUS_STATE.SUCCESS)
            {
                NavigateToPage(new MainWindow(false));
            }
        }
        private void NavigateToPage(Page page)
        {
            NavigationService.Navigate(page);
            NavigationService.RemoveBackEntry();
        }

        private void checkStatus()
        {
            while (true)
            {
                byte RoomStateRequestCode = (byte)REQUEST_CODES.getRoomStateRequestCode;
                byte[] serializedRequest = SerializationClientSide<object>.SerializeRequest(0, RoomStateRequestCode);

                m_communicator.SendBytes(serializedRequest);
                byte[] msg = m_communicator.ReceiveBytes();

                if(DeserializationClientSide.isError(msg) == true)
                {
                    ErrorResponse Error = DeserializationClientSide.DeserializeMessage<ErrorResponse>(msg);
                    MessageBoxResult result = MessageBox.Show(Error.message, "Confirmation", MessageBoxButton.OK, MessageBoxImage.Question);
                    //back to the menu
                    NavigateToPage(new MainWindow(true));


                }
                  
                else
                {
                  
                    GetRoomStateResponse RoomStateResponse = DeserializationClientSide.DeserializeMessage<GetRoomStateResponse>(msg);
                    if (RoomStateResponse.roomState == ROOM_STATUS.waiting)//is waiting?
                    {
                        Thread.Sleep(333);
                    }
                    else
                    {
                        //need to move to the room game...
                    }
                    
                }
                
            }
        }
    }
}
