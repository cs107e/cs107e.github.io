void ps2_write(unsigned char command);
enum { PS2_CMD_RESET = 0xFF, PS2_CODE_ACK = 0xFA, PS2_CMD_FLAGS = 0xED , PS2_CMD_ENABLE_DATA_REPORTING = 0xF4 };
