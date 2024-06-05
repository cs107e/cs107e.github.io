pins = [
	"PCCR23	0x0024	PWM23 Clock Configuration Register",
	"PCCR45	0x0028	PWM45 Clock Configuration Register",
	"PCCR67	0x002C	PWM67 Clock Configuration Register",
	"PCGR	0x0040	PWM Clock Gating Register",
	"PDZCR01	0x0060	PWM01 Dead Zone Control Register",
	"PDZCR23	0x0064	PWM23 Dead Zone Control Register",
	"PDZCR45	0x0068	PWM45 Dead Zone Control Register",
	"PDZCR67	0x006C	PWM67 Dead Zone Control Register",
	"PER	0x0080	PWM Enable Register",
	"PGR0	0x0090	PWM Group0 Register",
	"PGR1	0x0094	PWM Group1 Register",
	"PGR2	0x0098	PWM Group2 Register",
	"PGR3	0x009C	PWM Group3 Register",
	"CER	0x00C0	Capture Enable Register",
	"PCR0	0x0100	PWM0 Control Register",
	"PCR1	0x0120	PWM1 Control Register",
	"PPR0	0x0104	PWM0 Period Register",
	"PPR1	0x0124	PWM1 Period Register",
	"PCNTR0	0x0108	PWM0 Count Register",
	"PCNTR1	0x0128	PWM1 Count Register",
	"PPCNTR0	0x010c	PWM0 Pulse Count Register",
	"PPCNTR1	0x012c	PWM1 Pulse Count Register"
]

for pin in pins:
    pinparts = pin.split('\t')
    try:
        print(f"// {pinparts[2]}")
    except IndexError:
        breakpoint()
    print(f"#define {pinparts[0]} (PWM_BASE + {pinparts[1]})")
