package com.Skycoder42.TrainMe;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.Intent;
import android.preference.PreferenceManager;
import android.graphics.BitmapFactory;
import android.app.NotificationManager;
import android.app.Notification;
import android.app.PendingIntent;
import android.app.AlarmManager;
import android.support.v4.app.NotificationCompat;
import android.text.TextUtils;
import java.text.MessageFormat;
import java.text.NumberFormat;
import java.util.Set;
import java.util.HashSet;
import java.util.Arrays;
import java.util.Date;
import java.util.Calendar;

import android.util.Log;

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

	public static final int OPEN_INTENT_ID = 0;

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
		String[] keys = this.loadReminders().toArray(new String[0]);
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

	public void setActive(boolean activate) {
		if(activate == this.isActive())
			return;

		this.prefs
			.edit()
			.putBoolean(ACTIVE_KEY, activate)
			.apply();

		if(activate)
			this.activate();
		else {
			NotificationManager manager = (NotificationManager) this.context.getSystemService(Context.NOTIFICATION_SERVICE);
			manager.cancelAll();

			ReminderInfo[] infos = this.getReminders();
			for(ReminderInfo info : infos)
				this.cancelNotification(info);
		}
	}

	public void addReminder(ReminderInfo info) {
		Set<String> keys = this.loadReminders();
		String newKey = MessageFormat.format(REM_ENTRY_FORMAT, info.hours, info.minutes);
		keys.add(newKey);

		this.prefs
			.edit()
			.putString(REM_KEYS_KEY, TextUtils.join(KEY_LIST_SPLITTER, keys))
			.putBoolean(REM_KEY_BASE + newKey, info.intense)
			.apply();

		this.scheduleNotification(info);
	}

	public void removeReminder(ReminderInfo info) {
		Set<String> keys = this.loadReminders();
		String newKey = MessageFormat.format(REM_ENTRY_FORMAT, info.hours, info.minutes);
		keys.remove(newKey);

		this.prefs
			.edit()
			.putString(REM_KEYS_KEY, TextUtils.join(KEY_LIST_SPLITTER, keys))
			.remove(REM_KEY_BASE + newKey)
			.apply();

		this.cancelNotification(info);
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
			PendingIntent pending = PendingIntent.getActivity(this.context,
				OPEN_INTENT_ID,
				intent,
				PendingIntent.FLAG_UPDATE_CURRENT);

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

	private int calcId(ReminderInfo info) {
		return (info.hours * 100) + info.minutes;
	}

	private Set<String> loadReminders() {
		String data = this.prefs.getString(REM_KEYS_KEY, "");
		if(data.isEmpty())
			return new HashSet<String>();
		return new HashSet<String>(Arrays.asList(data.split(KEY_LIST_SPLITTER)));
	}

	private void scheduleNotification(ReminderInfo info) {
		Intent intent = new Intent(this.context, AlarmReceiver.class);
		intent.putExtra(AlarmReceiver.ID_EXTRA, this.calcId(info));
		intent.putExtra(AlarmReceiver.INTENSE_EXTRA, info.intense);
		PendingIntent pending = PendingIntent.getBroadcast(this.context,
			this.calcId(info),
			intent,
			PendingIntent.FLAG_UPDATE_CURRENT);

		Calendar cal = Calendar.getInstance();
		if(info.hours < cal.get(Calendar.HOUR_OF_DAY) ||
		   (info.hours == cal.get(Calendar.HOUR_OF_DAY) && info.minutes <= cal.get(Calendar.MINUTE))) {
		   cal.add(Calendar.DAY_OF_MONTH, 1);
		}
		cal.set(Calendar.HOUR_OF_DAY, info.hours);
		cal.set(Calendar.MINUTE, info.minutes);
		cal.set(Calendar.SECOND, 0);

		Log.d("TrainMe.Controller", "Scheduled alarm for " + cal.getTime());
		AlarmManager alarm = (AlarmManager) this.context.getSystemService(Context.ALARM_SERVICE);
		alarm.setInexactRepeating(info.intense ? AlarmManager.RTC_WAKEUP : AlarmManager.RTC,
			cal.getTimeInMillis(),
			AlarmManager.INTERVAL_DAY,
			pending);
	}

	private void cancelNotification(ReminderInfo info) {
		Intent intent = new Intent(this.context, AlarmReceiver.class);
		PendingIntent pending = PendingIntent.getBroadcast(this.context,
			this.calcId(info),
			intent,
			PendingIntent.FLAG_UPDATE_CURRENT);

		AlarmManager alarm = (AlarmManager) this.context.getSystemService(Context.ALARM_SERVICE);
		alarm.cancel(pending);
	}

	private void activate() {
		this.setAlwaysVisible(this.isAlwaysVisible());

		ReminderInfo[] infos = this.getReminders();
		for(ReminderInfo info : infos)
			this.scheduleNotification(info);
	}
}
