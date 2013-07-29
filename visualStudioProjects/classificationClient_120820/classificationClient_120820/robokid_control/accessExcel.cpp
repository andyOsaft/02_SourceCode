#include "stdafx.h"

#include <vcclr.h>

using namespace Runtime::InteropServices;

//Implementations for class accessExcel

//constructor
accessExcel::accessExcel(void)
{
	//Initialize Excel application and display it
	_variant_t  varOption( (long) DISP_E_PARAMNOTFOUND, VT_ERROR );

	//A try block is used to trap any errors in communication
	//try
	//{
		//Initialise COM interface
		//CoInitialize(NULL);

		//Start the Excel Application
		if (FAILED (excel_ptr.CreateInstance(L"Excel.Application")))
		{
			//robokid_control::Form1.commLogBox->AppendText("[INFO]: Failed to initialize Excel!" + Environment::NewLine);
		}
		
		//excel_ptr.CreateInstance(L"Excel.Application");
		//Sleep(200);
		
		//Make the Excel Application visible, so that we can see it!
		excel_ptr->Visible = true;

		//XL->GetWorkbooks()
		
		//open existing Excel workbook
		workbook_ptr = excel_ptr->Workbooks->Open(L"E:\\Buero_120801\\Studium\\Diplomarbeit\\10_software\\classificationClient_120820\\classificationClient_120820\\Debug\\test.xlsx", //Filename, 
													varOption,  // UpdateLinks, 
													varOption,  // ReadOnly, 
													varOption,  // Format, 
													varOption,  // Password, 
													varOption,  // WriteResPassword, 
													varOption,  // IgnoreReadOnlyRecommended, 
													varOption,  // Origin, 
													varOption,  // Delimiter, 
													varOption,  // Editable, 
													varOption,  // Notify, 
													varOption,  // Converter, 
													varOption,  // AddToMru, 
													varOption,  // Local, 
													varOption  // CorruptLoad, 
													);
	//}

	//If a communication error is thrown, catch it and complain
	//catch(_com_error)
	//{
		//cout << "COM error " << endl;
	//}

	
	//XL->Workbooks->Add(Excel::xlWorksheet);
	
		
	//Get a pointer to the first worksheet
	//worksheet_ptr = workbook_ptr->Sheets->Item[ 1 ];
	
	//Get a pointer to all cells in the active worksheet
	//range_ptr = worksheet_ptr->Cells;

	//range_ptr->Item[2][1] = 0.86;

	//Get a pointer to the active workbook
	//workbook_ptr = excel_ptr->GetActiveWorkbook();

	//worksheet_ptr = workbook_ptr->Sheets->Item[ 2 ];
	
	//Get a pointer to all cells in the active worksheet
	//range_ptr = worksheet_ptr->Cells;

	//range_ptr->Item[2][1] = 0.86;

	//workbook_ptr = excel_ptr->GetActiveWorkbook();
	//worksheet_ptr = workbook_ptr->Sheets->Item[ 2 ];
	//range_ptr = worksheet_ptr->Cells;
	//range_ptr->Item[3][1] = L"Servus";


	//writeExcelCell(1,1,1,L"blabla");
}

//destructor
accessExcel::~accessExcel(void)
{
	//close Excel instance
	try
	{
		//workbook_ptr = excel_ptr->GetActiveWorkbook();
		//workbook_ptr->Close( VARIANT_FALSE );
		excel_ptr->Quit();
	}
	catch (_com_error)
	{
		
	}

}


void accessExcel::writeExcelCell(int worksheetNr, int lineValue, int columnValue, String ^ dataString)
{
	//convert system::string into _bstr_t
	
	// Obtain a pointer to the System::String in order to
    // first lock memory into place, so that the
    // Garbage Collector (GC) cannot move that object
    // while we call native functions.
	pin_ptr<const wchar_t> wch = PtrToStringChars(dataString);

	// Convert a wide character system string to a
    // wide character _bstr_t string.
	//_bstr_t	bstrText(wch);
	_variant_t vItem(wch);
		
	workbook_ptr = excel_ptr->GetActiveWorkbook();

	worksheet_ptr = workbook_ptr->Sheets->Item[ worksheetNr ];
	
	//Get a pointer to all cells in the active worksheet
	range_ptr = worksheet_ptr->Cells;

	range_ptr->Item[lineValue][columnValue] = vItem;	

	
}

void accessExcel::refreshWorkbook()
{
	//Excel::
	//workbook_ptr = excel_ptr->GetActiveWorkbook();
	//workbook_ptr->RefreshAll();
}


//return values:
//value: free storage for data available in this line
//0: worksheet is full
int accessExcel::searchForFreeLine(int worksheetNumber)
{
	int nextFreeLine;

	workbook_ptr = excel_ptr->GetActiveWorkbook();

	worksheet_ptr = workbook_ptr->Sheets->Item[ worksheetNumber ];

	range_ptr = worksheet_ptr->Cells;

	_variant_t  vItem = range_ptr->Item[ 1 ][ 2 ];

	_bstr_t     bstrText( vItem );

	const size_t newSize = 100;
	wchar_t wcString[newSize];
	wcscpy_s(wcString, (wchar_t *)bstrText);
	
	String ^systemstring = gcnew String(wcString);


	try
	{
		nextFreeLine = Convert::ToInt32(systemstring);
	}
	catch (FormatException ^)
	{
		nextFreeLine = 0;
	}
	catch (OverflowException ^)
	{
		nextFreeLine = 0;
	}

	return nextFreeLine;
}