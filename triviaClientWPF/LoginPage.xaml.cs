using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using triviaClientWPF.Requests;
using triviaClientWPF.Response;
using triviaClientWPF;

namespace triviaClientWPF
{
    public partial class LoginPage : Page
    {
        private Communicator m_communicator;
        public LoginPage(Communicator communicator)
        {
            InitializeComponent();
            m_communicator = communicator;
        }

        private void Login_Click(object sender, RoutedEventArgs e)
        {

            string userName = userNameTextBox.Text;
            string password = passwordTextBox.Password;

            LoginRequest loginRequest = new LoginRequest();
            loginRequest.username = userName;
            loginRequest.password = password;
           
            
            byte loginCode = (byte)REQUEST_CODES.loginRequestCode;
            byte[] serializedRequest = SerializationClientSide<LoginRequest>.SerializeRequest(loginRequest, loginCode);

            m_communicator.SendBytes(serializedRequest);
            byte[] msg = m_communicator.ReceiveBytes();

            if (DeserializationClientSide.isError(msg) == true)
            {
                ErrorResponse Error = DeserializationClientSide.DeserializeMessage<ErrorResponse>(msg);
                MessageBoxResult result = MessageBox.Show(Error.message, "Confirmation", MessageBoxButton.OK, MessageBoxImage.Question);
                //back to the menu
                NavigateToPage(new MainWindow(false));
            }
            else
            {
                LoginResponse loginResponse = DeserializationClientSide.DeserializeMessage<LoginResponse>(msg);
                if (loginResponse.status == STATUS_STATE.SUCCESS)
                {
                    NavigateToPage(new MainWindow(true));
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
