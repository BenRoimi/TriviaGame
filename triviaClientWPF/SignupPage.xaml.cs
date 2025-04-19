using System;
using System.Collections;
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
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace triviaClientWPF
{
    public partial class SignupPage : Page
    {
        private Communicator m_communicator;
        public SignupPage(Communicator communicator)
        {
            InitializeComponent();
            m_communicator = communicator;
        }

        private void SignUp_Click(object sender, RoutedEventArgs e)
        {

            string email = emailTextBox.Text;
            string userName = userNameTextBox.Text;
            string password = passwordTextBox.Password;

            SignupRequest signupRequest = new SignupRequest();
            signupRequest.username = userName;
            signupRequest.password = password;
            signupRequest.mail = email;

            byte signupCode = (byte)REQUEST_CODES.signupRequestCode;
            byte[] serializedRequest = SerializationClientSide<LoginRequest>.SerializeRequest(signupRequest, signupCode);
            string sendmsg = Encoding.ASCII.GetString(serializedRequest);
            m_communicator.SendBytes(serializedRequest);
            byte[] msg = m_communicator.ReceiveBytes();
            string sendmsg1 = Encoding.ASCII.GetString(msg);
            if (DeserializationClientSide.isError(msg) == true)
            {
                ErrorResponse Error = DeserializationClientSide.DeserializeMessage<ErrorResponse>(msg);
                MessageBoxResult result = MessageBox.Show(Error.message, "Confirmation", MessageBoxButton.OK, MessageBoxImage.Question);
                //back to the menu
                NavigateToPage(new MainWindow(false));
            }
            else
            {
                SignupResponse signupResponse = DeserializationClientSide.DeserializeMessage<SignupResponse>(msg);
                if (signupResponse.status == STATUS_STATE.SUCCESS)
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
