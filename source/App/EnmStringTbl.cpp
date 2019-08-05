#include  "EnmStringTbl.hpp"

static const MCHAR*  gStringTable[][LANGUE_MAX] = 
{
		/// Common 
		{ "",											"" 		}, 
		{ "<GO>=下一步",					"<GO>=Next" 		}, 
		{ "<STOP>=退出",					"<STOP>=Exit" 	}, 
		{ "按<STOP>终止测试",					"Press <STOP> to abort measurement" 	}, 
		{ "<P>=H.Copy",						"<P>=H.Copy" 		}, 

		{ "日期:",			"Date:"  			},
		{ "时间:",			"Time:"  			},
		{ "速度:",			"Speed:" 			},

		{ "是",					"Y"  			},
		{ "否",					"N" 			},		

		{ "中文",					"Chinese"  			},
		{ "英文",					"English" 			},		

		{ "正在自检...", "Checking..." },
		{ "初始化错误，请重新启动酶标仪", "Init Error, Please Reset the Device" },
	  
		{ "最后的临界值",					"Last CUT-OFF computer" 			},		

		{ "定量",					"Qualitative"  			},
		{ "定性",					"Qualititive" 			},		

		{ "慢",					"Slow"  			},
		{ "中",					"Middle" 			},		
		{ "快",					"Fast"  			},

		{ "测量方式:",	"Measure Mode:" 	}, 
		{ "空白方式:",  "Blanking Mode:" 	},  
		{ "空白位置:",  "Blank Position:" 	},
		{ "计算方式:",  "Calculate Mode:" 	}, 
		{ "进板方式:",  "Plate movement:" 	}, 
		{ "振板方式:",  "Plate shake:" 		}, 	

		{ "数量:",  	"N:" 	  },
		{ "平均值:",  "Mean:" }, 
		{ "CV:",  		"CV:" 	}, 
		{ "SD:",  		"SD:" 	}, 	
	
		/// Main Page
		{ "编程模式",  "Programmed Mode" 	}, 
		{ "快速模式",  "Fast Mode" 	},  
		{ "方法编程",  "Programming" 	},
		{ "程序列表",  "Programmed Test List" 	}, 
		{ "历史数据",  "Saved Data List" 	}, 
		{ "系统设置",  "Reader Setup" 		}, 
		{ "计算机控制",  "Computer Control" 		}, 
		
		/// Programm Mode page
		{ "选择程序",  "Select Program" 	}, 
		{ "测试#",  "Test#" 	}, 
		{ "名称:",  "Name:" 	},  
		{ "类型:",  "Type:" 	},
		{ "临界值:",  "CUT OFF:" 	}, 
		{ "所选波长:",  "Selected Filter:" 	}, 
		{ "主:",  "M: " 		}, 
		{ "辅:",  "R: " 	}, 
		{ "副:",  "S: " 		}, 
		{ "新临界值:",  "New CUT OFF:" 	}, 
		{ "样本数[0-88]:",  "Total Sample[0-88]:" 		}, 
		{ "...正在检测...",  "...Measuring..." 		}, 
		
		{ "测试样品板",  "Sample Plate" 	}, 

		{ "测试数据处理",  "Measure Data processing" 	}, 
		{ "数据计算",  "Data Compute" 	}, 
		{ "编辑临界值",  "Edit CUT OFF" 		}, 
		{ "删除临界值",  "Delete CUT OFF" 	}, 
		{ "存储临界值",  "Save  CUT OFF" 		}, 
		
		{ "测试数据计算：",  "Measure Data Computing:" 	}, 
		{ "选择临界值：",  "Choice CUT OFF:" 		}, 
		{ "NC 对照：",  "Control Nc:" 	}, 
		{ "PC 对照：",  "Control Pc:" 		}, 

		{ "删除临界值",  "Delete CUT-OFF" 	}, 
		{ "删除临界值?",  "Delete CUT-OFF in memory?" 		}, 

		{ "保存临界值",  "Save CUT-OFF" 	}, 
		{ "保存临界值?",  "Saving CUT-OFF Measure?" 		}, 
		
    /// Fast Mode Pages
		{ "快速模式",  "Fast Mode" 	}, 
		{ "整板测试",  "Whole Plate" 	}, 
		{ "整条检测",  "Comumn Monitor" 		}, 
		{ "多波长测试",  "Policromatic" 	}, 
		{ "动态法",  "Kinetic" 		}, 

		{ "整板测试设置", "Whole Plate Setup" },
		{ "滤光片",  "Filter" 	}, 
		{ "波长",  "Lambda=" 		}, 
		{ "板条数 [1-12]:",  "Columns[1-12]:" 	}, 
		{ "震荡时间[0-25]:",  "Mixing Time[0-25]:" 		}, 
		{ "震荡模式:",  "Mixing Mode:" 		}, 
	
		{ "正在测试",  "Measuring" 		}, 
		{ "测试波长",  "Measure WL:  " 		}, 
		{ "辅助波长",  "Reference WL:" 		}, 
		{ "正在测试",  "Measuring on" 		}, 
		{ "个板条",  "Columns" 		}, 
		//{ "",  "" 		}, 

		{ "测试报告",  "Measure Report" 		}, 
		{ "板条数:",  "Column Number:" 		}, 
		{ "反应板数据统计:",  "Plate General Data:" 		}, 
    
		{ "板条数:",  "Column Number:" 		}, 
		{ "间隔时间[2-20]:",  "Period[2-20s]:" 		}, 

		{ "板条检测",  	"Columns Monitoring" 		}, 
		{ "时间(秒):",  "Time (Sec):" 		}, 
		{ "板条",  			"Column" 		}, 
		{ "吸光度:",  	"Absorbance:" 		}, 

		/// Kinetic 
		{ "动力学检测",  	"Kinetic of reaction" 		}, 
		{ "间隔时间[20-1000]:",  "Period[20-1000s]:" 		}, 

		/**
	 *  Programming function
	 */	
		{ "程序",  		"Programming" 		}, 
		{ "创建新程序",  		"Create New Test" 		}, 
		{ "编辑程序",  			"Edit Test" 		}, 
		{ "删除程序",  			"Delete Test" 		}, 

		{ "删除程序",  		"Delete Test" 		}, 
		{ "程序编号:",  			"Identification No.:" 		}, 
		{ "按0删除所有程序",  			"0 delete all tests" 		}, 
		
		{ "创建测试",  				"Program New Test" 		}, 
		{ "编辑测试:",  			"Edit Test" 		}, 
		{ "测试名称:",  			"Test Name:" 		}, 
		{ "主虑光片:  ",  			"Main Filter:" 		}, 
		{ "辅助虑光片:",  			"Reference Filter:" 		}, 
		{ "副虑光片:  ",  			"Secondary Filter:" 		}, 
		{ "测量方式:  ",  			"Measure Mode:" 		}, 

	  /// Qualititive page
		{ "定量测试",  				"Qualitative Mode" 		}, 
		{ "单位:",  					"Unit:" 		}, 
		{ "空白孔数:",  			"Repetitions Blank:" 		}, 
		{ "标准品数:",  			"Number of Stands:" 		}, 
		{ "标准品重复数:",  		"Repetitions Stdnadard:" 		}, 
		{ "样本重复数:",  			"Repetitions Samples:" 		}, 
		{ "标准品号",  				"Std No." 		}, 
		{ "浓度",  					"CONC" 		}, 
		{ "位置",  					"Pos" 		}, 
		
		/// Mixing Mode page
		{ "震动模式",  				"Mixing Setting" 		}, 
		{ "K-参数:  ",  			"K-factor:  " 		}, 
		{ "温度:",  				  "Temprature:" 		}, 
		{ "温度控制",  				"Temprature Control" 		}, 
		
		/// Curve select page
		{ "选择拟合曲线",  		"Select Curve" 		}, 
		{ "拟合曲线:",  			"Curve:" 		}, 
		{ "X坐标轴:",  				"X-Axis:" 		}, 
		{ "Y坐标轴:",  				"Y-Axis:" 		}, 
		{ "直线法",  				  "Linear" 		}, 
		{ "折线法",  				  "Segment" 		}, 
		
		/// Qualitative page
		{ "定性测试",  				"Qualititive Mode" 	}, 
		{ "NC 重复数:",  			"Repetitions NC:" 		}, 
		{ "CO 重复数:",  			"Repetitions CO:" 		}, 
		{ "LP 重复数:",  			"Repetitions LP:" 		}, 
		{ "PC 重复数:",  			"Repetitions PC:" 		}, 
		{ "HP 重复数:",  			"Repetitions HP:" 		}, 		

		/// Cutoff page
		{ "临界值设定",  			"Qualitaitive Mode" 		}, 
		{ "临界值:",  				"CUT-OFF:" 		}, 
		{ "阳性灰区(%):",  			"P Gray Zone(%):" 		}, 
		{ "阴性灰区(%):",  			"N Gray Zone(%):" 		}, 
		{ "+>-",				"+>-"	}, 
		{ "->+",				"->+"	}, 
		{ "S/CutOff",			"S/CutOff"	}, 

		/// Delete protocol page
		{ "正在删除检测程序",  			"Deleting test program" 		}, 		
		{ "检测程序",  					"Test program" 		}, 		
		{ "删除成功",  					" is deleted" 		}, 		

		
		/**
	 *  Reader Config
	 */	
		{ "临界值设定",  			"Reader Setup" 		}, 
		{ "滤光片",  					"Filters" 		}, 
		{ "时间-语言-设备名", "Time-Language-Name" 	}, 
		{ "串口",  						"Serial port" 		}, 
		{ "打印机",  					"Printer" 				}, 
		{ "维护",  						"Maintenance" 		}, 
		{ "反应板检测",  			"Check Plate" 		}, 

		/// Cfg Date-time-name page
		{ "日期设定",				"Set Date&Name"  			},
		{ "当前日期:",				"Current Date:"  			},
		{ "当前时间:",				"Current Time:"  			},
		{ "语言:",						"Lanuage:" 			},
		{ "名称:",						"Name:" 			},
		
	/// Cfg Filter Page
		{ "滤光片设置",  "Filter Setting" 	}, 
		{ "滤光片#1: 波长(nm) = ",  "Filter#1: lambdu(nm) = " 	}, 
		{ "滤光片#2: 波长(nm) = ",  "Filter#2: lambdu(nm) = " 	}, 
		{ "滤光片#3: 波长(nm) = ",  "Filter#3: lambdu(nm) = " 	}, 
		{ "滤光片#4: 波长(nm) = ",  "Filter#4: lambdu(nm) = " 	}, 
		{ "滤光片#5: 波长(nm) = ",  "Filter#5: lambdu(nm) = " 	}, 
		{ "滤光片#6: 波长(nm) = ",  "Filter#6: lambdu(nm) = " 	}, 
		{ "滤光片#7: 波长(nm) = ",  "Filter#7: lambdu(nm) = " 	}, 
		{ "滤光片#8: 波长(nm) = ",  "Filter#8: lambdu(nm) = " 	}, 

		/// Cfg Communication Page
		{ "串口设置",  "Serial Port Setting" 	}, 
		{ "波特率: ",  "Baud: " 	}, 

	/// Cfg Printer 
		{ "打印机设置",  "Printer Setup" 	}, 
		{ "打印机: ",  	 "Printer: " 	}, 
		{ "内置",  		"Internal " 	}, 
		{ "外置",  		"External" 	}, 
		{ "自动打印: ",  "Automatic print: " 	}, 
		{ "打印模式: ",  "Printer mode: " 	}, 
		{ "标准(主-辅)",  "Standard(M-R)" 	}, 
		//{ "打印模式: ",  "Printer mode: " 	}, 

	/// Cfg Maintenance
		{ "设备维护",  "Maintenance" 	}, 
		{ "设备开机时间(小时): ",  	 "Power-on instrument(hours): " 	}, 
		{ "设备上电时间(天): ",  	 "Power-on Instrument(days): " 	}, 
		{ "清除时间?",  	 "Clear Time?" 	}, 
		
		/// Service Test
		{ "维护测试",  "Serve Test" 	}, 
		{ "灯光强度[0-8]: ",  	 "Lamps intensity[0-8]: " 	}, 
		{ "滤光片号[1-8]: ",  	 "Filter Number[1-8]: " 	}, 
		{ "测试条[1-12]  :",  	 "Measure Column[1-12]: " 	}, 
		{ "刷新时间[1-255]:",  	 "Averaging[1-255]: " 	}, 

	/// Cfg Plate Check
		{ "反应板检测",  "Plate Presence Check" 	}, 
		{ "反应板存在检测: ",  "Plate Presence Check: " 	}, 
		
		/**
	 *  Computer controlling
	 */	
		{ "计算机控制模式", 		"Computer Control"	}, 
		{ " 测试参数 ", 				" Program Parameters " 	}, 
		{ "震荡时间:",  				"Mixing Time:" 		}, 
		{ "震荡模式:",  				"Mixing Mode:" 		}, 
		{ "秒",  				"Sec" 		}, 
		{ "扫描起始列:",  				"Start Column:" 		}, 
		{ "总列数:",  				"Sum Columns:" 		}, 
		{ "正在检测样品，请稍候...", "Testing, Please Wait..."}, 
		{ "检测完毕",						"Test Finished" 	}, 
		
};

const MCHAR** gGetStringTbl()
{
	return (const MCHAR**)gStringTable;
}

INT32U  gGetStrTblItemCount()
{
	return sizeof(gStringTable) / sizeof(gStringTable[0]);
}
