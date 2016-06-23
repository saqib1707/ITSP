package com.gmail.saqib1707.btcontrolledlight;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Set;

public class PairedDevicesList extends AppCompatActivity {
    Button pairedDevicesList;
    BluetoothAdapter btAdapter;
    ListView devicesList;
    static String EXTRA_ADDRESS="address";

    public void msg(String str){
        Toast.makeText(getApplicationContext(),str,Toast.LENGTH_SHORT).show();
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_paired_devices_list);
        pairedDevicesList=(Button)findViewById(R.id.button_paireddeviceslist);
        btAdapter=BluetoothAdapter.getDefaultAdapter();
        pairedDevicesList.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        listOfDevices();
                    }
                }
        );
    }
    public void listOfDevices(){
        ArrayList list=new ArrayList();
        Set<BluetoothDevice> pairedDevices=btAdapter.getBondedDevices();
        if(pairedDevices.size() > 0){
            for(BluetoothDevice device:pairedDevices){
                list.add(device.getName()+"\n"+device.getAddress());
                msg("Paired Devices:"+pairedDevices.size());
            }
            msg("Paired Devices added");
        }
        else{
            msg("No Bluetooth paired devices found");
        }
        ArrayAdapter<String> adapter=new ArrayAdapter<String>(this,android.R.layout.simple_dropdown_item_1line,list);
        devicesList.setAdapter(adapter);
        devicesList.showContextMenu();
        devicesList.setOnItemClickListener(myItemClickListener);
    }
    AdapterView.OnItemClickListener myItemClickListener=new AdapterView.OnItemClickListener(){
        public void onItemClick(AdapterView av, View v, int arg2, long arg3){
            try{
                String info=((TextView)v).getText().toString();
                String address=info.substring(info.length()-17);
                Intent nextintent=new Intent(PairedDevicesList.this,LedControl.class);
                nextintent.putExtra(EXTRA_ADDRESS,address);
                startActivity(nextintent);
            }catch(Exception e){
                msg("Error occured");
            }
        }
    };
}
