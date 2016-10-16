package mazerfaker.harduino.bluetooth;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;

import java.io.IOException;
import java.util.UUID;

public class MainActivity extends Activity {
    // Debugging for LOGCAT
    private static final String TAG = "MainActivity";

    private ConnectedThread mConnectedThread;
    private BluetoothAdapter btAdapter = null;
    private BluetoothSocket btSocket = null;

    private boolean flag = false;

    // SPP UUID service - this should work for most devices //
    private static final UUID BTMODULEUUID = UUID.fromString(Constants.DEFAULT_UUID);


    private final Handler bluetoothIn = new Handler() {
        private String message = "";

        public void handleMessage(Message msg) {
            if (msg.what == Constants.MESSAGE_HANDLER_STATE) {
                String readMessage = (String) msg.obj;
                if(readMessage.equals("~")) {
                    message = "";
                } else {
                    message += readMessage;
                }

                if(message.equals("Led on")) {
                    Toast.makeText(getBaseContext(), message, Toast.LENGTH_SHORT).show();
                } else if(message.equals("Led off")) {
                    Toast.makeText(getBaseContext(), message, Toast.LENGTH_SHORT).show();
                }

            }
        }
    };

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Link the buttons and textViews to respective views //
        Button btnSend = (Button) findViewById(R.id.buttonSend);

        // get Bluetooth adapter //
        btAdapter = BluetoothAdapter.getDefaultAdapter();
        checkBTState();

        // Set up onClick listeners //
        btnSend.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                if(flag) {
                    mConnectedThread.write("~on");
                    flag = false;
                } else {
                    mConnectedThread.write("~off");
                    flag = true;
                }
            }
        });
    }

    // creates secure outgoing connecetion with BT device using UUID //
    private BluetoothSocket createBluetoothSocket(BluetoothDevice device) throws IOException {
        return  device.createRfcommSocketToServiceRecord(BTMODULEUUID);
    }

    @Override
    public void onResume() {
        super.onResume();

        // Get MAC address from DeviceListActivity via intent //
        Intent intent = getIntent();

        // Get the MAC address from the DeviceListActivty via EXTRA that we have set on DeviceListActivity //
        String address = intent.getStringExtra(Constants.EXTRA_DEVICE_ADDRESS);

        // create device and set the MAC address //
        BluetoothDevice device = btAdapter.getRemoteDevice(address);

        try {
            btSocket = createBluetoothSocket(device);
        } catch (IOException e) {
            Toast.makeText(getBaseContext(), "Socket creation failed", Toast.LENGTH_LONG).show();
            Log.e(TAG, e.getMessage());
        }

        // Establish the Bluetooth socket connection //
        try
        {
            btSocket.connect();
        } catch (IOException e) {
            Log.e(TAG, e.getMessage());
            Log.e(TAG, "Try to close connection...");
            try
            {
                btSocket.close();
            } catch (IOException e2) {
                Log.e(TAG, "Failed to connect");
                Log.e(TAG, e.getMessage());
            }
        }

        mConnectedThread = new ConnectedThread(btSocket, bluetoothIn);
        mConnectedThread.start();

        /*********************************************************************************************
         *
         * send a character when resuming.beginning transmission to check device is connected
         * if it is not an exception will be thrown in the write method and finish() will be called
         *
         *********************************************************************************************/

        mConnectedThread.write("x");
    }

    @Override
    public void onPause()
    {
        super.onPause();
        try
        {
            // Don't leave Bluetooth sockets open when leaving activity //
            btSocket.close();
        } catch (IOException e2) {
            Log.e(TAG, e2.getMessage());
        }
    }

    // Checks that the Android device Bluetooth is available and prompts to be turned on if off //
    private void checkBTState() {

        if(btAdapter==null) {
            String toastMessage = getResources().getText(R.string.bluetooth_not_supported).toString();
            Toast.makeText(getBaseContext(), toastMessage, Toast.LENGTH_LONG).show();
        } else {
            if (!btAdapter.isEnabled()) {
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, 1);
            }
        }
    }

}