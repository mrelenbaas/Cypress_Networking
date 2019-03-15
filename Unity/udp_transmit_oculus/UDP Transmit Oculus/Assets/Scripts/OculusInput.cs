using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class OculusInput : MonoBehaviour
{
    public TransmitUDP transmitUDP;
    public GameObject cube;

    const string message = "Hello!";


    void Update()
    {
        if (OVRInput.GetDown(OVRInput.Button.PrimaryIndexTrigger))
        {
            transmitUDP.SendString(message);
            cube.SetActive(!cube.activeSelf);
        }
    }
}
