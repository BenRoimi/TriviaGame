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
    public partial class HighScoresPage : Page
    {
        private Communicator m_communicator;
        public HighScoresPage(Communicator communicator)
        {
            InitializeComponent();
            m_communicator = communicator;

            byte getHighScoreCode = (byte)REQUEST_CODES.getHighScoreRequestCode;
            byte[] serializedRequest = SerializationClientSide<object>.SerializeRequest(0, getHighScoreCode);

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
                GetHighScoreResponse HighScoreResponse = DeserializationClientSide.DeserializeMessage<GetHighScoreResponse>(msg);
                if (HighScoreResponse.status == STATUS_STATE.SUCCESS)
                {
                    string[] user1 = HighScoreResponse.statistics[0].Split(':');
                    player1.Text = user1[0];
                    score1.Text = user1[1];

                    string[] user2 = HighScoreResponse.statistics[1].Split(':');
                    player2.Text = user2[0];
                    score2.Text = user2[1];

                    string[] user3 = HighScoreResponse.statistics[2].Split(':');
                    player3.Text = user3[0];
                    score3.Text = user3[1];

                    string[] user4 = HighScoreResponse.statistics[3].Split(':');
                    player4.Text = user4[0];
                    score4.Text = user4[1];

                    string[] user5 = HighScoreResponse.statistics[5].Split(':');
                    player5.Text = user5[0];
                    score5.Text = user5[1];
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
