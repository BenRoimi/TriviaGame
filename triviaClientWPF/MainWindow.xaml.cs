using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;

namespace triviaClientWPF
{
    public partial class MainWindow
    {
        private Communicator m_communicator;

        public MainWindow()
        {
            InitializeComponent();
            m_communicator = Communicator.Instance;
        }

        public MainWindow(bool isLoginActive)
        {
            InitializeComponent();
            if (isLoginActive)
            {
                loginButton.IsEnabled = false;
                signUpButton.IsEnabled = false;

                createRoomButton.IsEnabled = true;
                joinRoomButton.IsEnabled = true;
                StatisticsButton.IsEnabled = true;
                highScoresButton.IsEnabled = true;
            }
            m_communicator = Communicator.Instance;
        }

        private void NavigateToLogIn(object sender, RoutedEventArgs e)
        {
            NavigateToPage(new LoginPage(m_communicator));
        }

        private void NavigateToSignUp(object sender, RoutedEventArgs e)
        {
            NavigateToPage(new SignupPage(m_communicator));
        }

        private void NavigateToJoinRoom(object sender, RoutedEventArgs e)
        {
            NavigateToPage(new JoinRoomPage(m_communicator));
        }

        private void NavigateToCreateRoom(object sender, RoutedEventArgs e)
        {
            NavigateToPage(new CreateRoomPage(m_communicator));
        }

        private void NavigateToStatistics(object sender, RoutedEventArgs e)
        {
            NavigateToPage(new MyStatisticsPage(m_communicator));
        }

        private void NavigateToHighScores(object sender, RoutedEventArgs e)
        {
            NavigateToPage(new HighScoresPage(m_communicator));
        }

        private void NavigateToPage(Page page)
        {
            NavigationService.Navigate(page);
            NavigationService.RemoveBackEntry();
        }

        private void QuitApplication(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }
    }
}