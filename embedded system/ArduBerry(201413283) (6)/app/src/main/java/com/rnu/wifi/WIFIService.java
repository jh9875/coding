package com.rnu.wifi;

import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;

import com.rnu.data.Constant;

public class WIFIService {

	private static final String TAG = "WIFIService";
	
	private final Handler mHandler;
	private String mHost;
	private int mPort;
	private int mState;

	private ConnectThread mConnectThread;
	private ConnectedThread mConnectedThread;
	
	public static final int STATE_NONE = 0;
	public static final int STATE_CONNECTING = 1;
	public static final int STATE_CONNECTED = 2;

	public WIFIService(Context context, Handler handler, String host, int port) {

		mHandler = handler;
		mHost = host;
		mPort = port;
		mState = STATE_NONE;

	}
	
	private synchronized void setState(int state) {
		mState = state;
		mHandler.obtainMessage(Constant.MESSAGE_STATE_CHANGE, state, -1).sendToTarget();
	}
	
	public synchronized int getState() {
		return mState;
	}
	
	public synchronized void start() {
		Log.d(TAG,"[HYOWON] >>start()");
		if(mConnectedThread != null) {mConnectedThread.cancel(); mConnectedThread = null;}
		
		setState(STATE_CONNECTING);

//		if(mConnectThread == null) {

            Log.d(TAG,"[HYOWON] >>start() mHost :" + mHost + ", mPort : " + mPort);

			mConnectThread = new ConnectThread(mHost, mPort);
			mConnectThread.start();
//		}
		
	}
	
	public synchronized void connect(Socket socket) {
		
		if(mConnectThread != null) {mConnectThread.cancel(); mConnectThread = null;};
		
		if (mConnectedThread != null) {mConnectedThread.cancel(); mConnectedThread = null;}
		
		mConnectedThread = new ConnectedThread(socket);
		mConnectedThread.start();
		
		setState(STATE_CONNECTED);
	}
	
	public synchronized void stop() {
		
		if(mConnectThread != null) {
			mConnectThread.cancel();
			mConnectThread = null;
		}
		
		if(mConnectedThread != null) {
			mConnectedThread.cancel();
			mConnectedThread = null;
		}
		
		setState(STATE_NONE);
	}
	
	public void write(byte[] out) {
		
		ConnectedThread r;
		
		synchronized (this) {
			if(mState != STATE_CONNECTED) return;
			r = mConnectedThread;
		}
		
		r.write(out);
	}
	
	private void connectionFailed() {
		Log.d(TAG,"[HYOWON] >>connectionFailed()");
        // Send a failure message back to the Activity
        Message msg = mHandler.obtainMessage(Constant.MESSAGE_TOAST);
        Bundle bundle = new Bundle();
        bundle.putString(Constant.TOAST, "Unable to connect device");
        msg.setData(bundle);
        mHandler.sendMessage(msg);

        // Start the service over to restart listening mode
        //WIFIService.this.start();
	}

    private void connectionLost() {
    	Log.d(TAG,"[HYOWON] >>connectionLost()");
        // Send a failure message back to the Activity
        Message msg = mHandler.obtainMessage(Constant.MESSAGE_TOAST);
        Bundle bundle = new Bundle();
        bundle.putString(Constant.TOAST, "Device connection was lost");
        msg.setData(bundle);
        mHandler.sendMessage(msg);

        // Start the service over to restart listening mode
        //WIFIService.this.start();
    }

	private class ConnectThread extends Thread {
		private final Socket mmSocket;
		private final String mmHost;
		private final int mmPort;

		public ConnectThread(String host, int port) {

			mmSocket = new Socket();
            Log.e(TAG, "ConnectThread() host : " + host + ", port : " + port);
			mmHost = host;
			mmPort = port;
		}
		
		public void run() {
			try {
                Log.e(TAG, "run() 1");
				mmSocket.connect(new InetSocketAddress(mmHost, mmPort));

				Log.e(TAG, "run() 2 mmPort : " + mmPort + ", mmHost : " + mmHost);
			} catch (IOException e) {
				try {
					mmSocket.close();
				} catch (IOException e2) {
					Log.e(TAG, "unable to close() socket during connection failure", e2);
				}
				Log.e(TAG, " connection failure :" +e.getMessage());
				connectionFailed();
				return;
			}
			
			connect(mmSocket);
		}
		
		public void cancel() {
//			try {
//				mmSocket.close();
//			} catch (IOException e) {
//				Log.e(TAG, "close() of connect socket failed", e);
//			}
		}
	}
	
	private class ConnectedThread extends Thread {
		private final Socket mmSocket;
		private final InputStream mmInStream;
		private final OutputStream mmOutStream;
		
		public ConnectedThread(Socket socket) {
			mmSocket = socket;
			InputStream tmpIn = null;
			OutputStream tmpOut = null;
			
			try {
				tmpIn = socket.getInputStream();
				tmpOut = socket.getOutputStream();
			} catch (IOException e) {
				Log.e(TAG, "temp sockets not created", e);
			}
			
			mmInStream = tmpIn;
			mmOutStream = tmpOut;
		}
		
		public void run() {
		        byte[] buffer = new byte[230400];
	            boolean isStart = false;
	            boolean isImage = false;
	            byte head = 0;
	            byte bData = 0;
	            byte bData2 = 0;
	            byte bData3 = 0;
	            int nCnt = 0;

	            // Keep listening to the InputStream while connected
	            while (true) {
	                try {
	                	
	                	// Header check
	                	if(!isStart) {
	                		head = (byte) mmInStream.read();
							Log.i(TAG, "head = " + (char)head);
	                		if(head == '@') {
	                			isStart = true;
	                			for(int i=0; i < 10; i++) {
	                				buffer[i] = 0;
	                			}
	                		}
	                	} else {
	                		bData = (byte) mmInStream.read();
							Log.i(TAG, "bData = " + (char)bData);

	                		if(bData == '#') {
	                			if(isImage) {
	                				bData2 = (byte) mmInStream.read();
	                				if(bData2 == '#') {
	                					bData3 = (byte) mmInStream.read();
	                					if(bData3 == '#') {
	                						byte[] sendBuffer = new byte[230400];
	                						for(int i=0; i < nCnt; i++) {
	                							sendBuffer[i] = buffer[i];
	                						}
	                						mHandler.obtainMessage(Constant.MESSAGE_READ, nCnt, 1, sendBuffer).sendToTarget();
	                						isImage = false;
	                						isStart = false;
	                						nCnt = 0;
	                					} else {
	                    					buffer[nCnt++] = bData;
	                    					buffer[nCnt++] = bData2;
	                    					buffer[nCnt++] = bData3;
	                					}
	                				} else {
	                					buffer[nCnt++] = bData;
	                					buffer[nCnt++] = bData2;
	                				}
	                			} else {
	                				byte[] sendBuffer = new byte[10];
	                				for(int i=0; i < nCnt; i++) {
	                					sendBuffer[i] = buffer[i];
	                				}
		                			mHandler.obtainMessage(Constant.MESSAGE_READ, nCnt, -1, sendBuffer).sendToTarget();
		                			isStart = false;
		                			nCnt = 0;
	                			}
	                		} else {
	                			buffer[nCnt++] = bData;
								Log.i(TAG, "bData = " + (char)bData);
	                			if(nCnt == 3) {
	                				String strHead = new String(buffer, 0, 3);
	                				if("IMG".equals(strHead)) {
	                					isImage =true;
	                					mHandler.obtainMessage(Constant.MESSAGE_START_IMAGE).sendToTarget();
	                				} else {
	                					isImage = false;
	                				}
	                			}
	                		}
	                		
	                	}
	                } catch (IOException e) {
	                    Log.e(TAG, "disconnected", e);
	                    connectionLost();
	                    break;
	                }
	            }
		}
		
		public void write(byte[] buffer) {
			try {
				mmOutStream.write(buffer);
				
				mHandler.obtainMessage(Constant.MESSAGE_WRITE, -1, -1, buffer).sendToTarget();
			} catch (IOException e) {
				Log.e(TAG, "Exception during write", e);
			}
		}
		
		public void cancel() {
			try {
				mmSocket.close();
			} catch (IOException e) {
				Log.e(TAG, "close() of connect socket failed", e);
			}
		}
	}
}
