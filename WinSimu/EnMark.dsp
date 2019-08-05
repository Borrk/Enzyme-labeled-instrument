# Microsoft Developer Studio Project File - Name="EnMark" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=EnMark - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EnMark.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EnMark.mak" CFG="EnMark - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EnMark - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "EnMark - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EnMark - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "EnMark - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W2 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "EnMark - Win32 Release"
# Name "EnMark - Win32 Debug"
# Begin Group "Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Base\CDtKid.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\CDtKid.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmApi.c
# End Source File
# Begin Source File

SOURCE=..\Base\EnmApi.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmBeeper.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmCfgCheckPage.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmCfgCheckPage.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmCfgCommuPage.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmCfgCommuPage.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmCfgFilterPage.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmCfgFilterPage.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmCfgPlateCheckPage.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmCfgPlateCheckPage.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmCfgPrinterPage.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmCfgPrinterPage.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmCfgServiceTestPage.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmCfgServiceTestPage.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmCfgTimePage.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmCfgTimePage.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmConcDataUtil.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmConcDataUtil.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmCutoffLib.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmCutoffLib.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmDataProc.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmDataProc.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmDef.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmFilterMotor.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmFilterMotor.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmFormater.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmFormater.hpp
# End Source File
# Begin Source File

SOURCE=..\Driver\Src\EnmFs.c
# End Source File
# Begin Source File

SOURCE=..\Driver\Inc\EnmFs.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmMathParser.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmMathParser.hpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmModeLib.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmModeLib.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmMotor.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmMotor.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmObserver.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmObserver.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmPlateMotor.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmPlateMotor.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmPrinter.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmPrinter.hpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmProtocol.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmSubject.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmSubject.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmSysCfgBasePage.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmSysCfgBasePage.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmSysCfgLib.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmSysCfgLib.h
# End Source File
# Begin Source File

SOURCE=..\Wfs\EnmWfoRoot.cpp
# End Source File
# Begin Source File

SOURCE=..\Wfs\EnmWfoRoot.h
# End Source File
# Begin Source File

SOURCE=..\Driver\Src\FlashMng.c
# End Source File
# Begin Source File

SOURCE=..\Driver\Inc\FlashMng.h
# End Source File
# Begin Source File

SOURCE=..\Base\Matrix.c
# End Source File
# Begin Source File

SOURCE=..\Base\Matrix.h
# End Source File
# End Group
# Begin Group "App"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\App\EnmApp.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmApp.hpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmBase.hpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmBasePage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmBasePage.hpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmConcListPage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmConcListPage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmControlConcPage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmControlConcPage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmCutoffCompute.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmCutoffCompute.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmDeleteCutoffPage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmDeleteCutoffPage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmDensityCurvePage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmDensityCurvePage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmDensityListPage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmDensityListPage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmFastModePage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmFastModePage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmFmCmMeasPage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmFmCmMeasPage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmFmCmPage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmFmCmPage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmFmKnConcListPage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmFmKnConcListPage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmFmModePage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmFmModePage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmFmWpMeasurePage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmFmWpMeasurePage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmFmWpModePage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmFmWpModePage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmMainPage.hpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmModeBasePage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmModeBasePage.hpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmModePage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmModePage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgDtPage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgDtPage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgEtCurvePage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgEtCurvePage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgEtCutoffPage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgEtCutoffPage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgEtMixingPage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgEtMixingPage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgEtModePage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgEtModePage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgEtQualitativePage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgEtQualitativePage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgEtQualityPage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgEtQualityPage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgEtSampleList.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgEtSampleList.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgListPage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmPgListPage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmPlatePage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmPlatePage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmProcPanelPage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmProgramScanPage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmProgramScanPage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmSampleConcListPage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmSampleConcListPage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmSaveCutoffPage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmSaveCutoffPage.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmScanContext.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmScanContext.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmStringTbl.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmStringTbl.hpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmTestKit.h
# End Source File
# Begin Source File

SOURCE=..\Wfs\EnmWfo.cpp
# End Source File
# Begin Source File

SOURCE=..\Wfs\EnmWfo.h
# End Source File
# Begin Source File

SOURCE=..\Wfs\EnmWfoSelecter.cpp
# End Source File
# Begin Source File

SOURCE=..\Wfs\EnmWfoSelecter.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmWfSelectPage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmWfSelectPage.h
# End Source File
# Begin Source File

SOURCE=..\Wfs\EnmWfsFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\Wfs\EnmWfsFactory.h
# End Source File
# Begin Source File

SOURCE=..\Wfs\IWorkStep.cpp
# End Source File
# Begin Source File

SOURCE=..\Wfs\IWorkStep.h
# End Source File
# End Group
# Begin Group "MUI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\MUI\MIG_Base.cpp
# End Source File
# Begin Source File

SOURCE=..\MUI\MIG_Base.hpp
# End Source File
# Begin Source File

SOURCE=..\MUI\MIG_Control.cpp
# End Source File
# Begin Source File

SOURCE=..\MUI\MIG_Control.hpp
# End Source File
# Begin Source File

SOURCE=..\MUI\Mig_Drawer44b0.cpp
# End Source File
# Begin Source File

SOURCE=..\MUI\Mig_Drawer44b0.h
# End Source File
# Begin Source File

SOURCE=..\MUI\MIG_EditBox.cpp
# End Source File
# Begin Source File

SOURCE=..\MUI\MIG_EditBox.Hpp
# End Source File
# Begin Source File

SOURCE=..\MUI\Mig_IDrawer.cpp
# End Source File
# Begin Source File

SOURCE=..\MUI\Mig_IDrawer.h
# End Source File
# Begin Source File

SOURCE=..\MUI\MIG_Obj.cpp
# End Source File
# Begin Source File

SOURCE=..\MUI\MIG_Obj.hpp
# End Source File
# Begin Source File

SOURCE=..\MUI\MIG_OS_Base.hpp
# End Source File
# Begin Source File

SOURCE=..\MUI\MIG_Page.cpp
# End Source File
# Begin Source File

SOURCE=..\MUI\MIG_Page.hpp
# End Source File
# Begin Source File

SOURCE=..\MUI\MigEventMng.cpp
# End Source File
# Begin Source File

SOURCE=..\MUI\MigEventMng.h
# End Source File
# Begin Source File

SOURCE=..\MUI\MStrMngr.h
# End Source File
# Begin Source File

SOURCE=..\MUI\MStrMngr.hpp
# End Source File
# End Group
# Begin Group "Engine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Base\EnmConsoleProc.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmConsoleProc.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmKeyEngine.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmKeyEngine.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmMainProc.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmMainProc.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmProcEngine.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmProcEngine.h
# End Source File
# Begin Source File

SOURCE=..\Base\EnmScanEngine.cpp
# End Source File
# Begin Source File

SOURCE=..\Base\EnmScanEngine.h
# End Source File
# Begin Source File

SOURCE=..\Wfs\EnmWfStepHub.cpp
# End Source File
# Begin Source File

SOURCE=..\Wfs\EnmWfStepHub.h
# End Source File
# End Group
# Begin Group "Simulater"

# PROP Default_Filter ""
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\EnMark.ico
# End Source File
# Begin Source File

SOURCE=.\res\EnMark.rc2
# End Source File
# Begin Source File

SOURCE=.\res\EnMarkDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\EnMark.cpp
# End Source File
# Begin Source File

SOURCE=.\EnMark.h
# End Source File
# Begin Source File

SOURCE=.\EnMark.rc
# End Source File
# Begin Source File

SOURCE=.\EnMarkDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\EnMarkDoc.h
# End Source File
# Begin Source File

SOURCE=.\EnMarkView.cpp
# End Source File
# Begin Source File

SOURCE=.\EnMarkView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SimKm29.C
# End Source File
# Begin Source File

SOURCE=.\SimuScanEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuScanEngine.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\App\EnmConsole.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmConsole.h
# End Source File
# Begin Source File

SOURCE=..\App\EnmConsolePage.cpp
# End Source File
# Begin Source File

SOURCE=..\App\EnmConsolePage.h
# End Source File
# Begin Source File

SOURCE=.\EnmTester.cpp
# End Source File
# Begin Source File

SOURCE=.\EnmTester.h
# End Source File
# End Target
# End Project
