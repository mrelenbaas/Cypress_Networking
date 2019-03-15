using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Valve.VR;

public class ViveInput : MonoBehaviour
{
    public TransmitUDP transmitUDP;

    SteamVR_ActionSet actionSet;
    SteamVR_Action_Single squeezeInput;

    const string message = "Hello!";

    void Awake()
    {
        squeezeInput = SteamVR_Actions._default.Squeeze;
    }

    void Start()
    {
        actionSet.Activate(SteamVR_Input_Sources.Any, 0, true);
    }

    void Update()
    {
        if (true)
        {
            if(SteamVR_Actions._default.Teleport.GetStateDown(SteamVR_Input_Sources.Any))
            {
                transmitUDP.SendString(message);
                Debug.Log("SOMETHING");
            }
        }
    }
}
