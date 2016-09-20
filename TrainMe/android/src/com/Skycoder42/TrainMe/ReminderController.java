package com.Skycoder42.TrainMe;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.Intent;
import android.preference.PreferenceManager;
import android.graphics.BitmapFactory;
import android.app.NotificationManager;
import android.app.Notification;
import android.app.PendingIntent;
import android.support.v4.app.NotificationCompat;

public class ReminderController {
	private static final int STATUS_NOT_KEY = 42;

	private static final String ACTIVE_KEY = "Reminder.Active";
	private static final String VISIBLE_KEY = "Reminder.Visible";
	private static final String GIFTAG_KEY = "Reminder.Giftag";

	private Context context;
	private SharedPreferences prefs;

	public ReminderController(Context context) {
		this.context = context;
		this.prefs = PreferenceManager.getDefaultSharedPreferences(context);
	}

	public boolean isActive() {
		return this.prefs.getBoolean(ACTIVE_KEY, false);
	}

	public boolean isAlwaysVisible() {
		return this.prefs.getBoolean(VISIBLE_KEY, false);
	}

	public String gifTag() {
		return this.prefs.getString(GIFTAG_KEY, "");
	}

	public void activateReminders(boolean activate) {
		if(activate == this.isActive())
			return;

		this.prefs
			.edit()
			.putBoolean(ACTIVE_KEY, activate)
			.apply();

		if(activate) {
			this.showAlways(this.isAlwaysVisible());
		} else {
			NotificationManager manager = (NotificationManager) this.context.getSystemService(Context.NOTIFICATION_SERVICE);
			manager.cancelAll();
		}
	}

	public void showAlways(boolean always) {
		this.prefs
			.edit()
			.putBoolean(VISIBLE_KEY, always)
			.apply();

		NotificationManager manager = (NotificationManager) this.context.getSystemService(Context.NOTIFICATION_SERVICE);
		if(always) {
			Intent intent = new Intent(this.context, MainActivity.class);
			PendingIntent pending = PendingIntent.getActivity(this.context, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);

			Notification notification = new NotificationCompat.Builder(this.context)
				.setContentTitle("Train-Me! Reminders active")
				.setContentText("You will be notified for your training.")
				.setContentIntent(pending)
				.setLargeIcon(BitmapFactory.decodeResource(this.context.getResources(), R.drawable.icon))
				.setSmallIcon(R.drawable.icon)
				.setAutoCancel(false)
				.setCategory(NotificationCompat.CATEGORY_STATUS)
				.setOngoing(true)
				.setPriority(NotificationCompat.PRIORITY_MIN)
				.build();

			manager.notify(STATUS_NOT_KEY, notification);
		} else
			manager.cancel(STATUS_NOT_KEY);
	}
}
