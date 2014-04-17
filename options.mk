
# Set what option should compile into the OKPrompt

# Maximal memory available for persistent variables
OBP_Variables_MAX_SIZE = $(EE_MAX_SIZE)

# set builddate
BUILD_DATE = $(shell date)
BUILD_DATE_SHORT = $(shell date "+%Y-%m-%d")

DASH_VARS = \
	-DOBP_Variables_MAX_SIZE=$(OBP_Variables_MAX_SIZE) \
	-DBUILD_DATE="\"$(BUILD_DATE)\"" -DBUILD_DATE_SHORT="\"$(BUILD_DATE_SHORT)\""
	