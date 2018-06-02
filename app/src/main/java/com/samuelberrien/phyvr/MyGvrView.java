package com.samuelberrien.phyvr;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.view.MotionEvent;

import com.google.vr.sdk.base.Eye;
import com.google.vr.sdk.base.GvrView;
import com.google.vr.sdk.base.HeadTransform;
import com.google.vr.sdk.base.Viewport;
import com.samuelberrien.phyvr.controls.Controls;
import com.samuelberrien.phyvr.utils.LoadImage;

import javax.microedition.khronos.egl.EGLConfig;

public class MyGvrView extends GvrView implements GvrView.StereoRenderer {

	public MyGvrView(Context context) {
		super(context);
		setRenderer(this);
	}

	public MyGvrView(Context context, AttributeSet attrs) {
		super(context, attrs);
		setRenderer(this);
	}

	@Override
	public void onPause() {
		super.onPause();
	}

	@Override
	public void onResume() {
		super.onResume();
	}

	@Override
	public boolean onGenericMotionEvent(MotionEvent motionEvent) {
		controls.onMotionEvent(motionEvent);
		return super.onGenericMotionEvent(motionEvent);
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		controls.onKeyDown(keyCode, event);
		return true;
	}

	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		controls.onKeyUp(keyCode, event);
		return true;
	}

	/**
	 * Stereo Renderer stuff
	 */

	private final float Z_NEAR = 0.1f;
	private final float Z_FAR = 500f;

	static {
		System.loadLibrary("phyvr");
	}

	private long entitiesPtr;
	private long rendererPtr;
	private long enginePtr;
	private long playerPtr;
	private long controlPtr;

	private Controls controls;

	@Override
	public void onNewFrame(HeadTransform headTransform) {
		float[] mHeadView = new float[16];
		headTransform.getHeadView(mHeadView, 0);
		willDrawRenderer(rendererPtr, mHeadView);
		updateLevel(enginePtr);
		//addBox(getContext().getAssets(), boxesPtr);
	}

	@Override
	public void onDrawEye(Eye eye) {
		float[] eyeView = eye.getEyeView();

		float[] mProjectionMatrix = eye.getPerspective(Z_NEAR, Z_FAR);
		drawRenderer(rendererPtr, mProjectionMatrix, eyeView, new float[4], new float[3]);
	}

	@Override
	public void onFinishFrame(Viewport viewport) {
	}

	@Override
	public void onSurfaceChanged(int width, int height) {
	}

	@Override
	public void onSurfaceCreated(EGLConfig config) {
		//boxesPtr = initBoxes(getContext().getAssets());
		LoadImage loadImage = new LoadImage(getContext(), "heightmap/heightmap6.png");
		entitiesPtr = initEntity(getContext().getAssets(),
				loadImage.tofloatGreyArray(), loadImage.getWidth(), loadImage.getHeight());
		enginePtr = initEngine(entitiesPtr);
		rendererPtr = initRenderer(entitiesPtr);
		playerPtr = initPlayer(getContext().getAssets(), enginePtr, rendererPtr, entitiesPtr);
		controlPtr = getControlPtrFromPlayer(playerPtr);
		controls = new Controls(getContext(), controlPtr);
	}

	@Override
	public void onRendererShutdown() {
		freeBoxes(entitiesPtr);
		freeLevel(enginePtr);
		freeRenderer(rendererPtr);
	}


	/**
	 * CPP wrappers
	 */

	public native long initEntity(AssetManager assetManager, float[] heightmap, int width, int height);

	public native long initPlayer(AssetManager assetManager, long levelPtr, long rendererPtr, long entityPtr);

	public native long getControlPtrFromPlayer(long carPtr);

	public native long initEngine(long boxesPtr);

	public native long initRenderer(long boxesPtr);

	public native void addBox(AssetManager assetManager, long boxesPtr);

	public native void willDrawRenderer(long rendererPtr, float[] mHeadView);

	public native void drawRenderer(long rendererPtr,
									float[] mEyeProjectionMatrix,
									float[] mEyeViewMatrix,
									float[] myLighPosInEyeSpace,
									float[] mCameraPos);

	public native void updateLevel(long levelptr);

	public native void freeBoxes(long boxesPtr);

	public native void freeLevel(long levelPtr);

	public native void freeRenderer(long rendererPtr);

}
