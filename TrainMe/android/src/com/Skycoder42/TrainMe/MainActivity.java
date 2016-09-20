package com.Skycoder42.TrainMe;

import org.qtproject.qt5.android.bindings.QtActivity;
import android.os.Bundle;
import android.content.Intent;
import android.view.WindowManager;
import android.view.Window;
import android.graphics.Color;
import android.util.DisplayMetrics;
import android.widget.Toast;

public class MainActivity extends QtActivity {

    @Override
    public void onCreate(Bundle savedInstanceState)
	{
		Window window = this.getWindow();
		window.clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
		window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
		window.setStatusBarColor(Color.parseColor("#D32F2F"));

		super.onCreate(savedInstanceState);
	}

	public double getDpiFactor() {
		DisplayMetrics metrics = new DisplayMetrics();
		this.getWindowManager()
			.getDefaultDisplay()
			.getMetrics(metrics);

		return metrics.density;
	}

	public void showToast(String message, boolean isLong) {
		Toast.makeText(this, message, isLong ? Toast.LENGTH_LONG : Toast.LENGTH_SHORT).show();
	}
}
