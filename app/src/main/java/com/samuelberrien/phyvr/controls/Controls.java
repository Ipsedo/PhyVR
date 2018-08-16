package com.samuelberrien.phyvr.controls;

import android.content.Context;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import com.samuelberrien.phyvr.controls.axis.Axis;

import java.util.ArrayList;

public class Controls {


	private final long controlPtr;


	private float dir;
	private float turret;
	private float canon;
	private float speed;

	boolean brake;
	boolean respawn;
	boolean fire;

	private Axis dirAxis;
	private Axis speedAxis;
	private Axis turretAxis;
	private Axis canonAxis;

	static {
		System.loadLibrary("phyvr");
	}

	public Controls(Context context, long controlPtr) {
		this.controlPtr = controlPtr;
		dir = 0.f;
		turret = 0.f;
		canon = 0.f;
		speed = 0.f;

		brake = false;
		respawn = false;
		fire = false;

		dirAxis = new Axis(context, Axis.AxisMap.DIR);
		dirAxis.addListener((float value) -> {
				dir = value;
			}
		);

		speedAxis = new Axis(context, Axis.AxisMap.SPEED);
		speedAxis.addListener((float value) -> {
			speed = value;
		});

		turretAxis = new Axis(context, Axis.AxisMap.TURRET);
		turretAxis.addListener((float value) -> {
			turret = value;
		});

		canonAxis = new Axis(context, Axis.AxisMap.CANON);
		canonAxis.addListener((float value) -> {
			canon = value;
		});
	}

	public boolean onMotionEvent(MotionEvent event) {
		dirAxis.onGenericMotion(event);
		speedAxis.onGenericMotion(event);
		turretAxis.onGenericMotion(event);
		canonAxis.onGenericMotion(event);

		control(controlPtr, dir, speed, brake, turret, canon, respawn, fire);

		return false;
	}

	public boolean onKeyDown(int keyCode, KeyEvent event) {
		boolean handled = false;

		//control(controlPtr, dir, speed, brake, turret, canon, respawn, fire);

		return handled;
	}

	public void onKeyUp(int keyCode, KeyEvent event) {

	}

	public void initControllerIds() throws NoControllerException {
		ArrayList<Integer> gameControllerDeviceIds = new ArrayList<>();
		int[] deviceIds = InputDevice.getDeviceIds();
		for (int deviceId : deviceIds) {
			InputDevice dev = InputDevice.getDevice(deviceId);
			int sources = dev.getSources();

			// Verify that the device has gamepad buttons, control sticks, or both.
			if (((sources & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD)
					|| ((sources & InputDevice.SOURCE_JOYSTICK)
					== InputDevice.SOURCE_JOYSTICK)) {
				// This device is a game controller. Store its device ID.
				if (!gameControllerDeviceIds.contains(deviceId)) {
					gameControllerDeviceIds.add(deviceId);
				}
			}
		}
		if (gameControllerDeviceIds.isEmpty()) {
			throw new NoControllerException();
		}
		int controllerId = gameControllerDeviceIds.get(0);
	}

	private class NoControllerException extends Exception {

	}

	public native void control(long controlPtr,
							   float direction,
							   float speed,
							   boolean brake,
							   float turretDir,
							   float turretUp,
							   boolean respawn,
							   boolean fire);

	public native void control2(long controlPtr, float[] arrayControl);
}
