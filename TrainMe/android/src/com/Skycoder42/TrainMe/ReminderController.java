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
import android.text.TextUtils;
import java.text.MessageFormat;
import java.text.NumberFormat;
import java.util.Set;
import java.util.HashSet;
import java.util.Arrays;
import java.util.Date;

public class ReminderController {
	private static final int STATUS_NOT_KEY = 42;

	private static final String ACTIVE_KEY = "Reminder.Active";
	private static final String VISIBLE_KEY = "Reminder.Visible";
	private static final String GIFTAG_KEY = "Reminder.Giftag";
	private static final String REM_KEY_BASE = "Reminder.Reminders.";
	private static final String REM_KEYS_KEY = REM_KEY_BASE + "Keys";
	private static final String REM_ENTRY_FORMAT = "Rem_{0}_{1}";

	private static final String KEY_LIST_SPLITTER = ";";
	private static final String KEY_ENTRY_SPLITTER = "_";

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
		String data = this.prefs.getString(REM_KEYS_KEY, "");
		if(data.isEmpty())
			return new ReminderInfo[0];
		else {
			String[] keys = data.split(KEY_LIST_SPLITTER);
			ReminderInfo[] infoArray = new ReminderInfo[keys.length];
			for(int i = 0; i < keys.length; i++) {
				String[] entry = keys[i].split(KEY_ENTRY_SPLITTER);
				infoArray[i] = new ReminderInfo();
				infoArray[i].hours = Integer.parseInt(entry[1]);
				infoArray[i].minutes = Integer.parseInt(entry[2]);
				infoArray[i].intense = this.prefs.getBoolean(REM_KEY_BASE + keys[i], false);
			}
			return infoArray;
		}
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
		Set<String> keys = new HashSet<String>(Arrays.asList(this.prefs.getString(REM_KEYS_KEY, "").split(KEY_LIST_SPLITTER)));
		String newKey = MessageFormat.format(REM_ENTRY_FORMAT, hours, minutes);
		keys.add(newKey);

		this.prefs
			.edit()
			.putString(REM_KEYS_KEY, TextUtils.join(KEY_LIST_SPLITTER, keys))
			.putBoolean(REM_KEY_BASE + newKey, intense)
			.apply();

		//TODO reload reminders
	}

	public void removeReminder(int hours, int minutes) {
		Set<String> keys = new HashSet<String>(Arrays.asList(this.prefs.getString(REM_KEYS_KEY, "").split(KEY_LIST_SPLITTER)));
		String newKey = MessageFormat.format(REM_ENTRY_FORMAT, hours, minutes);
		keys.remove(newKey);

		this.prefs
			.edit()
			.putString(REM_KEYS_KEY, TextUtils.join(KEY_LIST_SPLITTER, keys))
			.remove(REM_KEY_BASE + newKey)
			.apply();

		//TODO reload reminders
	}

	public void skipDate(Date date) {
		((MainActivity)this.context).showToast(date.toString(), false);
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
