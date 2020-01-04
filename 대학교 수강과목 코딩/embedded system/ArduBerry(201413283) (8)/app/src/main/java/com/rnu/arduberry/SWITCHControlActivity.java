package com.rnu.arduberry;

import android.app.Activity;
import android.os.Bundle;
import android.os.StrictMode;
import android.view.View;
import android.widget.Button;


public class SWITCHControlActivity  extends Activity
{
    private static final String TAG = "SWITCHControlActivity";

    private boolean 				btn1_on =false, btn2_on =false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.switch_layout);

        StrictMode.enableDefaults();
       // mContext = this;
        initUI();
    }

    public void onStart() {
        super.onStart();
    }

    @Override
    public synchronized void onResume() {
        super.onResume();
    }

    @Override
    public synchronized void onPause() {
        super.onPause();
    }

    @Override
    public void onStop() {
        super.onStop();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    private void initUI() {
        /*
        mBTN1 = (Button)findViewById(R.id.btBTN);
        mBTN1.setVisibility(View.VISIBLE);
        mBTN1.setBackgroundResource(R.drawable.selector_bt_btn);
        mBTN1.setSelected(false);

        mBTN2 = (Button)findViewById(R.id.btBTN);
        mBTN2.setVisibility(View.VISIBLE);
        mBTN2.setBackgroundResource(R.drawable.selector_bt_btn);
        mBTN2.setSelected(false);
        */

    }

    public void clickFunc(View v) {
        switch (v.getId()) {
            case R.id.btn8:
                ((Button) v).setText((btn1_on == false) ? "Button1 (On)" : "Button1 (Off)");
                btn1_on = !btn1_on;
                break;

            case R.id.btn9:
                ((Button) v).setText((btn2_on == false) ? "Button2 (On)" : "Button2 (Off)");
                btn2_on = !btn2_on;
                break;
        }



    }
}
