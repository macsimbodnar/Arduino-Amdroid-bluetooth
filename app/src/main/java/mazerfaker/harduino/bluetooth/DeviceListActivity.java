package mazerfaker.harduino.bluetooth;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Set;


public class DeviceListActivity extends Activity {

    // Debugging for LOGCAT
    private static final String TAG = "DeviceListActivity";

    TextView textView1;
    private BluetoothAdapter mBtAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.device_list);
    }

    @Override
    public void onResume()
    {
        super.onResume();
        checkBTState();

        textView1 = (TextView) findViewById(R.id.connecting);
        textView1.setTextSize(Constants.TEXT_SIZE);
        textView1.setText(Constants.BLANK);

        // Initialize array adapter for paired devices //
        ArrayAdapter<String> mPairedDevicesArrayAdapter;
        mPairedDevicesArrayAdapter = new ArrayAdapter<String>(this, R.layout.device_name);

        // Find and set up the ListView for paired devices //
        ListView pairedListView = (ListView) findViewById(R.id.paired_devices);
        pairedListView.setAdapter(mPairedDevicesArrayAdapter);
        pairedListView.setOnItemClickListener(mDeviceClickListener);

        // Get the local Bluetooth adapter //
        mBtAdapter = BluetoothAdapter.getDefaultAdapter();

        // Get a set of currently paired devices and append to 'pairedDevices' //
        Set<BluetoothDevice> pairedDevices = mBtAdapter.getBondedDevices();

        // Add previosuly paired devices to the array //
        if (pairedDevices.size() > 0) {
            // make title viewable //
            findViewById(R.id.title_paired_devices).setVisibility(View.VISIBLE);
            for (BluetoothDevice device : pairedDevices) {
                mPairedDevicesArrayAdapter.add(device.getName() + Constants.NEW_LINE + device.getAddress());
            }
        } else {
            String noDevices = getResources().getText(R.string.none_paired).toString();
            mPairedDevicesArrayAdapter.add(noDevices);
        }
    }

    // Set up on-click listener for the list //
    private OnItemClickListener mDeviceClickListener = new OnItemClickListener() {
        public void onItemClick(AdapterView<?> av, View v, int arg2, long arg3) {

            textView1.setText(getResources().getText(R.string.connecting).toString());
            // Get the device MAC address, which is the last 17 chars in the View //
            String info = ((TextView) v).getText().toString();
            String address = info.substring(info.length() - Constants.MAC_ADRESS_LENGHT);

            // Make an intent to start next activity while taking an extra which is the MAC address. //
            Intent i = new Intent(DeviceListActivity.this, MainActivity.class);
            i.putExtra(Constants.EXTRA_DEVICE_ADDRESS, address);
            startActivity(i);
        }
    };

    // Check device has Bluetooth and that it is turned on //
    private void checkBTState() {

        mBtAdapter = BluetoothAdapter.getDefaultAdapter();
        if(mBtAdapter == null) {
            String message = getResources().getText(R.string.bluetooth_not_supported).toString();
            Toast.makeText(getBaseContext(), message, Toast.LENGTH_SHORT).show();
        } else {
            if (mBtAdapter.isEnabled()) {
                Log.d(TAG, "...Bluetooth ON...");
            } else {
                // Prompt user to turn on Bluetooth //
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, 1);
            }
        }
    }


}
