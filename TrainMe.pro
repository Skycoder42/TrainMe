TEMPLATE = subdirs

SUBDIRS += TrainMe \
    GifLoader
win32: SUBDIRS += WinReminderService

TrainMe.depends += GifLoader
win32: WinReminderService.depends += GifLoader

DISTFILES += \
	LICENSE \
    muscle-flex-outline.svg \
    up-arrow.svg \
