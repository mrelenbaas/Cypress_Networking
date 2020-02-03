package com.terrortronics.udptransmit;

import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.text.DateFormat;
import java.util.Date;

public class MainActivity extends AppCompatActivity {

    final String message = "Hello!";
    final String IPAdress = "192.168.1.3";

    Button button;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        button = findViewById(R.id.button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                MessageSender messageSender = new MessageSender();
                messageSender.execute();
            }
        });
        button.setBackgroundColor(Color.WHITE);

        startServerSocket();
    }

    private void startServerSocket() {

        Thread thread = new Thread(new Runnable() {

            private String stringData = null;

            @Override
            public void run() {

                byte[] msg = new byte[1000];
                DatagramPacket dp = new DatagramPacket(msg, msg.length);
                DatagramSocket ds = null;
                try {
                    ds = new DatagramSocket(50007);
                    //ds.setSoTimeout(50000);
                    ds.receive(dp);

                    stringData = new String(msg, 0, dp.getLength());
                    if (stringData.contains("on"))
                    {
                        button.setBackgroundColor(Color.RED);
                    }
                    else if (stringData.contains("off"))
                    {
                        button.setBackgroundColor(Color.WHITE);
                    }
                    String currentDateTimeString = DateFormat.getDateTimeInstance().format(new Date());
                    stringData = stringData + " " + currentDateTimeString;
                    Log.d("SOMETHING", stringData);
                    //updateUI(stringData);

                    //String msgToSender = "Bye Bye ";
                    //dp = new DatagramPacket(msgToSender.getBytes(), msgToSender.length(), dp.getAddress(), dp.getPort());
                    //ds.send(dp);

                } catch (IOException e) {
                    e.printStackTrace();
                } finally {
                    if (ds != null) {
                        ds.close();
                        startServerSocket();
                    }
                }
            }

        });
        thread.start();
    }

    private class MessageSender extends AsyncTask<String, Void, Void> {
        @Override
        protected Void doInBackground(String... voids) {

            int serverPort = 1234;

            try {
                DatagramSocket s = new DatagramSocket();
                InetAddress inetAddress = InetAddress.getByName(IPAdress);
                byte[] tempMessage = message.getBytes();
                DatagramPacket d = new DatagramPacket(
                        tempMessage,
                        message.length(),
                        inetAddress,
                        serverPort
                );
                s.send(d);
            } catch (Exception e) {
                System.err.println(e);
                e.printStackTrace();
            }

            return null;
        }
    }
}
