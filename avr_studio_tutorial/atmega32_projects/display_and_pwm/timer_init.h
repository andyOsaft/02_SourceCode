//Funktionsprototypen
void timer_init(uint16_t compare);


//Register TCCR1A -> Timer/Counter 1 Control Register A
#define WGM10	0  //Bit 0 - 1 -> Waveform Generation Mode (Datenblatt S.114)
#define WGM11	1
#define FOC1B	2  //Force Output Compare for channel B
#define FOC1A	3  //Force Output Compare for channel A
#define COM1B0	4  //Bit 4 - 7 -> Compare Output Mode (Datenblatt S.113)
#define	COM1B1	5
#define COM1A0	6
#define COM1A1	7


//Register TCCR1B – Timer/Counter 1 Control Register B
#define CS10	0	//Bit 0 - 2 -> Clock Select
#define	CS11	1
#define CS12	2
#define WGM12	3	//Bit 3 - 4 -> Waveform Generation Mode
#define	WGM13	4
//#define   --    5 nicht belegt
#define ICES1	6	//Input Capture Edge Select
#define	ICNC1	7	//Input Capture Noise Canceler





#define BIT0 0
#define BIT1 1
#define BIT2 2
#define BIT3 3
#define BIT4 4
#define BIT5 5
#define BIT6 6
#define BIT7 7


