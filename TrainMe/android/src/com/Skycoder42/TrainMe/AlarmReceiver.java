package com.Skycoder42.TrainMe;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.graphics.BitmapFactory;
import android.app.NotificationManager;
import android.app.Notification;
import android.app.PendingIntent;
import android.app.AlarmManager;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.NotificationCompat.BigTextStyle;
import java.util.Date;

//DEBUG
import android.widget.Toast;
import android.util.Log;

public class AlarmReceiver extends BroadcastReceiver {
	public static final String ID_EXTRA = "AlarmReceiver.Id";
	public static final String INTENSE_EXTRA = "AlarmReceiver.Intense";
	public static final String GROUP_KEY = "AlarmReceiver.NotificationGroup";

	@Override
	public void onReceive(Context context, Intent intent) {
		//test allowed
		if(ReminderController.testDateSkipped(context, new Date())) {
			Log.d("TrainMe.AlarmReceiver", "Notification skipped");
			return;
		}

		int id = intent.getIntExtra(ID_EXTRA, -1);
		if(id == -1) {
			Log.e("TrainMe.AlarmReceiver", "Invalid notification ID!");
			return;
		}
		boolean intense = intent.getBooleanExtra(INTENSE_EXTRA, false);

		NotificationManager manager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
		Intent openIntent = new Intent(context, MainActivity.class);
		PendingIntent pending = PendingIntent.getActivity(context,
			ReminderController.OPEN_INTENT_ID,
			openIntent,
			PendingIntent.FLAG_UPDATE_CURRENT);

		NotificationCompat.Builder builder = new NotificationCompat.Builder(context)
			.setContentInfo("Train-Me! Reminder")
			.setContentIntent(pending)
			.setLargeIcon(BitmapFactory.decodeResource(context.getResources(), R.drawable.icon))
			.setSmallIcon(R.drawable.icon)
			.setAutoCancel(true)
			.setCategory(NotificationCompat.CATEGORY_REMINDER)
			.setDefaults(Notification.DEFAULT_ALL)
			.setGroup(GROUP_KEY);

		Notification notification;
		if(intense) {
			notification = builder.setContentTitle("YOU NEED TO TRAIN!!!")
				   .setContentText("I want to train!")
				   .setPriority(NotificationCompat.PRIORITY_HIGH)
				   .setStyle(new NotificationCompat.BigTextStyle()
					   .bigText("Please, just let me train already. My Body is ready!"))
				   .build();

			//notification.flags |= Notification.FLAG_INSISTENT;
		} else {
			notification = builder.setContentTitle("Do your Training!")
				   .setContentText("It's time for training!")
				   .setStyle(new NotificationCompat.BigTextStyle()
					   .bigText("It's time for your daily sports training! Open Train-Me! to start your Training!"))
					.build();
		}

		manager.notify(id, notification);
	}
}
