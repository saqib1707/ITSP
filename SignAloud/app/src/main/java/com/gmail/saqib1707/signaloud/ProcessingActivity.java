package com.gmail.saqib1707.signaloud;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Handler;
import android.os.Message;
import android.speech.tts.TextToSpeech;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.common.api.GoogleApiClient;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Locale;
import java.util.UUID;

public class ProcessingActivity extends AppCompatActivity {

    Button ledTurnOn, ledTurnOff, disconnect;
    ProgressDialog progress;
    UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    BluetoothSocket btSocket = null;
    BluetoothAdapter btAdapter = null;
    boolean isBtconnected = false;
    String address = null;
    TextView msgReceived;
    static Handler myHandler;
    StringBuilder recReadData;
    ConnectedThread myThread;
    TextToSpeech textToSpeech;
    ConnectBluetooth bt = new ConnectBluetooth();
    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient client;

    public void msg(String str) {
        final Toast toast = Toast.makeText(getApplicationContext(), str, Toast.LENGTH_LONG);
        toast.show();
        Handler myTimeHandler = new Handler();
        myTimeHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                toast.cancel();
            }
        }, 500);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_processing);
        Intent receivedIntent = getIntent();
        address = receivedIntent.getStringExtra(HomeActivity.EXTRA_ADDRESS);
        ledTurnOff = (Button) findViewById(R.id.button_ledturnoff);
        ledTurnOn = (Button) findViewById(R.id.button_ledturnon);
        disconnect = (Button) findViewById(R.id.button_disconnect);
        msgReceived = (TextView) findViewById(R.id.textView_receivedmessage);
        recReadData = new StringBuilder();
        bt.execute();

        myHandler = new Handler() {
            public void handleMessage(Message msg) {
                if (msg.what == 0) {
                    String readmessage = (String) msg.obj;
                    recReadData.append(readmessage);
                    int indexOfEndChar = recReadData.indexOf("~");
                    if (indexOfEndChar > 0) {
                        if (recReadData.charAt(0) == '#') {
                            String finalMessage = recReadData.substring(1, indexOfEndChar);
                            msgReceived.setText(finalMessage);
                            String toSpeak = msgReceived.getText().toString();
                            msg(toSpeak);
                            if (Build.VERSION.RELEASE.startsWith("6")) {
                                textToSpeech.speak(toSpeak, TextToSpeech.QUEUE_FLUSH, null, null);
                            } else {
                                textToSpeech.speak(toSpeak, TextToSpeech.QUEUE_FLUSH, null);
                            }
                        }
                        recReadData.delete(0, recReadData.length());
                    }
                }
            }
        };

        ledTurnOn.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if (btSocket != null) {
                            try {
                                btSocket.getOutputStream().write("TO".getBytes());
                                msg("TO sent to HC-05");
                            } catch (Exception e) {
                                msg("Unable to TurnOn");
                            }
                        }
                    }
                }
        );

        ledTurnOff.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if (btSocket != null) {
                            try {
                                btSocket.getOutputStream().write("TF".getBytes());
                                msg("TF sent to HC-05");
                            } catch (Exception e) {
                                msg("Unable to TurnOff");
                            }
                        }
                    }
                }
        );

        disconnect.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if (btSocket != null) {
                            try {
                                btSocket.getOutputStream().close();
                                btSocket.getInputStream().close();
                                btSocket.close();
                                msg("Disconnected");
                            } catch (Exception e) {
                                msg("Unable to Disconnect");
                            }
                        }
                        finish();
                    }
                }
        );
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();
    }

    @Override
    public void onStop() {
        super.onStop();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        Action viewAction = Action.newAction(
                Action.TYPE_VIEW, // TODO: choose an action type.
                "Processing Page", // TODO: Define a title for the content shown.
                // TODO: If you have web page content that matches this app activity's content,
                // make sure this auto-generated web page URL is correct.
                // Otherwise, set the URL to null.
                Uri.parse("http://host/path"),
                // TODO: Make sure this auto-generated app URL is correct.
                Uri.parse("android-app://com.gmail.saqib1707.signaloud/http/host/path")
        );
        AppIndex.AppIndexApi.end(client, viewAction);
        client.disconnect();
    }

    public class ConnectBluetooth extends AsyncTask<Void, Void, Void> {
        boolean connectSuccess = true;

        @Override
        public void onPreExecute() {
            progress = ProgressDialog.show(ProcessingActivity.this, "Connecting to device", "Please wait man");
        }

        @Override
        public Void doInBackground(Void... btDevices) {
            try {
                if (btSocket == null || !isBtconnected) {
                    btAdapter = BluetoothAdapter.getDefaultAdapter();
                    BluetoothDevice btDevice = btAdapter.getRemoteDevice(address);
                    btSocket = btDevice.createInsecureRfcommSocketToServiceRecord(myUUID);
                    btAdapter.cancelDiscovery();
                    btSocket.connect();
                    connectSuccess = true;

                }
            } catch (IOException e) {
                connectSuccess = false;
            }
            return null;
        }

        @Override
        public void onPostExecute(Void result) {
            super.onPostExecute(result);

            if (!connectSuccess) {
                msg("Connection failed");
                finish();                        //basically it finishes the current activity from stack if
            }                                    //connection fails.
            else {
                msg("Connected");
                isBtconnected = true;
            }
            progress.dismiss();
        }
    }


    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == 0) {
            if (resultCode == TextToSpeech.Engine.CHECK_VOICE_DATA_PASS) {
                textToSpeech = new TextToSpeech(getApplicationContext(), new TextToSpeech.OnInitListener() {
                    @Override
                    public void onInit(int status) {
                        if (status == TextToSpeech.SUCCESS) {
                            textToSpeech.setLanguage(Locale.US);
                            msg("Text to speech success");
                        } else if (status == TextToSpeech.ERROR) {
                            msg("Sorry Text to speech failed");
                        } else {
                            //Nothing to do
                        }
                    }
                });
            } else {
                Intent installtts = new Intent();
                installtts.setAction(TextToSpeech.Engine.ACTION_INSTALL_TTS_DATA);
                startActivity(installtts);
            }
        }
    }

    @Override
    public void onStart() {
        super.onStart();
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client.connect();
        Intent checkttsintent = new Intent();
        checkttsintent.setAction(TextToSpeech.Engine.ACTION_CHECK_TTS_DATA);
        startActivityForResult(checkttsintent, 0);
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        Action viewAction = Action.newAction(
                Action.TYPE_VIEW, // TODO: choose an action type.
                "Processing Page", // TODO: Define a title for the content shown.
                // TODO: If you have web page content that matches this app activity's content,
                // make sure this auto-generated web page URL is correct.
                // Otherwise, set the URL to null.
                Uri.parse("http://host/path"),
                // TODO: Make sure this auto-generated app URL is correct.
                Uri.parse("android-app://com.gmail.saqib1707.signaloud/http/host/path")
        );
        AppIndex.AppIndexApi.start(client, viewAction);
    }

    @Override
    public void onPause() {
        if (textToSpeech != null) {
            textToSpeech.stop();
            textToSpeech.shutdown();
        }
        super.onPause();
    }

    @Override
    public void onResume() {
        super.onResume();
        myThread = new ConnectedThread(btSocket);
        myThread.start();
    }

    public class ConnectedThread extends Thread {
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        //creation of the connect thread
        public ConnectedThread(BluetoothSocket socket) {
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            try {
                //Create I/O streams for connection
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) {
            }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public void run() {
            byte[] buffer = new byte[256];
            int bytes;

            // Keep looping to listen for received messages
            while (true) {
                try {
                    bytes = mmInStream.read(buffer);            //read bytes from input buffer
                    String readMessage = new String(buffer, 0, bytes);
                    // Send the obtained bytes to the UI Activity via handler
                    myHandler.obtainMessage(0, bytes, -1, readMessage).sendToTarget();
                } catch (IOException e) {
                    break;
                }
            }
        }
    }


}
