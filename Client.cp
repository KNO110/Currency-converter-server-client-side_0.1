using System;
using System.Net.Sockets;
using System.Text;

class CurrencyClient
{
    static void Main(string[] args)
    {
        TcpClient client = null;
        try
        {
            while (true)
            {

                client = new TcpClient("localhost", 20042);
                NetworkStream stream = client.GetStream();

                Console.WriteLine("Enter currency pair (USD-EUR or EUR-USD) or type 'quit' to leave ☺");
                string input = Console.ReadLine();

                if (input.ToLower() == "quit")
                {
                            ///Короче вообще тема, при написании quit оно отправит звёздочку и серв поймёт что челик ливнул (на самом деле мне лень делать на серваке многопотоковость чтобы проверять подключён ли челик)
                    input = "*";
                    byte[] quitSignal = Encoding.ASCII.GetBytes(input);
                    stream.Write(quitSignal, 0, quitSignal.Length);

                    break;
                }

                byte[] data = Encoding.ASCII.GetBytes(input);
                stream.Write(data, 0, data.Length);

                data = new byte[256];
                int bytes = stream.Read(data, 0, data.Length);
                string response = Encoding.ASCII.GetString(data, 0, bytes);
                Console.WriteLine($"Received response: {response}");

                stream.Close();
                client.Close();
            }
        }
        catch (Exception e)
        {
            Console.WriteLine($"Exception: {e}");
        }
        finally
        {
            if (client != null)
                client.Close();
        }

        Console.WriteLine("\nClient disconnected :__(");
    }
}
