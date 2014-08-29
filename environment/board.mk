
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
HAS_LCD			= 1		# still unused 				

# Output debug messages on UART
DEBUGFN 		= usart_write

BOARD_OPTS = 	-DHAS_LCD=$(HAS_LCD) \
#				-DDEBUGFN=$(DEBUGFN)

# FUSE CONFIGURATION 
FUSE_LOW = 0xff
FUSE_HIGH = 0xdf
FUSE_EXTENDED = 0x07
