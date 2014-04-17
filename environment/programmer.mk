
# Settings for progrmmmer 

# id to use with programmer
# default: PROGRAMMER_MCU=$(MCU)
PROGRAMMER_MCU = m328p

# Programmer to use with avrdude
PROGRAMMER=usbasp

# port to use a bootloader
# This should be the port for your MCU.
BOOTLOADER_PORT = /dev/tty.SLAB_USBtoUART

# baud to use a bootloader
BOOTLOADER_BAUD = 9600
