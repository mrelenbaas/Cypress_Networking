using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

using System;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;

public class TransmitUDP : MonoBehaviour
{
    IPEndPoint ipEndPoint;
    UdpClient udpClient;
    String ipAddress = "192.168.0.1";
    int portNumber = 50007;
    String message = "Hello!";

    void Start()
    {
        ipEndPoint = new IPEndPoint(IPAddress.Parse(ipAddress), portNumber);
        udpClient = new UdpClient();
    }

    void Update()
    {
        if (Input.GetKeyDown("space"))
        {
            SendString(message);
        }
    }

    public void SendString(string someMessage)
    {
        try
        {
            byte[] data = Encoding.UTF8.GetBytes(someMessage);
            udpClient.Send(data, message.Length, ipEndPoint);
            Debug.Log("UDP SENT");
        }
        catch (Exception e)
        {

        }
    }
}
