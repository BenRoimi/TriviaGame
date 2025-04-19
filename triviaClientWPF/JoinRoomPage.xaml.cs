using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using triviaClientWPF.Requests;
using triviaClientWPF.Response;

namespace triviaClientWPF
{
    public partial class JoinRoomPage : Page
    {
        private Communicator m_communicator;
        private Thread _backgroundThread;
        public ObservableCollection<RoomData> m_roomList;
        private bool stopBackroundThread;
        public JoinRoomPage(Communicator communicator)
        {
            InitializeComponent();
            _backgroundThread = new Thread(new ThreadStart(RoomsListRefresh));
            _backgroundThread.Start();
            stopBackroundThread = false;
            m_communicator = communicator;
            m_roomList = new ObservableCollection<RoomData>();
        }
        private void GoBack_Click(object sender, RoutedEventArgs e)
        {
            stopBackroundThread = true;

            if (_backgroundThread.IsAlive)
            {
                _backgroundThread.Join();
            }

            if (NavigationService.CanGoBack)
            {
                NavigationService.GoBack();
            }
        }

        private void RoomsListRefresh()
        {
            byte getRoomsCode = (byte)REQUEST_CODES.getRoomsRequestCode;
            byte[] sizeBytes = BitConverter.GetBytes(0);
            byte[] serializedMessage = new byte[1 + sizeBytes.Length];
            serializedMessage[0] = getRoomsCode;
            Array.Copy(sizeBytes, 0, serializedMessage, 1, sizeBytes.Length);

            while (!stopBackroundThread)
            {
                m_communicator.SendBytes(serializedMessage);
                byte[] msg = m_communicator.ReceiveBytes();

                if (DeserializationClientSide.isError(msg) == true)
                {
                    ErrorResponse Error = DeserializationClientSide.DeserializeMessage<ErrorResponse>(msg);
                    MessageBoxResult result = MessageBox.Show(Error.message, "Confirmation", MessageBoxButton.OK, MessageBoxImage.Question);
                    NavigateToPage(new MainWindow(true));
                }
                else
                {
                    GetRoomsResponse getRoomsResponse = DeserializationClientSide.DeserializeMessage<GetRoomsResponse>(msg);
                    m_roomList = new ObservableCollection<RoomData>(getRoomsResponse.rooms);
                }

                Thread.Sleep(333);
            }
        }

        private void OnListElementDoubleClick(object sender, RoutedEventArgs e)
        {
            var listView = sender as ListView;

            if(listView.Items.Count > 0 )
            {
                var clickedItem = (RoomData)listView.SelectedItem;

                JoinRoomRequest joinRoomRequest = new JoinRoomRequest();
                joinRoomRequest.roomId = clickedItem.id;

                byte joinRoomCode = (byte)REQUEST_CODES.joinRoomRequestCode;
                byte[] serializedRequest = SerializationClientSide<LoginRequest>.SerializeRequest(joinRoomRequest, joinRoomCode);

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
                    JoinRoomResponse joinRoomResponse = DeserializationClientSide.DeserializeMessage<JoinRoomResponse>(msg);
                    if (joinRoomResponse.status == STATUS_STATE.SUCCESS)
                    {
                        NavigateToPage(new waitingRoomsForUser(m_communicator));
                    }
                }
            }
        }

        private void NavigateToPage(Page page)
        {
            NavigationService.Navigate(page);
            NavigationService.RemoveBackEntry();
        }
    }
}
