package com.Skycoder42.TrainMe;

import org.qtproject.qt5.android.bindings.QtActivity;
import android.os.Bundle;
import android.content.Intent;
import android.view.WindowManager;
import android.view.Window;
import android.graphics.Color;

public class MainActivity extends QtActivity {
    public static MainActivity s_activity = null;

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
		s_activity = this;

		Window window = this.getWindow();
		window.clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
		window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
		window.setStatusBarColor(Color.parseColor("#D32F2F"));

		super.onCreate(savedInstanceState);
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        s_activity = null;
	}
}
