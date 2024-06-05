registers = [
    "I2S/PCM_CTL 0x0000 I2S/PCM Control Register",
    "I2S/PCM_FMT0 0x0004 I2S/PCM Format Register 0",
    "I2S/PCM_FMT1 0x0008 I2S/PCM Format Register 1",
    "I2S/PCM_ISTA 0x000C I2S/PCM Interrupt Status Register",
    "I2S/PCM_RXFIFO 0x0010 I2S/PCM RXFIFO Register",
    "I2S/PCM_FCTL 0x0014 I2S/PCM FIFO Control Register",
    "I2S/PCM_FSTA 0x0018 I2S/PCM FIFO Status Register",
    "I2S/PCM_INT 0x001C I2S/PCM DMA & Interrupt Control Register",
    "I2S/PCM_TXFIFO 0x0020 I2S/PCM TXFIFO Register",
    "I2S/PCM_CLKD 0x0024 I2S/PCM Clock Divide Register",
    "I2S/PCM_TXCNT 0x0028 I2S/PCM TX Sample Counter Register",
    "I2S/PCM_RXCNT 0x002C I2S/PCM RX Sample Counter Register",
    "I2S/PCM_CHCFG 0x0030 I2S/PCM Channel Configuration Register",
    "I2S/PCM_TX0CHSEL 0x0034 I2S/PCM TX0 Channel Select Register",
    "I2S/PCM_TX1CHSEL 0x0038 I2S/PCM TX1 Channel Select Register",
    "I2S/PCM_TX2CHSEL 0x003C I2S/PCM TX2 Channel Select Register",
    "I2S/PCM_TX3CHSEL 0x0040 I2S/PCM TX3 Channel Select Register",
    "I2S/PCM_TX0CHMAP0 0x0044 I2S/PCM TX0 Channel Mapregisterg Register0",
    "I2S/PCM_TX0CHMAP1 0x0048 I2S/PCM TX0 Channel Mapregisterg Register1",
    "I2S/PCM_TX1CHMAP0 0x004C I2S/PCM TX1 Channel Mapregisterg Register0",
    "I2S/PCM_TX1CHMAP1 0x0050 I2S/PCM TX1 Channel Mapregisterg Register1",
    "I2S/PCM_TX2CHMAP0 0x0054 I2S/PCM TX2 Channel Mapregisterg Register0",
    "I2S/PCM_TX2CHMAP1 0x0058 I2S/PCM TX2 Channel Mapregisterg Register1",
    "I2S/PCM_TX3CHMAP0 0x005C I2S/PCM TX3 Channel Mapregisterg Register0",
    "I2S/PCM_TX3CHMAP1 0x0060 I2S/PCM TX3 Channel Mapregisterg Register1",
    "I2S/PCM_RXCHSEL 0x0064 I2S/PCM RX Channel Select Register",
    "I2S/PCM_RXCHMAP0 0x0068 I2S/PCM RX Channel Mapregisterg Register0",
    "I2S/PCM_RXCHMAP1 0x006C I2S/PCM RX Channel Mapregisterg Register1",
    "I2S/PCM_RXCHMAP2 0x0070 I2S/PCM RX Channel Mapregisterg Register2",
    "I2S/PCM_RXCHMAP3 0x0074 I2S/PCM RX Channel Mapregisterg Register3",
    "UNUSED 0x0078 ",
    "UNUSED 0x007c ",
    "MCLKCFG 0x0080 ASRC MCLK Configuration Register",
    "FsoutCFG 0x0084 ASRC Out Sample Rate Configuration Register",
    "FsinEXTCFG 0x0088 ASRC Input Sample Pulse Extend Configuration Register",
    "ASRCCFG 0x008C ASRC Enable Register",
    "ASRCMANCFG 0x0090 ASRC Manual Ratio Configuration Register",
    "ASRCRATIOSTAT 0x0094 ASRC Status Register",
    "ASRCFIFOSTAT 0x0098 ASRC FIFO Level Status Register",
    "ASRCMBISTCFG 0x009C ASRC MBIST Test Configuration Register",
    "ASRCMBISTSTAT 0x00A0 ASRC MBIST Test Status Register",
]

print("typedef struct I2S {")
registerdetails = [] 
for idx, register in enumerate(registers):
    registerparts = register.split(' 0x')
    name = registerparts[0]
    name = name.replace('/PCM', '')
    offset = int(registerparts[1][:4], 16)
    details = registerparts[1][5:]
    registerdetails.append({ "name": name, "offset": offset, "details": details})


for idx, register in enumerate(registerdetails):
    var_type = 'unsigned int'
    if idx < len(registerdetails) - 1:
        var_size = registerdetails[idx + 1]['offset'] - register['offset']
        if var_size == 4:
            var_type = 'unsigned int'
        elif var_size == 8:
            var_type = 'unsigned long' 
        else:
            breakpoint()
            raise Exception("found register that isn't 4 or 8 bytes")
    print(f"    {var_type} {register['name']}; // offset: {hex(register['offset'])}, {register['details']}")
        
print("} I2S;")
