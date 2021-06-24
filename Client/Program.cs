using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Threading;
using System.Net.Sockets;

namespace Client
{
    class Response
    {
        public string request { get; set; }
        public string key { get; set; }
    }
    class ResponseAdd
    {
        public string request { get; set; }
        public string key { get; set; }
        public string value { get; set; }
    }

    class Program
    {
        static TcpClient client = new TcpClient();
        static Random rnd = new Random((int)DateTime.Now.Ticks & 0x0000FFFF);
        static int id = 0;
        static void Main(string[] args)
        {
            while (true)
            {
                try
                {
                    Console.WriteLine("wait server");
                    client.Connect("127.0.0.1", 8001);
                    Console.WriteLine("server ok");
                    var stream = client.GetStream();
                    while (true)
                    {
                        Thread.Sleep(rnd.Next(100, 2000));
                        if (!Response(stream)) break;
                    }
                }
                catch (Exception){
                    client.Close();
                    client = new TcpClient();
                }
            }
        }
        static bool Response(NetworkStream stream)
        {
            byte[] buff = MakeResponse();
            Console.WriteLine(Encoding.UTF8.GetString(buff));
            stream.Write(buff, 0, buff.Length);
            stream.WriteByte(0);
            string answer = "";
            do
            {
                var size = stream.Read(buff, 0, buff.Length);
                answer+=Encoding.UTF8.GetString(buff, 0, size);
            } while (stream.DataAvailable);
            OutAnswer(answer);
            return true;
        }
        static byte[] MakeResponse()
        {
            switch (rnd.Next(0, 2))
            {
                case 0:
                    return AddValue();
                case 1:
                    return GetValue();
            }
            return new byte[0];
        }
        static byte[] AddValue()
        {
            id += rnd.Next(0, 2);
            ResponseAdd add = new ResponseAdd
            {
                request = "write",
                key = id.ToString(),
                value = rnd.Next(-100, 101).ToString()
            };
            return JsonSerializer.SerializeToUtf8Bytes<ResponseAdd>(add);
        }
        static byte[] GetValue()
        {
            Response get = new Response
            {
                request = "read",
                key = rnd.Next(0, id+1).ToString()
            };
            return JsonSerializer.SerializeToUtf8Bytes<Response>(get);
        }
        static void OutAnswer(string answer)
        {
            JsonDocument json = JsonDocument.Parse(answer);
            var root = json.RootElement;
            var it = root.EnumerateObject();
            while (it.MoveNext())
            {
                var prop = it.Current;
                Console.WriteLine($"{prop.Name} : {prop.Value}");
            }
            json.Dispose();
        }
    }
}
