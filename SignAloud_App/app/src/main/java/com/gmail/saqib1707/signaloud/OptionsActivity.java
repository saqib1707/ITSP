package com.gmail.saqib1707.signaloud;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.common.api.GoogleApiClient;

import java.io.IOException;
import java.util.UUID;

public class OptionsActivity extends AppCompatActivity {
    Button playButton,send,disconnect;
    EditText actionName;
    ProgressDialog progress;
    UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
     static BluetoothSocket btSocket = null;
    BluetoothAdapter btAdapter ;
    boolean isBtconnected = false;
    String address,action;
    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient client;

    public void msg(String str,int time) {
        final Toast toast = Toast.makeText(getApplicationContext(), str, Toast.LENGTH_SHORT);
        toast.show();
        Handler myTimeHandler = new Handler();
        myTimeHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                toast.cancel();
            }
        }, time);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_options);
        playButton = (Button) findViewById(R.id.button_playmode);
        send=(Button)findViewById(R.id.button_send);
        disconnect = (Button) findViewById(R.id.button_disconnect);
        actionName=(EditText)findViewById(R.id.editText_actionname);
        Intent recoverIntent = getIntent();
        address=recoverIntent.getStringExtra(HomeActivity.EXTRA_ADDRESS);

        ConnectBluetooth bt = new ConnectBluetooth();
        bt.execute();

       /* calibrate.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {

                        if (btSocket != null) {
                            try {
                                btSocket.getOutputStream().write("C".getBytes());
                                msg("Calibrate sent to HC-05",500);
                            } catch (Exception e) {
                                msg("Unable to calibrate",500);
                            }
                        }
                    }
                }
        );*/
        send.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        action=actionName.getText().toString();
                        if (btSocket != null) {
                            try {
                                btSocket.getOutputStream().write(action.getBytes());
                                msg( action+"  sent to HC-05",500);
                            } catch (Exception e) {
                                msg("Unable to send",500);
                            }
                        }
                    }
                }
        );

        playButton.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if (btSocket != null) {
                            try {
                                btSocket.getOutputStream().write("P".getBytes());
                                msg("Play sent to HC-05",500);
                            } catch (Exception e) {
                                msg("Unable to go to play Mode",500);
                            }
                            Intent nextIntent = new Intent(OptionsActivity.this, ProcessingActivity.class);
                            startActivity(nextIntent);
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
                                msg("Disconnected",500);
                            } catch (Exception e) {
                                msg("Unable to Disconnect",500);
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
    public void onStart() {
        super.onStart();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client.connect();
        Action viewAction = Action.newAction(
                Action.TYPE_VIEW, // TODO: choose an action type.
                "Options Page", // TODO: Define a title for the content shown.
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
    public void onStop() {
        super.onStop();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        Action viewAction = Action.newAction(
                Action.TYPE_VIEW, // TODO: choose an action type.
                "Options Page", // TODO: Define a title for the content shown.
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
            progress = ProgressDialog.show(OptionsActivity.this, "Connecting to device", "Please wait man");
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
                msg("Connection failed",500);
                finish();                        //basically it finishes the current activity from stack if
            }                                    //connection fails.
            else {
                msg("Connected",500);
                isBtconnected = true;
            }
            progress.dismiss();
        }
    }
}
