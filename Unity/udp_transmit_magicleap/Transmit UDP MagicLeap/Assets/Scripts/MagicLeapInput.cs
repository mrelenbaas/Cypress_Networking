using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.XR.MagicLeap;

public class MagicLeapInput : MonoBehaviour
{
    public TransmitUDP transmitUDP;
    public GameObject cube;

    MLInputController controller;
    const string message = "Hello!";

	void Start ()
    {
        MLInput.Start();
        MLInput.OnControllerButtonDown += OnButtonDown;
        MLInput.OnControllerButtonUp += OnButtonUp;
        controller = MLInput.GetController(MLInput.Hand.Left);
	}
	
	void Update ()
    {
        
	}

    void OnDestroy()
    {
        MLInput.OnControllerButtonDown -= OnButtonDown;
        MLInput.OnControllerButtonUp -= OnButtonUp;
        MLInput.Stop();
    }

    void OnButtonDown(byte controllerID, MLInputControllerButton button)
    {
        if (button == MLInputControllerButton.Bumper)
        {
            transmitUDP.SendString(message);
            cube.SetActive(!cube.activeSelf);
        }
    }

    void OnButtonUp(byte controllerID, MLInputControllerButton button)
    {

    }
}
