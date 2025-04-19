using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using triviaClientWPF.Requests;
using triviaClientWPF.Response;

namespace triviaClientWPF
{
    partial class waitingRoomsForAdmin: Page
    {
        private Communicator m_communicator;
        public waitingRoomsForAdmin(Communicator communicator)
        {
            InitializeComponent();
            m_communicator = communicator;
        }
        private void Start_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            byte startGameRequestCode = (byte)REQUEST_CODES.startGameRequestCode;
            byte[] serializedRequest = SerializationClientSide<object>.SerializeRequest(0, startGameRequestCode);

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
                StartGameResponse startGameResponse = DeserializationClientSide.DeserializeMessage<StartGameResponse>(msg);
                if (startGameResponse.status == STATUS_STATE.SUCCESS)
                {
                    //
                }
            }
           
        }

        private void Exit_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            byte closeRoomRequestCode = (byte)REQUEST_CODES.closeRoomRequestCode;
            byte[] serializedRequest = SerializationClientSide<object>.SerializeRequest(0, closeRoomRequestCode);

            m_communicator.SendBytes(serializedRequest);
            byte[] msg = m_communicator.ReceiveBytes();

            CloseRoomResponse closeRoomResponse = DeserializationClientSide.DeserializeMessage<CloseRoomResponse>(msg);
            if (closeRoomResponse.status == STATUS_STATE.SUCCESS)
            {
                NavigateToPage(new MainWindow(false));
            }
        }
        private void NavigateToPage(Page page)
        {
            NavigationService.Navigate(page);
            NavigationService.RemoveBackEntry();
        }

    }

}


    
