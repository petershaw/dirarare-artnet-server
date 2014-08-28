
# Settings for the basic options of the target board

# BOARD REVISION
REVISION = 2

# Name of target controller
MCU = atmega328p

# Frequency of the controller
F_CPU = 20000000UL

# Maximal Memory size of eeprom in byte
EE_MAX_SIZE = 0

# Define the board configuration
HAS_LCD			= 0
HAS_UART 		= 0

# Boards-LED
STATUS_LED		= PD7

# Output debug messages on UART
DEBUGFN 		= usart_write

BOARD_OPTS = 	-DHAS_LCD=$(HAS_LCD) \
				-DHAS_UART=$(HAS_UART) \
				-DSTATUS_LED=$(STATUS_LED) 
#				-DDEBUGFN=$(DEBUGFN)
