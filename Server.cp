using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;

class CurrencyServer
{
    private static Dictionary<string, double> exchangeRates = new Dictionary<string, double>()
    {
        { "USD-EUR", 0.93 },
        { "EUR-USD", 1.08 }
    };

    static void Main(string[] args)
    {
        TcpListener server = null;
        try
        {
            int port = 20042;
            IPAddress local_ = IPAddress.Parse("127.0.0.1");

            server = new TcpListener(local_, port);

            server.Start();

            Console.WriteLine("Server started...");

            while (true)
            {
                Console.WriteLine("\nWaiting for a connection...");

                TcpClient client = server.AcceptTcpClient();

                Console.WriteLine("\nConnected!\n");

                NetworkStream stream = client.GetStream();

                string request = "";

                byte[] data = new byte[256];
                int bytes = stream.Read(data, 0, data.Length);
                request = Encoding.ASCII.GetString(data, 0, bytes);

                if (request == "*")     /// объяснял в коде клиента, короче если ловит * то челик отключился 
                {
                    Console.WriteLine($"\nUser disconnected at {DateTime.Now}");
                    client.Close();
                    continue;
                }

                Console.WriteLine($"\nReceived request: {request}");

                string response = Exchange_valuta(request);

                byte[] responseData = Encoding.ASCII.GetBytes(response);

                stream.Write(responseData, 0, responseData.Length); 
                Console.WriteLine($"\nSent response: {response}");

                Console.WriteLine($"\nConnection details: {client.Client.RemoteEndPoint} connected at {DateTime.Now}");

                client.Close();
            }
        }
        catch (SocketException e)
        {
            Console.WriteLine($"\nSocketException: {e}");
        }
        finally
        {
            server.Stop();
        }   

        Console.WriteLine("\nServer stopped...");
    }

    private static string Exchange_valuta(string request)
    {
        if (exchangeRates.ContainsKey(request.ToUpper()))
        {

            return $"{request}: {exchangeRates[request.ToUpper()]}";

        }
        else
        {

            return "Invalid currency choto-tam.";

        }
    }
}
