package mazerfaker.harduino.bluetooth;

import android.bluetooth.BluetoothSocket;
import android.os.Handler;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class ConnectedThread extends Thread {

    // Debugging for LOGCAT
    private static final String TAG = "ConnectionThread";

    private final InputStream mmInStream;
    private final OutputStream mmOutStream;
    private Handler bluetoothIn;

    // creation of the connect thread //
    public ConnectedThread(BluetoothSocket socket, Handler handler) {
        InputStream tmpIn = null;
        OutputStream tmpOut = null;

        try {
            //Create I/O streams for connection
            tmpIn = socket.getInputStream();
            tmpOut = socket.getOutputStream();
        } catch (IOException e) {
            Log.e(TAG, e.getMessage());
        }

        mmInStream = tmpIn;
        mmOutStream = tmpOut;
        bluetoothIn = handler;
    }


    public void run() {
        byte[] buffer = new byte[1];
        int bytes;

        // keep looping to listen for received messages //
        while (true) {
            try {
                // read bytes from input buffer //
                bytes = mmInStream.read(buffer);
                String readMessage = new String(buffer, 0, bytes);
                bluetoothIn.obtainMessage(Constants.MESSAGE_HANDLER_STATE, bytes, -1, readMessage).sendToTarget();

            } catch (IOException e) {
                Log.e(TAG, e.getMessage());
                break;
            }
        }
    }

    // write method //
    public void write(String input) {
        // converts entered String into bytes //
        byte[] msgBuffer = input.getBytes();

        try {
            //write bytes over BT connection via outstream //
            mmOutStream.write(msgBuffer);

        } catch (IOException e) {
            Log.e(TAG, e.getMessage());
        }
    }
}
