:: Run this script to prepare the winreminderservice to be run in debug mode
:: working dir: %{buildDir}
@echo off

mkdir DEBUG_SERVICE_LOCATION
xcopy ".\WinReminderService\debug\WinReminderService.*" ".\DEBUG_SERVICE_LOCATION\*" /y
xcopy ".\WinReminderService\debug\WinReminderService1.*" ".\DEBUG_SERVICE_LOCATION\*" /y
xcopy ".\GifLoader\debug\GifLoader.*" ".\DEBUG_SERVICE_LOCATION\*" /y
del ".\DEBUG_SERVICE_LOCATION\*.obj"
