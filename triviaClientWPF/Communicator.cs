using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

public class Communicator
{
    private Socket m_socket;
    private static Communicator _instance;

    public Communicator()
    {
        m_socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        Connect("127.0.0.1", 8826);
    }

    public void Connect(string ipAddress, int port)
    {
        try
        {
            IPAddress serverAddress = IPAddress.Parse(ipAddress);
            IPEndPoint endPoint = new IPEndPoint(serverAddress, port);

            m_socket.Connect(endPoint);
            Console.WriteLine("Connected to server.");
        }
        catch (Exception ex)
        {
            Console.WriteLine("Error connecting to server: " + ex.Message);
        }
    }

    public void SendBytes(byte[] bytes)
    {
        try
        {
            m_socket.Send(bytes);
            Console.WriteLine("Bytes sent to server.");
        }
        catch (Exception ex)
        {
            Console.WriteLine("Error sending message to server: " + ex.Message);
        }
    }

    public byte[] ReceiveBytes()
    {
        try
        {
            byte[] buffer = new byte[1024];
            int bytesRead = m_socket.Receive(buffer);
            byte[] receivedBytes = new byte[bytesRead];
            Array.Copy(buffer, receivedBytes, bytesRead);
            return receivedBytes;
        }
        catch (Exception ex)
        {
            Console.WriteLine("Error receiving message from server: " + ex.Message);
            return new byte[0];
        }
    }

    public void CloseConnection()
    {
        try
        {
            m_socket.Shutdown(SocketShutdown.Both);
            m_socket.Close();
            Console.WriteLine("Connection closed.");
        }
        catch (Exception ex)
        {
            Console.WriteLine("Error closing connection: " + ex.Message);
        }
    }
    public static Communicator Instance
    {
        get
        {
            if (_instance == null)
            {
                _instance = new Communicator();
            }
            return _instance;
        }
    }
}
