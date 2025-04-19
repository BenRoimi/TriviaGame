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
    public partial class MyStatisticsPage : Page
    {
        private Communicator m_communicator;
        public MyStatisticsPage(Communicator communicator)
        {
            InitializeComponent();
            m_communicator = communicator;


            byte getStatisticsCode = (byte)REQUEST_CODES.getStatisticsRequestCode;
            byte[] serializedRequest = SerializationClientSide<object>.SerializeRequest(0, getStatisticsCode);

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
                GetPersonalStatsResponse PersonalStatsResponse = DeserializationClientSide.DeserializeMessage<GetPersonalStatsResponse>(msg);
                if (PersonalStatsResponse.status == (uint)STATUS_STATE.SUCCESS)
                {
                    // need to check the order in the list of statistics
                    UserTotalScore.Text = PersonalStatsResponse.statistics[0];
                    UserAvrageTime.Text = PersonalStatsResponse.statistics[1];
                    userCorrectAnswer.Text = PersonalStatsResponse.statistics[2];
                    UserTotalAnswer.Text = PersonalStatsResponse.statistics[3];
                    UserTotalGames.Text = PersonalStatsResponse.statistics[4];
                }
            }
        }

        private void NavigateToPage(Page mainWindow)
        {
            NavigationService.Navigate(mainWindow);
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
