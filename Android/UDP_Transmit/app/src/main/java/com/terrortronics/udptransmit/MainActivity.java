package com.terrortronics.udptransmit;

import android.os.AsyncTask;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class MainActivity extends AppCompatActivity {

    final String message = "Hello!\n";
    final String IPAdress = "192.168.0.1";

    private class MessageSender extends AsyncTask<String, Void, Void> {
        @Override
        protected Void doInBackground(String... voids) {

            Log.d("SOMETHING", "SENDING MESSAGE -2");
            int serverPort = 50007;

            Log.d("SOMETHING", "SENDING MESSAGE -1");
            try {
                DatagramSocket s = new DatagramSocket();
                Log.d("SOMETHING", "SENDING MESSAGE 0");
                InetAddress inetAddress = InetAddress.getByName(IPAdress);
                Log.d("SOMETHING", "SENDING MESSAGE 1");
                byte[] tempMessage = message.getBytes();
                Log.d("SOMETHING", "SENDING MESSAGE 2");
                DatagramPacket d = new DatagramPacket(
                        tempMessage,
                        message.length(),
                        inetAddress,
                        serverPort
                );
                Log.d("SOMETHING", "SENDING MESSAGE 3");
                s.send(d);
                Log.d("SOMETHING", "SENDING MESSAGE 4");
            } catch (Exception e) {
                System.err.println(e);
                e.printStackTrace();
            }

            return null;
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final Button button = findViewById(R.id.button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.d("SOMETHING", "CLICK 1");
                MessageSender messageSender = new MessageSender();
                Log.d("SOMETHING", "CLICK 2");
                messageSender.execute();
                Log.d("SOMETHING", "CLICK 3");
            }
        });

        /*
        new Thread(new Runnable() {
            @Override
            public void run() {
                while(true) {
                    try {

                    } catch(Exception e) {

                    }
                }
            }
        }).start();
        */
    }
}
