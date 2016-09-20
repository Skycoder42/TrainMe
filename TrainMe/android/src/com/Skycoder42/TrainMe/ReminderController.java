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
	private static final String REM_COUNT_KEY = "Reminder.Reminders.Count";
	private static final String REM_BASE_KEY = "Reminder.Reminders.Rem_";
	private static final String REM_SUB_HOURS_KEY = ".Hours";
	private static final String REM_SUB_MIN_KEY = ".Mins";
	private static final String REM_SUB_INTENSE_KEY = ".Intense";

	public static class ReminderInfo {
		public int hours;
		public int minutes;
		public boolean intense;
	}

	private Context context;
	private SharedPreferences prefs;

	public ReminderController(Context context) {
		this.context = context;
		this.prefs = PreferenceManager.getDefaultSharedPreferences(context);

		if(this.isActive())
			this.activate();
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

	public ReminderInfo[] getReminders() {
		int count = this.prefs.getInt(REM_COUNT_KEY, 0);
		ReminderInfo[] infoArray = new ReminderInfo[count];
		for(int i = 0; i < count; i++) {
			infoArray[i] = new ReminderInfo();
			infoArray[i].hours = this.prefs.getInt(REM_BASE_KEY + i + REM_SUB_HOURS_KEY, 0);
			infoArray[i].minutes = this.prefs.getInt(REM_BASE_KEY + i + REM_SUB_MIN_KEY, 0);
			infoArray[i].intense = this.prefs.getBoolean(REM_BASE_KEY + i + REM_SUB_INTENSE_KEY, false);
		}
//		ReminderInfo[] infoArray = new ReminderInfo[2];
//		infoArray[0] = new ReminderInfo();
//		infoArray[0].hours = 11;
//		infoArray[0].minutes = 11;
//		infoArray[0].intense = false;
//		infoArray[1] = new ReminderInfo();
//		infoArray[1].hours = 22;
//		infoArray[1].minutes = 22;
//		infoArray[1].intense = true;
		return infoArray;
	}

	public void setActive(boolean activate) {
		if(activate == this.isActive())
			return;

		this.prefs
			.edit()
			.putBoolean(ACTIVE_KEY, activate)
			.apply();

		if(activate) {
			this.activate();
		} else {
			NotificationManager manager = (NotificationManager) this.context.getSystemService(Context.NOTIFICATION_SERVICE);
			manager.cancelAll();
		}
	}

	public void addReminder(int hours, int minutes, boolean intense) {
		int count = this.prefs.getInt(REM_COUNT_KEY, 0);
		this.prefs
			.edit()
			.putInt(REM_COUNT_KEY, count + 1)
			.putInt(REM_BASE_KEY + count + REM_SUB_HOURS_KEY, hours)
			.putInt(REM_BASE_KEY + count + REM_SUB_MIN_KEY, minutes)
			.putBoolean(REM_BASE_KEY + count + REM_SUB_INTENSE_KEY, intense)
			.apply();

		//TODO reload reminders
	}

	public void removeReminder(int hours, int minutes) {
		int count = this.prefs.getInt(REM_COUNT_KEY, 0);
		for(int i = 0; i < count; i++) {
			int iHours = this.prefs.getInt(REM_BASE_KEY + i + REM_SUB_HOURS_KEY, 0);
			int iMinutes = this.prefs.getInt(REM_BASE_KEY + i + REM_SUB_MIN_KEY, 0);
			if(hours == iHours && minutes == iMinutes) {

			}
		}

		this.prefs
			.edit()
			.putInt(REM_COUNT_KEY, count + 1)
			.putInt(REM_BASE_KEY + count + REM_SUB_HOURS_KEY, hours)
			.putInt(REM_BASE_KEY + count + REM_SUB_MIN_KEY, minutes)
			.putBoolean(REM_BASE_KEY + count + REM_SUB_INTENSE_KEY, intense)
			.apply();

		//TODO reload reminders
	}

	public void setAlwaysVisible(boolean always) {
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

	public void setGifTag(String gifTag) {
		this.prefs
			.edit()
			.putString(GIFTAG_KEY, gifTag)
			.apply();
	}

	private void activate() {
		this.setAlwaysVisible(this.isAlwaysVisible());
	}
}
