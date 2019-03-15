using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class GoogleInput : MonoBehaviour
{
    public TransmitUDP transmitUDP;
    public GameObject cube;
    public GameObject controller;

    const string message = "Hello!";

    private const GvrControllerButton pointerButtonMask =
                GvrControllerButton.App |
                GvrControllerButton.TouchPadButton |
                GvrControllerButton.Trigger |
                GvrControllerButton.Grip;

    static readonly GvrControllerHand[] AllHands =
    {
        GvrControllerHand.Right,
        GvrControllerHand.Left,
    };


    void Update()
    {
        //GvrTrackedController trackedController1 = controllerPointers[1].GetComponent<GvrTrackedController>();
        foreach (var hand in AllHands)
        {
            GvrControllerInputDevice device = GvrControllerInput.GetDevice(hand);
            if (device.GetButtonDown(pointerButtonMask))
            {
                cube.SetActive(!cube.activeSelf);
                transmitUDP.SendString(message);
                // Match the button to our own controllerPointers list.
                /*
                if (device == trackedController1.ControllerInputDevice)
                {
                }
                else
                {
                }
                */

                break;
            }
        }
    }
}
