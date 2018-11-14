package com.siuryan.smartwatchcompanion;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import org.w3c.dom.Text;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Locale;
import java.util.UUID;

public class WatchControl extends AppCompatActivity {

    public static final String TEST_NOTIF = "This is a test";
    public static final String DATETIME_FORMAT = "yyyy/MM/dd HH:mm:ss";

    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    Button btn1, btn2, btnDis;
    String address = null;
    private ProgressDialog progress;
    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    private boolean isBtConnected = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Intent newInt = getIntent();
        address = newInt.getStringExtra(MainActivity.EXTRA_ADDRESS);

        setContentView(R.layout.activity_watch_control);

        btn1 = findViewById(R.id.button2);
        btn2 = findViewById(R.id.button3);
        btnDis = findViewById(R.id.button4);

        new ConnectBT().execute();

        btn1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View v) {
                sendSignal("1" + TEST_NOTIF);
            }
        });

        btn2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View v) {
                String timeStamp = new SimpleDateFormat(DATETIME_FORMAT, Locale.US).format(Calendar.getInstance().getTime());
                sendSignal(timeStamp);
            }
        });

        btnDis.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View v) {
                disconnect();
            }
        });
    }

    private void sendSignal ( String number ) {
        if (btSocket != null) {
            try {
                btSocket.getOutputStream().write(number.getBytes());
            } catch (IOException e) {
                msg("Error");
            }
        }
    }

    private void disconnect () {
        if (btSocket != null) {
            try {
                btSocket.close();
            } catch(IOException e) {
                msg("Error");
            }
        }

        finish();
    }

    private void msg (String s) {
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_LONG).show();
    }

    private class ConnectBT extends AsyncTask<Void, Void, Void> {
        private boolean connectSuccess = true;

        @Override
        protected void onPreExecute () {
            progress = ProgressDialog.show(WatchControl.this, "Connecting", "Please wait...");
        }

        @Override
        protected Void doInBackground (Void... devices) {
            try {
                if (btSocket == null || !isBtConnected) {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();
                    BluetoothDevice bluetoothDevice = myBluetooth.getRemoteDevice(address);
                    btSocket = bluetoothDevice.createInsecureRfcommSocketToServiceRecord(myUUID);
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();
                }
            } catch (IOException e) {
                connectSuccess = false;
            }

            return null;
        }

        @Override
        protected void onPostExecute (Void result) {
            super.onPostExecute(result);

            if (!connectSuccess) {
                msg("Connection Failed. Is it a SPP Bluetooth? Try again.");
                finish();
            } else {
                msg("Connected");
                isBtConnected = true;
            }

            progress.dismiss();
        }
    }
}