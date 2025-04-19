using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace triviaClientWPF
{
    public static class SerializationClientSide<T>
    {

        public static byte[] SerializeRequest<T>(T Request, byte MsgCode)
        {
           
            if (Request == null)
            {
                var jsonString = "";
                byte[] bytesOfJson = Encoding.ASCII.GetBytes(jsonString);
                int sizeOfMsg = bytesOfJson.Length;
                byte[] sizeBytes = BitConverter.GetBytes(sizeOfMsg);
                byte[] fullMessage = new byte[1 + sizeBytes.Length + bytesOfJson.Length];
                fullMessage[0] = MsgCode;
                Array.Copy(sizeBytes, 0, fullMessage, 1, sizeBytes.Length);
                Array.Copy(bytesOfJson, 0, fullMessage, 1 + sizeBytes.Length, bytesOfJson.Length);
                return fullMessage;
            }
            else
            {
                var jsonString = JsonSerializer.Serialize(Request);
                byte[] bytesOfJson = Encoding.ASCII.GetBytes(jsonString);
                int sizeOfMsg = bytesOfJson.Length;
                byte[] sizeBytes = BitConverter.GetBytes(sizeOfMsg);
                byte[] fullMessage = new byte[1 + sizeBytes.Length + bytesOfJson.Length];
                fullMessage[0] = MsgCode;
                Array.Copy(sizeBytes, 0, fullMessage, 1, sizeBytes.Length);
                Array.Copy(bytesOfJson, 0, fullMessage, 1 + sizeBytes.Length, bytesOfJson.Length);
                return fullMessage;
            }
           
        }
    }

}

