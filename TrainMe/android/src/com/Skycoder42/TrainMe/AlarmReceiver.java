package com.Skycoder42.TrainMe;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

//DEBUG
import android.widget.Toast;
import android.util.Log;

public class AlarmReceiver extends BroadcastReceiver {
	public static final String INTENSE_EXTRA = "AlarmReceiver.Intense";

	@Override
	public void onReceive(Context context, Intent intent) {
		Log.d("TrainMe.AlarmReceiver", "A notification has been triggered");
		Toast.makeText(context, "NOTIFICATION!!!", Toast.LENGTH_LONG).show();
	}
}
