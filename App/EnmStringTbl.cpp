#include  "EnmStringTbl.hpp"

static const MCHAR*  gStringTable[][LANGUE_MAX] = 
{
		/// Common 
		{ "",											"" 		}, 
		{ "<GO>=��һ��",					"<GO>=Next" 		}, 
		{ "<STOP>=�˳�",					"<STOP>=Exit" 	}, 
		{ "��<STOP>��ֹ����",					"Press <STOP> to abort measurement" 	}, 
		{ "<P>=H.Copy",						"<P>=H.Copy" 		}, 

		{ "����:",			"Date:"  			},
		{ "ʱ��:",			"Time:"  			},
		{ "�ٶ�:",			"Speed:" 			},

		{ "��",					"Y"  			},
		{ "��",					"N" 			},		

		{ "����",					"Chinese"  			},
		{ "Ӣ��",					"English" 			},		

		{ "�����Լ�...", "Checking..." },
		{ "��ʼ����������������ø����", "Init Error, Please Reset the Device" },
	  
		{ "�����ٽ�ֵ",					"Last CUT-OFF computer" 			},		

		{ "����",					"Qualitative"  			},
		{ "����",					"Qualititive" 			},		

		{ "��",					"Slow"  			},
		{ "��",					"Middle" 			},		
		{ "��",					"Fast"  			},

		{ "������ʽ:",	"Measure Mode:" 	}, 
		{ "�հ׷�ʽ:",  "Blanking Mode:" 	},  
		{ "�հ�λ��:",  "Blank Position:" 	},
		{ "���㷽ʽ:",  "Calculate Mode:" 	}, 
		{ "���巽ʽ:",  "Plate movement:" 	}, 
		{ "��巽ʽ:",  "Plate shake:" 		}, 	

		{ "����:",  	"N:" 	  },
		{ "ƽ��ֵ:",  "Mean:" }, 
		{ "CV:",  		"CV:" 	}, 
		{ "SD:",  		"SD:" 	}, 	
	
		/// Main Page
		{ "���ģʽ",  "Programmed Mode" 	}, 
		{ "����ģʽ",  "Fast Mode" 	},  
		{ "�������",  "Programming" 	},
		{ "�����б�",  "Programmed Test List" 	}, 
		{ "��ʷ����",  "Saved Data List" 	}, 
		{ "ϵͳ����",  "Reader Setup" 		}, 
		{ "���������",  "Computer Control" 		}, 
		
		/// Programm Mode page
		{ "ѡ�����",  "Select Program" 	}, 
		{ "����#",  "Test#" 	}, 
		{ "����:",  "Name:" 	},  
		{ "����:",  "Type:" 	},
		{ "�ٽ�ֵ:",  "CUT OFF:" 	}, 
		{ "��ѡ����:",  "Selected Filter:" 	}, 
		{ "��:",  "M: " 		}, 
		{ "��:",  "R: " 	}, 
		{ "��:",  "S: " 		}, 
		{ "���ٽ�ֵ:",  "New CUT OFF:" 	}, 
		{ "������[0-88]:",  "Total Sample[0-88]:" 		}, 
		{ "...���ڼ��...",  "...Measuring..." 		}, 
		
		{ "������Ʒ��",  "Sample Plate" 	}, 

		{ "�������ݴ���",  "Measure Data processing" 	}, 
		{ "���ݼ���",  "Data Compute" 	}, 
		{ "�༭�ٽ�ֵ",  "Edit CUT OFF" 		}, 
		{ "ɾ���ٽ�ֵ",  "Delete CUT OFF" 	}, 
		{ "�洢�ٽ�ֵ",  "Save  CUT OFF" 		}, 
		
		{ "�������ݼ��㣺",  "Measure Data Computing:" 	}, 
		{ "ѡ���ٽ�ֵ��",  "Choice CUT OFF:" 		}, 
		{ "NC ���գ�",  "Control Nc:" 	}, 
		{ "PC ���գ�",  "Control Pc:" 		}, 

		{ "ɾ���ٽ�ֵ",  "Delete CUT-OFF" 	}, 
		{ "ɾ���ٽ�ֵ?",  "Delete CUT-OFF in memory?" 		}, 

		{ "�����ٽ�ֵ",  "Save CUT-OFF" 	}, 
		{ "�����ٽ�ֵ?",  "Saving CUT-OFF Measure?" 		}, 
		
    /// Fast Mode Pages
		{ "����ģʽ",  "Fast Mode" 	}, 
		{ "�������",  "Whole Plate" 	}, 
		{ "�������",  "Comumn Monitor" 		}, 
		{ "�ನ������",  "Policromatic" 	}, 
		{ "��̬��",  "Kinetic" 		}, 

		{ "�����������", "Whole Plate Setup" },
		{ "�˹�Ƭ",  "Filter" 	}, 
		{ "����",  "Lambda=" 		}, 
		{ "������ [1-12]:",  "Columns[1-12]:" 	}, 
		{ "��ʱ��[0-25]:",  "Mixing Time[0-25]:" 		}, 
		{ "��ģʽ:",  "Mixing Mode:" 		}, 
	
		{ "���ڲ���",  "Measuring" 		}, 
		{ "���Բ���",  "Measure WL:  " 		}, 
		{ "��������",  "Reference WL:" 		}, 
		{ "���ڲ���",  "Measuring on" 		}, 
		{ "������",  "Columns" 		}, 
		//{ "",  "" 		}, 

		{ "���Ա���",  "Measure Report" 		}, 
		{ "������:",  "Column Number:" 		}, 
		{ "��Ӧ������ͳ��:",  "Plate General Data:" 		}, 
    
		{ "������:",  "Column Number:" 		}, 
		{ "���ʱ��[2-20]:",  "Period[2-20s]:" 		}, 

		{ "�������",  	"Columns Monitoring" 		}, 
		{ "ʱ��(��):",  "Time (Sec):" 		}, 
		{ "����",  			"Column" 		}, 
		{ "�����:",  	"Absorbance:" 		}, 

		/// Kinetic 
		{ "����ѧ���",  	"Kinetic of reaction" 		}, 
		{ "���ʱ��[20-1000]:",  "Period[20-1000s]:" 		}, 

		/**
	 *  Programming function
	 */	
		{ "����",  		"Programming" 		}, 
		{ "�����³���",  		"Create New Test" 		}, 
		{ "�༭����",  			"Edit Test" 		}, 
		{ "ɾ������",  			"Delete Test" 		}, 

		{ "ɾ������",  		"Delete Test" 		}, 
		{ "������:",  			"Identification No.:" 		}, 
		{ "��0ɾ�����г���",  			"0 delete all tests" 		}, 
		
		{ "��������",  				"Program New Test" 		}, 
		{ "�༭����:",  			"Edit Test" 		}, 
		{ "��������:",  			"Test Name:" 		}, 
		{ "���ǹ�Ƭ:  ",  			"Main Filter:" 		}, 
		{ "�����ǹ�Ƭ:",  			"Reference Filter:" 		}, 
		{ "���ǹ�Ƭ:  ",  			"Secondary Filter:" 		}, 
		{ "������ʽ:  ",  			"Measure Mode:" 		}, 

	  /// Qualititive page
		{ "��������",  				"Qualitative Mode" 		}, 
		{ "��λ:",  					"Unit:" 		}, 
		{ "�հ׿���:",  			"Repetitions Blank:" 		}, 
		{ "��׼Ʒ��:",  			"Number of Stands:" 		}, 
		{ "��׼Ʒ�ظ���:",  		"Repetitions Stdnadard:" 		}, 
		{ "�����ظ���:",  			"Repetitions Samples:" 		}, 
		{ "��׼Ʒ��",  				"Std No." 		}, 
		{ "Ũ��",  					"CONC" 		}, 
		{ "λ��",  					"Pos" 		}, 
		
		/// Mixing Mode page
		{ "��ģʽ",  				"Mixing Setting" 		}, 
		{ "K-����:  ",  			"K-factor:  " 		}, 
		{ "�¶�:",  				  "Temprature:" 		}, 
		{ "�¶ȿ���",  				"Temprature Control" 		}, 
		
		/// Curve select page
		{ "ѡ���������",  		"Select Curve" 		}, 
		{ "�������:",  			"Curve:" 		}, 
		{ "X������:",  				"X-Axis:" 		}, 
		{ "Y������:",  				"Y-Axis:" 		}, 
		{ "ֱ�߷�",  				  "Linear" 		}, 
		{ "���߷�",  				  "Segment" 		}, 
		
		/// Qualitative page
		{ "���Բ���",  				"Qualititive Mode" 	}, 
		{ "NC �ظ���:",  			"Repetitions NC:" 		}, 
		{ "CO �ظ���:",  			"Repetitions CO:" 		}, 
		{ "LP �ظ���:",  			"Repetitions LP:" 		}, 
		{ "PC �ظ���:",  			"Repetitions PC:" 		}, 
		{ "HP �ظ���:",  			"Repetitions HP:" 		}, 		

		/// Cutoff page
		{ "�ٽ�ֵ�趨",  			"Qualitaitive Mode" 		}, 
		{ "�ٽ�ֵ:",  				"CUT-OFF:" 		}, 
		{ "���Ի���(%):",  			"P Gray Zone(%):" 		}, 
		{ "���Ի���(%):",  			"N Gray Zone(%):" 		}, 
		{ "+>-",				"+>-"	}, 
		{ "->+",				"->+"	}, 
		{ "S/CutOff",			"S/CutOff"	}, 

		/// Delete protocol page
		{ "����ɾ��������",  			"Deleting test program" 		}, 		
		{ "������",  					"Test program" 		}, 		
		{ "ɾ���ɹ�",  					" is deleted" 		}, 		

		
		/**
	 *  Reader Config
	 */	
		{ "�ٽ�ֵ�趨",  			"Reader Setup" 		}, 
		{ "�˹�Ƭ",  					"Filters" 		}, 
		{ "ʱ��-����-�豸��", "Time-Language-Name" 	}, 
		{ "����",  						"Serial port" 		}, 
		{ "��ӡ��",  					"Printer" 				}, 
		{ "ά��",  						"Maintenance" 		}, 
		{ "��Ӧ����",  			"Check Plate" 		}, 

		/// Cfg Date-time-name page
		{ "�����趨",				"Set Date&Name"  			},
		{ "��ǰ����:",				"Current Date:"  			},
		{ "��ǰʱ��:",				"Current Time:"  			},
		{ "����:",						"Lanuage:" 			},
		{ "����:",						"Name:" 			},
		
	/// Cfg Filter Page
		{ "�˹�Ƭ����",  "Filter Setting" 	}, 
		{ "�˹�Ƭ#1: ����(nm) = ",  "Filter#1: lambdu(nm) = " 	}, 
		{ "�˹�Ƭ#2: ����(nm) = ",  "Filter#2: lambdu(nm) = " 	}, 
		{ "�˹�Ƭ#3: ����(nm) = ",  "Filter#3: lambdu(nm) = " 	}, 
		{ "�˹�Ƭ#4: ����(nm) = ",  "Filter#4: lambdu(nm) = " 	}, 
		{ "�˹�Ƭ#5: ����(nm) = ",  "Filter#5: lambdu(nm) = " 	}, 
		{ "�˹�Ƭ#6: ����(nm) = ",  "Filter#6: lambdu(nm) = " 	}, 
		{ "�˹�Ƭ#7: ����(nm) = ",  "Filter#7: lambdu(nm) = " 	}, 
		{ "�˹�Ƭ#8: ����(nm) = ",  "Filter#8: lambdu(nm) = " 	}, 

		/// Cfg Communication Page
		{ "��������",  "Serial Port Setting" 	}, 
		{ "������: ",  "Baud: " 	}, 

	/// Cfg Printer 
		{ "��ӡ������",  "Printer Setup" 	}, 
		{ "��ӡ��: ",  	 "Printer: " 	}, 
		{ "����",  		"Internal " 	}, 
		{ "����",  		"External" 	}, 
		{ "�Զ���ӡ: ",  "Automatic print: " 	}, 
		{ "��ӡģʽ: ",  "Printer mode: " 	}, 
		{ "��׼(��-��)",  "Standard(M-R)" 	}, 
		//{ "��ӡģʽ: ",  "Printer mode: " 	}, 

	/// Cfg Maintenance
		{ "�豸ά��",  "Maintenance" 	}, 
		{ "�豸����ʱ��(Сʱ): ",  	 "Power-on instrument(hours): " 	}, 
		{ "�豸�ϵ�ʱ��(��): ",  	 "Power-on Instrument(days): " 	}, 
		{ "���ʱ��?",  	 "Clear Time?" 	}, 
		
		/// Service Test
		{ "ά������",  "Serve Test" 	}, 
		{ "�ƹ�ǿ��[0-8]: ",  	 "Lamps intensity[0-8]: " 	}, 
		{ "�˹�Ƭ��[1-8]: ",  	 "Filter Number[1-8]: " 	}, 
		{ "������[1-12]  :",  	 "Measure Column[1-12]: " 	}, 
		{ "ˢ��ʱ��[1-255]:",  	 "Averaging[1-255]: " 	}, 

	/// Cfg Plate Check
		{ "��Ӧ����",  "Plate Presence Check" 	}, 
		{ "��Ӧ����ڼ��: ",  "Plate Presence Check: " 	}, 
		
		/**
	 *  Computer controlling
	 */	
		{ "���������ģʽ", 		"Computer Control"	}, 
		{ " ���Բ��� ", 				" Program Parameters " 	}, 
		{ "��ʱ��:",  				"Mixing Time:" 		}, 
		{ "��ģʽ:",  				"Mixing Mode:" 		}, 
		{ "��",  				"Sec" 		}, 
		{ "ɨ����ʼ��:",  				"Start Column:" 		}, 
		{ "������:",  				"Sum Columns:" 		}, 
		{ "���ڼ����Ʒ�����Ժ�...", "Testing, Please Wait..."}, 
		{ "������",						"Test Finished" 	}, 
		
};

const MCHAR** gGetStringTbl()
{
	return (const MCHAR**)gStringTable;
}

INT32U  gGetStrTblItemCount()
{
	return sizeof(gStringTable) / sizeof(gStringTable[0]);
}
