package com.gmail.saqib1707.signaloud;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.net.Uri;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.common.api.GoogleApiClient;

import java.util.ArrayList;
import java.util.Set;

public class HomeActivity extends AppCompatActivity {
    BluetoothAdapter btAdapter = null;
    Button pairedDevicesList;
    ListView devicesList;
    static String EXTRA_ADDRESS = "address";
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
        setContentView(R.layout.activity_home);
        btAdapter = BluetoothAdapter.getDefaultAdapter();
        pairedDevicesList = (Button) findViewById(R.id.button_paireddeviceslist);
        devicesList = (ListView) findViewById(R.id.listView_deviceslist);

        if (btAdapter == null) {
            msg("Bluetooth not supported");
            finish();
        } else if (!btAdapter.isEnabled()) {
            Intent btTurnOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(btTurnOn, 1);
            msg("Bluetooth turned on");
        } else {
            msg("Bluetooth already running");
        }
        //msg("Bluetooth turned on");
        //list of all bluetooth devices
        pairedDevicesList.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        listOfDevices();           //function call for listing all paired devices.
                    }
                }
        );
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();
    }

    public void listOfDevices() {
        ArrayList list = new ArrayList();
        Set<BluetoothDevice> pairedDevices = btAdapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice device : pairedDevices) {
                list.add(device.getName() + "\n" + device.getAddress());
                msg("Paired Devices:" + pairedDevices.size());
            }
            msg("Paired Devices added");
        } else {
            msg("No Bluetooth paired devices found");
        }
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_dropdown_item_1line, list);
        devicesList.setAdapter(adapter);
        devicesList.showContextMenu();
        devicesList.setOnItemClickListener(myItemClickListener);   //method called when device from the list of paired devices is called
    }

    private AdapterView.OnItemClickListener myItemClickListener = new AdapterView.OnItemClickListener() {
        public void onItemClick(AdapterView av, View v, int arg2, long arg3) {
            try {
                String info = ((TextView) v).getText().toString();
                String address = info.substring(info.length() - 17);
                Intent nextIntent = new Intent(HomeActivity.this, OptionsActivity.class);
                nextIntent.putExtra(EXTRA_ADDRESS, address);
                startActivity(nextIntent);
            } catch (Exception e) {
                msg("Error Occured");
            }
        }
    };

    @Override
    public void onStart() {
        super.onStart();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client.connect();
        Action viewAction = Action.newAction(
                Action.TYPE_VIEW, // TODO: choose an action type.
                "Home Page", // TODO: Define a title for the content shown.
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
                "Home Page", // TODO: Define a title for the content shown.
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
}
