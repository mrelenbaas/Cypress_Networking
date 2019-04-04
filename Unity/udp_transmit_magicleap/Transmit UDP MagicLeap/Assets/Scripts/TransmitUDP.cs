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

    // receiving Thread
    Thread receiveThread;

    // udpclient object
    UdpClient client;

    // public
    // public string IP = "127.0.0.1"; default local
    public int port; // define > init

    // infos
    public string lastReceivedUDPPacket = "";
    public string allReceivedUDPPackets = ""; // clean up this from time to time!

    public GameObject cube;
    public Text text;

    void Start()
    {
        ipEndPoint = new IPEndPoint(IPAddress.Parse(ipAddress), portNumber);
        udpClient = new UdpClient();

        //InvokeRepeating(ReceiveString);
        init();
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

    /*
    void ReceiveString()
    {

    }
    */

    // init
    private void init()
    {
        // Endpunkt definieren, von dem die Nachrichten gesendet werden.
        print("UDPSend.init()");

        // define port
        port = 50007;

        // status
        print("Sending to 127.0.0.1 : " + port);
        print("Test-Sending to this Port: nc -u 127.0.0.1  " + port + "");


        // ----------------------------
        // Abhören
        // ----------------------------
        // Lokalen Endpunkt definieren (wo Nachrichten empfangen werden).
        // Einen neuen Thread für den Empfang eingehender Nachrichten erstellen.
        receiveThread = new Thread(
            new ThreadStart(ReceiveData));
        receiveThread.IsBackground = true;
        receiveThread.Start();

    }

    // receive thread
    private void ReceiveData()
    {

        client = new UdpClient(port);
        while (true)
        {

            try
            {
                // Bytes empfangen.
                IPEndPoint anyIP = new IPEndPoint(IPAddress.Any, 0);
                byte[] data = client.Receive(ref anyIP);

                // Bytes mit der UTF8-Kodierung in das Textformat kodieren.
                string textString = Encoding.UTF8.GetString(data);

                // Den abgerufenen Text anzeigen.
                print(">> " + textString);
                cube.SetActive(!cube.activeSelf);
                if(textString.Contains("on"))
                {
                    cube.GetComponent<Renderer>().material.color = Color.red;
                }
                else
                {
                    cube.GetComponent<Renderer>().material.color = Color.white;
                }
                cube.GetComponent<Renderer>().material.color = Color.red;
                text.text = textString;

                // latest UDPpacket
                lastReceivedUDPPacket = textString;

                // ....
                allReceivedUDPPackets = allReceivedUDPPackets + textString;

            }
            catch (Exception err)
            {
                print(err.ToString());
            }
        }
    }

    // getLatestUDPPacket
    // cleans up the rest
    public string getLatestUDPPacket()
    {
        allReceivedUDPPackets = "";
        return lastReceivedUDPPacket;
    }
}
