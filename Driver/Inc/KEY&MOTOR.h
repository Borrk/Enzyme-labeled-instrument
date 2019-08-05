#ifndef __KEY_MOTOR_H__
#define __KEY_MOTOR_H__

#define KEY_0_N 		'0'
#define KEY_1_A 		'1'
#define KEY_2_B 		'2'
#define KEY_3_C 		'3'
#define KEY_4_D 		'4'
#define KEY_5_E 		'5'
#define KEY_6_F 		'6'
#define KEY_7_G 		'7'
#define KEY_8_H 		'8'
#define KEY_9_P 		'9'
#define KEY_Sign		'-'
#define KEY_DOT_L		'.'
#define KEY_OK			1
#define KEY_ESC			2
#define KEY_Up 			3
#define KEY_Down		4
#define KEY_Right		5
#define KEY_Left		6
#define KEY_GO			7
#define	KEY_TEST    8
#define KEY_Prn			9
#define KEY_PRG			10
#define	KEY_Par			11
#define KEY_Data		12
#define KEY_FUN1		13
#define KEY_FUN2		14
#define KEY_FUN3		15
#define KEY_FUN4		16
   
#define	KEY_SPACE		' '         
#define	KEY_0		    '0'         
#define	KEY_1		    '1'        
#define	KEY_2		    '2'        
#define	KEY_3		    '3'        
#define	KEY_4		    '4'        
#define	KEY_5		    '5'        
#define	KEY_6		    '6'        
#define	KEY_7		    '7'        
#define	KEY_8		    '8'        
#define	KEY_9		    '9'        
#define	KEY_A		    'A'        
#define	KEY_B		    'B'        
#define	KEY_C		    'C'        
#define	KEY_D		    'D'        
#define	KEY_E		    'E'        
#define	KEY_F		    'F'        
#define	KEY_G		    'G'        
#define	KEY_H		    'H'        
#define	KEY_I		    'I'        
#define	KEY_J		    'J'        
#define	KEY_K		    'K'        
#define	KEY_L		    'L'        
#define	KEY_M		    'M'        
#define	KEY_N		    'N'        
#define	KEY_O		    'O'        
#define	KEY_P		    'P'        
#define	KEY_Q			'Q'        
#define	KEY_R			'R'        
#define	KEY_S		 	'S'        
#define	KEY_T		 	'T'        
#define	KEY_U		    'U'        
#define	KEY_V		    'V'        
#define	KEY_W		    'W'        
#define	KEY_X		    'X'        
#define	KEY_Y		    'Y'        
#define	KEY_Z			'Z'        
#define	KEY_a		    'a'        
#define	KEY_b		    'b'        
#define	KEY_c		    'c'        
#define	KEY_d		    'd'        
#define	KEY_e		    'e'        
#define	KEY_f		    'f'        
#define	KEY_g		    'g'        
#define	KEY_h		    'h'        
#define	KEY_i		    'i'        
#define	KEY_j		    'j'        
#define	KEY_k		    'k'        
#define	KEY_l		    'l'        
#define	KEY_m		    'm'        
#define	KEY_n		    'n'        
#define	KEY_o		    'o'        
#define	KEY_p		    'p'        
#define	KEY_q				'q'        
#define	KEY_r				'r'        
#define	KEY_s		 		's'        
#define	KEY_t		 		't'        
#define	KEY_u		    'u'        
#define	KEY_v		    'v'        
#define	KEY_w		    'w'        
#define	KEY_x		    'x'        
#define	KEY_y		    'y'        
#define	KEY_z				'z'        
#define	KEY_ADD				'+'   
#define	KEY_SUB				'-'      
#define	KEY_MUL				'*' 
#define	KEY_DIV				'/'
#define	KEY_EQUAL			'=' 
#define	KEY_L_BRACKET		'('	
#define	KEY_R_BRACKET		')'

#define HW_VERSION200  200
#define HW_VER		HW_VERSION200
	
#if HW_VER == HW_VERSION200 

/// Key's Scan Code
#define SCANCODE_KEY_0_N 		0x840
#define SCANCODE_KEY_1_A 		0x220
#define SCANCODE_KEY_2_B 		0x240
#define SCANCODE_KEY_3_C 		0x280
#define SCANCODE_KEY_4_D 		0x120
#define SCANCODE_KEY_5_E 		0x140
#define SCANCODE_KEY_6_F 		0x180
#define SCANCODE_KEY_7_G 		0x420
#define SCANCODE_KEY_8_H 		0x440
#define SCANCODE_KEY_9_P 		0x480
#define SCANCODE_KEY_Up 		0x208
#define SCANCODE_KEY_Down		0x410
#define SCANCODE_KEY_Right		0x108
#define SCANCODE_KEY_Left		0x110
#define SCANCODE_KEY_OK			0x808
#define SCANCODE_KEY_ESC		0x810
#define	SCANCODE_KEY_TEST   	0x210
#define SCANCODE_KEY_Prn		0x408
#define SCANCODE_KEY_Sign		0x880
#define SCANCODE_KEY_DOT_L		0x820

#define SCANCODE_KEY_PRG		0x441
#define	SCANCODE_KEY_Par		0x241
#define SCANCODE_KEY_Data		0x221
#define SCANCODE_KEY_FUN1		0x481
#define SCANCODE_KEY_FUN2		0x881
#define SCANCODE_KEY_FUN3		0x281
#define SCANCODE_KEY_FUN4		0x181

#else
/// Key's Scan Code
#define SCANCODE_KEY_0_N 		0x110
#define SCANCODE_KEY_1_A 		0x210
#define SCANCODE_KEY_2_B 		0x208
#define SCANCODE_KEY_3_C 		0x204
#define SCANCODE_KEY_4_D 		0x410
#define SCANCODE_KEY_5_E 		0x408
#define SCANCODE_KEY_6_F 		0x404
#define SCANCODE_KEY_7_G 		0x810
#define SCANCODE_KEY_8_H 		0x808
#define SCANCODE_KEY_9_P 		0x804
#define SCANCODE_KEY_Up 		0x102
#define SCANCODE_KEY_Down		0x802
#define SCANCODE_KEY_Right		0x402
#define SCANCODE_KEY_Left		0x202
#define SCANCODE_KEY_OK			0x201
#define SCANCODE_KEY_ESC		0x101
#define	SCANCODE_KEY_TEST   	0x840
#define SCANCODE_KEY_PRG		0x440
#define	SCANCODE_KEY_Par		0x240
#define SCANCODE_KEY_Data		0x220
#define SCANCODE_KEY_Prn		0x120
#define SCANCODE_KEY_FUN1		0x880
#define SCANCODE_KEY_FUN2		0x480
#define SCANCODE_KEY_FUN3		0x280
#define SCANCODE_KEY_FUN4		0x180
#define SCANCODE_KEY_Sign		0x108
#define SCANCODE_KEY_DOT_L		0x104

#endif

#define GetKeyValue(key)	((key) & 0xFF)
#define GetKeyCount(key)	(((key) >> 8)& 0xFF )


#ifdef __cplusplus
extern "C" 
{
#endif

unsigned short Scan_KEY(void);
void Plate_Motor_On( char bOn );
void Light_Motor_On( char bOn );
void Plate_Motor_CW_ONE_STEP(void);
void Plate_Motor_CCW_ONE_STEP(void);
void Light_Motor_CW_ONE_STEP(void);
void Light_Motor_CCW_ONE_STEP(void);
unsigned char Is_Plate_Motor_Reset(void);
unsigned char Is_Light_Motor_Reset(void);

#ifdef __cplusplus
}
#endif

#endif	// __KEY_MOTOR_H__