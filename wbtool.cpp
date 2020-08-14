#include <string.h>
#include <unistd.h>

#include "wbmr_menu.hpp"
#include "wbtool.hpp"

static bool isWindowMode = false;

int main(int argc, char **argv)
{
    int baud = 9600;
    char parity = 'N';
    int stop = 2;
    unsigned int address;
    char opt;

    while ((opt = getopt(argc, argv, "b:p:s:")) != -1) {
        switch (opt) {
        case 'b':
            baud = atoi(optarg);
	    if (baud != 1200 && baud != 2400 && baud != 4800 && baud != 9600 &&
	        baud != 19200 && baud != 38400 && baud != 57600 && baud != 115200) {
	        fprintf(stderr, "Invalid baud rate value %s\n", optarg);
	        exit(EXIT_FAILURE);
	    }
            break;
        case 'p':
	    parity = toupper(optarg[0]);
            if (parity != 'N' && parity != 'E' && parity != 'O') {
	        fprintf(stderr, "Invalid parity value %s\n", optarg);
	        exit(EXIT_FAILURE);
	    }
            break;
        case 's':
	    stop = atoi(optarg);
	    if (stop != 1 && stop != 2) {
	        fprintf(stderr, "Invalid stop bits count %s\n", optarg);
	        exit(EXIT_FAILURE);
	    }
	    break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-b baudrate] [-p parity] [-s stopbits] <device> <address>\n"
	                    "  baudrate: 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200; default = 9600\n"
	                    "  parity  : n, e, o, none, even, odd (case-insensitive); default = none\n"
			    "  stopbits: 1, 2; default = 2\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (optind + 1 >= argc) {
        fprintf(stderr, "Device name and/or address is not supplied\n");
        exit(EXIT_FAILURE);
    }

    address = atoi(argv[optind + 1]);
    if (address > 255) {
	fprintf(stderr, "Invalid address %s\n", argv[optind + 1]);
    }

    Device dev(argv[optind], baud, parity, stop, address);

    isWindowMode = true;
    newtInit();
    newtCls();

    const char *model = dev.getModel();

    if (!model)
	model = "";

    if (!strncmp(model, "WBMR", 4)) {
	WBMR_Device wbmr(dev);
	WBMR_MainMenu(wbmr).Execute();
    } else {
        MainMenu(dev).Execute();
    }

    newtFinished();
}

// WB devices return EMBXILVAL for unsupported registers, so we
// produce more appropriate message
const char *error_or_unsupported(void)
{
    if (errno == EMBXILVAL)
        return "This function is not supported by this device";
    else
	return modbus_strerror(errno);
}

void critical(const char *fmt, ...)
{
    va_list ap;

    if (isWindowMode)
	newtFinished();

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(EXIT_FAILURE);
}
