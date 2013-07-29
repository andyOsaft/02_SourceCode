#pragma once

#include "stdafx.h"

//microsoft office objects
#import "E:\\Buero_120801\\Studium\\Diplomarbeit\\10_software\\classificationClient_120820\\classificationClient_120820\\Debug\\MSO.DLL" \
    rename( "RGB", "MSORGB" ) \
	rename( "DocumentProperties", "MSODocumentProperties")

using namespace Office;

//microsoft vba objects
#import "E:\\Buero_120801\\Studium\\Diplomarbeit\\10_software\\classificationClient_120820\\classificationClient_120820\\Debug\\VBE6EXT.OLB"

//using namespace VBIDE;

//excel application object
#import "C:\\Program Files (x86)\\Microsoft Office\\Office12\\EXCEL.EXE" \
    rename( "DialogBox", "ExcelDialogBox" ) \
    rename( "RGB", "ExcelRGB" ) \
    rename( "CopyFile", "ExcelCopyFile" ) \
    rename( "ReplaceText", "ExcelReplaceText" ) \
	rename( "DocumentProperties", "ExcelDocumentProperties") \
	exclude( "IFont", "IPicture") no_dual_interfaces
/*	The directive no_dual_interfaces changes the manner in
	which overloaded functions are called. It must be included so that the _ApplicationPtr,
	which has a dual interface, is called in the correct manner.
*/

using namespace System;


//class model for accessExcel
public class accessExcel
{

//member functions
public:
	accessExcel(void);
	~accessExcel(void);
	void accessExcel::writeExcelCell(int worksheetNr, int lineValue, int columnValue, String^ dataString);
	void accessExcel::refreshWorkbook();
	int accessExcel::searchForFreeLine(int numberOfSelectedCharacteristic);


//member variables
private:
	
	Excel::_ApplicationPtr excel_ptr;    //application
	Excel::_WorkbookPtr workbook_ptr;    //workbook
	Excel::_WorksheetPtr worksheet_ptr;  //worksheet
	Excel::RangePtr range_ptr;	         //range
	
};

