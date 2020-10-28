// Firmware bug: device reports IllegalDataValue on nonexistent registers
// instead of IllegalDataAddress
static inline int is_unsupported(void)
{
    return errno == EMBXILVAL || errno == EMBXILADD;
}

const char *error_or_unsupported(void);
void critical(const char *text, ...);
