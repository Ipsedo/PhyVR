package com.samuelberrien.phyvr.game;

import android.os.Bundle;
import android.view.KeyEvent;
import android.view.MotionEvent;
import com.google.vr.sdk.base.GvrActivity;

public class MyGvrActivity extends GvrActivity {

	private MyGvrView myGvrView;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		myGvrView = new MyGvrView(this);
		setContentView(myGvrView);
	}

	@Override
	protected void onResume() {
		super.onResume();
		myGvrView.onResume();
	}

	@Override
	protected void onPause() {
		myGvrView.onPause();
		super.onPause();
	}

	@Override
	public boolean dispatchGenericMotionEvent(MotionEvent e) {
		return myGvrView.onGenericMotionEvent(e);
	}

	@Override
	public boolean dispatchKeyEvent(KeyEvent event) {
		boolean handled = false;
		if (event.getAction() == KeyEvent.ACTION_DOWN)
			handled = myGvrView.onKeyDown(event.getKeyCode(), event);
		else if (event.getAction() == KeyEvent.ACTION_UP)
			handled = myGvrView.onKeyUp(event.getKeyCode(), event);
		return handled
				|| event.getKeyCode() != KeyEvent.KEYCODE_BACK
				|| super.dispatchKeyEvent(event);
	}

	@Override
	public void onDestroy() {
		myGvrView.free();
		super.onDestroy();
	}
}
