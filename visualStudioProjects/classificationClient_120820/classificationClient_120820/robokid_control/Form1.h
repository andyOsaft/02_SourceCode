#include "showStatisticsForm.h"
#include "toeControl.h"

#include <windows.h>

#include "accessExcel.h"

#include <time.h>


#define POSLRIGHT	 0
#define POSLLEFT	 1
#define TURNLRIGHT	 2
#define TURNLLEFT	 3
#define BRAKELIGHT	 4
#define REVERSELIGHT 5
#define FOGLIGHT	 6

#define START_CLASSIFICATION	0
#define WAIT_FOR_RESULTS		1
#define PROCESS_RESULTS			2
#define SETUP_NEXT_RUN			3
#define IDLE					4

#define INFOMODE_STANDARD		1
#define INFOMODE_DETAIL			2
#define INFOMODE_SAMPLE			3

#define CURRENTSAMPLES_SHEET		1
#define CLASSIFICATIONERRORS_SHEET	2
#define SLOPEANALYSIS_SHEET  		3
#define CURRENTINTEGRAL_SHEET		4
#define STATISTICALANALYSIS_SHEET	5


#define INCOMING_DATA_BUFFER_SIZE 200

#pragma once

namespace robokid_control {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary>
	/// Zusammenfassung für Form1
	/// </summary>

//reference to accessExcel class (so that it can be used inside Form1 class)
//ref class accessExcel;	

public ref class Form1 : public System::Windows::Forms::Form
{
	public:
		//Konstruktor für Form1
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Konstruktorcode hier hinzufügen.
			//

			// Set time zone from TZ environment variable. If TZ is not set,
			// the operating system is queried to obtain the default value 
			// for the variable. 
			//
			_tzset();

			selectPortBox->SelectedIndex=2;

			dutyCycleLabel->Visible = false;
			dutyCycleTextBox->Visible = false;
			//pwmGroupBox->Visible = false;
			dutyCycleTextBox->Text = "0";

			//set form start position
			this->Location = Point(100, 100);

			//create instance of power supply control form
			toeControlDialog = gcnew toeControl;
			toeControlDialog->Hide();
			isToeDialogActive = false;
			

			lampChannelWorkList = gcnew array<bool,1>(7);
			incomingDataBuffer = gcnew array<String^,1>(INCOMING_DATA_BUFFER_SIZE);
			incomingDataBufferIndex = 0;

			lampChannelLookUp = gcnew array<String^,1>(7);
			lampChannelLookUp[0] = "posLRight";
			lampChannelLookUp[1] = "posLLeft"; 
			lampChannelLookUp[2] = "turnLRight"; 
			lampChannelLookUp[3] = "turnLLeft"; 
			lampChannelLookUp[4] = "brakeLight";
			lampChannelLookUp[5] = "reverseLight";
			lampChannelLookUp[6] = "fogLight";


			lampNumberLookUp = gcnew array<String^,1>(7);
			lampNoteLookUp = gcnew array<String^,1>(7);
			lampTypeLookUp = gcnew array<String^,1>(7);

			lampTypeLookUp[0] = "Non-Flashing";
			lampTypeLookUp[1] = "Non-Flashing";
			lampTypeLookUp[2] = "Non-Flashing";
			lampTypeLookUp[3] = "Non-Flashing";
			lampTypeLookUp[4] = "Non-Flashing";
			lampTypeLookUp[5] = "Non-Flashing";
			lampTypeLookUp[6] = "Non-Flashing";

			//set default values for single results of the new matlab classification algorithm
			slopeAnalysisResult = "not set";
			currentIntegralResult = "not set";
			statisticalAnalysisResult = "not set";
			ecuOperatingVoltage = "not set";

			//Assign delegate to updateComLogBoxMethod
			updateComLogBoxDelegate = gcnew updateComLogBox(this, &Form1::updateComLogBoxMethod);
			
			
			usedCharacteristicComboBox->SelectedIndex = 0;
			numberIterationTextBox->Text = "1";
			currentIteration = 0;
			informationModeComboBox->SelectedIndex = 0;

			stateMachine_state = IDLE;

			serialPort1->BaudRate = 115200;
			//set timeout threshold for serial port (in ms) 
			serialPort1->ReadTimeout = 2000;

			waitTimeTextBox->Text = "0";
			waitingTimeBetweenIterations = 0;


			currentLineCurrentSamples = 0;
			currentColumnIL = 1;

			currentLineClassificationErrors = 0;

			currentLineSlopeResults = 0;
			currentColumnC1 = 1;

			currentLineCurrentIntegral = 0;

			currentLineStatisticalAnalysis = 0;


			//create pointer to accessExcel class (and call constructor)
			myExcelInstance = new accessExcel();
			//myExcelInstance->writeExcelCellInt(1,1,1,1);

			//check worksheets for the first free lines to store measurement data
			currentLineCurrentSamples = myExcelInstance->searchForFreeLine(CURRENTSAMPLES_SHEET);
			currentLineClassificationErrors = myExcelInstance->searchForFreeLine(CLASSIFICATIONERRORS_SHEET);
			currentLineSlopeResults = myExcelInstance->searchForFreeLine(SLOPEANALYSIS_SHEET);
			currentLineCurrentIntegral = myExcelInstance->searchForFreeLine(CURRENTINTEGRAL_SHEET);
			currentLineStatisticalAnalysis = myExcelInstance->searchForFreeLine(STATISTICALANALYSIS_SHEET);

			//check if the read of the first free lines was successfull -> if not display warning
			if (currentLineCurrentSamples == 0)
			{
				//worksheet is full -> display warning
				commLogBox->AppendText("[WARNING]: tool was not able to determine the next free line in excel worksheet for current samples." + Environment::NewLine);
				commLogBox->AppendText("[WARNING]: please check cell B1!" + Environment::NewLine);
				currentLineCurrentSamples = 12;
			}
			if (currentLineClassificationErrors == 0)
			{
				//worksheet is full -> display warning
				commLogBox->AppendText("[WARNING]: tool was not able to determine the next free line in worksheet for classification errors." + Environment::NewLine);
				commLogBox->AppendText("[WARNING]: please check cell B1!" + Environment::NewLine);
				currentLineClassificationErrors = 12;
			}
			if (currentLineSlopeResults == 0)
			{
				//worksheet is full -> display warning
				commLogBox->AppendText("[WARNING]: tool was not able to determine the next free line in worksheet for slope analysis results." + Environment::NewLine);
				commLogBox->AppendText("[WARNING]: please check cell B1!" + Environment::NewLine);
				currentLineSlopeResults = 12;
			}
			if (currentLineCurrentIntegral == 0)
			{
				//worksheet is full -> display warning
				commLogBox->AppendText("[WARNING]: tool was not able to determine the next free line in worksheet for current integral results." + Environment::NewLine);
				commLogBox->AppendText("[WARNING]: please check cell B1!" + Environment::NewLine);
				currentLineCurrentIntegral = 12;
			}
			if (currentLineStatisticalAnalysis == 0)
			{
				//worksheet is full -> display warning
				commLogBox->AppendText("[WARNING]: tool was not able to determine the next free line in worksheet for statistical analysis results." + Environment::NewLine);
				commLogBox->AppendText("[WARNING]: please check cell B1!" + Environment::NewLine);
				currentLineStatisticalAnalysis = 12;
			}

		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Form1()
		{
			serialPort1->Close();

			if (components)
			{
				delete myExcelInstance;
				delete toeControlDialog;
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  connectSerialButton;
	protected: 

	//class members:

	//***********************************************************************************************************
	// GUI parts:
	//***********************************************************************************************************

	private: System::Windows::Forms::Label^  serialConnectionLabel;
	private: System::Windows::Forms::GroupBox^  speedGroupBox;
	private: System::Windows::Forms::RadioButton^  speed115200Button;

	private: System::Windows::Forms::RadioButton^  speed57600Button;
	private: System::Windows::Forms::ComboBox^  selectPortBox;
	private: System::Windows::Forms::Label^  selectPortLabel;
	public: System::Windows::Forms::TextBox^  commLogBox;
	private: System::Windows::Forms::Label^  commDataLabel;
	private: System::Windows::Forms::MenuStrip^  menuBar;

	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exitMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  displaysSoftwareVersionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  displaysSoftwareVersionToolStripMenuItem1;
	private: System::IO::Ports::SerialPort^  serialPort1;
	private: System::Windows::Forms::ToolStripMenuItem^  statisticsToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  pwmViewToolStripMenuItem;

	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Button^  disconnectSerialButton;




	//statistics form
	private: showStatisticsForm^ showStatisticsDialog;
	//map form
	private: toeControl^ toeControlDialog;
	
	//***********************************************************************************************************
	// data variables:
	//***********************************************************************************************************
	//string for the incoming data of the serial port
	public: 
			String^ incomingData;

	private:
			String^ hpmMessage;

	private:
			String^ selectedCharacteristic;
			int selectedCharacteristicNumber;
			String^ selectedInformationMode;
			int selectedInformationModeNumber;
			int numberOfIterations;
			array<String^,1>^ incomingDataBuffer;
			int incomingDataBufferIndex;
			int stateMachine_state;
			int currentIteration;

	private:

			//array to store all the lamp channels that are to be classified with the current command (the current press on "startClassificationButton"
			//-> if one array index position has its value set to 1 -> then this channel is due for classification
			//-> the currently processed lamp channel is identified by the value of variable "currentWorklistIndex"
			//-> the worklist is processed in function "stateMachineHandler"
			array<bool,1>^ lampChannelWorkList;
			int currentWorklistIndex;


			// array to store the names of the different lamp channels
			array<String^,1>^ lampChannelLookUp;

			//array to store the value of the lamp number text box -> user input
			array<String^,1>^ lampNumberLookUp;

			//array to store the current flashing type of the light device of the channel
			//-> is updated when the flashing check-boxes are modified
			//-> array index represents the lamp channel, e.g. POSLRIGHT = 0 and FOGLIGHT = 6
			array<String^,1>^ lampTypeLookUp; //true = flashing

			//array to store the value of the lamp note text box -> user input
			array<String^,1>^ lampNoteLookUp;


	private:
			//variables to store the results of the single classifications -> to log it in case a classification error occurs
			String^ slopeAnalysisResult;
			String^ currentIntegralResult;
			String^ statisticalAnalysisResult;
			String^ ecuOperatingVoltage;


	private:
			int currentLampNumber;
			String^ currentLampType;

	private:
			int waitingTimeBetweenIterations;

	private:
			int currentLineSlopeResults;
			int currentColumnC1;
			int currentLineSlopeResultsDetail;
			int currentLineSlopeResultsSample;

	private:
			int currentLineCurrentSamples;
			int currentColumnIL;
			int currentLineCurrentSamplesLed;
			int currentLineCurrentSamplesLedNormed;
			int currentLineCurrentSamplesBulb;
			int currentLineCurrentSamplesBulbNormed;
			int currentLineCurrentSamplesEnd;
			int currentLineCurrentSamplesEndNormed;


	private:
			int currentLineClassificationErrors;
			int currentLineCurrentIntegral;
			int currentLineStatisticalAnalysis;

	private:		
			accessExcel* myExcelInstance;	
		
	private:
			//variable to store the visible status of the toeControlDialog
			bool isToeDialogActive;
	
	private: System::Windows::Forms::GroupBox^  classificationSetupGroupBox;

	private: System::Windows::Forms::Button^  clearLogButton;
	private: System::Windows::Forms::Button^  clearResultsButton;
	private: System::Windows::Forms::GroupBox^  classificationSetupInnerGroupBox;


	private: System::Windows::Forms::ToolStripMenuItem^  standardViewToolStripMenuItem;
	private: System::Windows::Forms::Label^  selectTestTypeLabel;


	private: System::Windows::Forms::Label^  classificationSetupLabel;
	private: System::Windows::Forms::TextBox^  numberIterationTextBox;
	private: System::Windows::Forms::ComboBox^  usedCharacteristicComboBox;
	private: System::Windows::Forms::Label^  usedCharacterisicLabel;
	private: System::Windows::Forms::ComboBox^  informationModeComboBox;
	private: System::Windows::Forms::Label^  informationModeLabel;

	private: System::Windows::Forms::Label^  lampChannelLabel;
	private: System::Windows::Forms::Button^  abortClassificationButton;
	private: System::Windows::Forms::Button^  startClassificationButton;
	private: System::Windows::Forms::GroupBox^  lampSelectionGroupBox;
	private: System::Windows::Forms::CheckBox^  fogLightSelectCheckBox;

	private: System::Windows::Forms::CheckBox^  reverseLightSelectCheckBox;

	private: System::Windows::Forms::CheckBox^  brakeLightSelectCheckBox;

	private: System::Windows::Forms::CheckBox^  turnLLeftSelectCheckBox;

	private: System::Windows::Forms::CheckBox^  turnLRightSelectCheckBox;

	private: System::Windows::Forms::CheckBox^  posLRightSelectCheckBox;
	private: System::Windows::Forms::CheckBox^  posLLeftSelectCheckBox;


	private: System::Windows::Forms::CheckBox^  allLampsSelectCheckBox;
	private: System::Windows::Forms::CheckBox^  fogLightFlashingCheckBox;

	private: System::Windows::Forms::CheckBox^  reverseLightFlashingCheckBox;

	private: System::Windows::Forms::CheckBox^  brakeLightFlashingCheckBox;

	private: System::Windows::Forms::CheckBox^  turnLLeftFlashingCheckBox;

	private: System::Windows::Forms::CheckBox^  turnLRightFlashingCheckBox;

	private: System::Windows::Forms::CheckBox^  posLRightFlashingCheckBox;
	private: System::Windows::Forms::CheckBox^  posLLeftFlashingCheckBox;
	private: System::Windows::Forms::Label^  lampNumberLabel;
	private: System::Windows::Forms::TextBox^  fogLightLampNrTextBox;
	private: System::Windows::Forms::TextBox^  reverseLightLampNrTextBox;
	private: System::Windows::Forms::TextBox^  brakeLightLampNrTextBox;
	private: System::Windows::Forms::TextBox^  turnLLeftLampNrTextBox;
	private: System::Windows::Forms::TextBox^  turnLRightLampNrTextBox;
	private: System::Windows::Forms::TextBox^  posLLeftLampNrTextBox;
	private: System::Windows::Forms::TextBox^  posLRightLampNrTextBox;
	private: System::Windows::Forms::Label^  lampTypeLabel;
	private: System::Windows::Forms::Label^  lampNoteLabel;
	private: System::Windows::Forms::TextBox^  fogLightLampNoteTextBox;
	private: System::Windows::Forms::TextBox^  reverseLightLampNoteTextBox;
	private: System::Windows::Forms::TextBox^  brakeLightLampNoteTextBox;
	private: System::Windows::Forms::TextBox^  turnLLeftLampNoteTextBox;
	private: System::Windows::Forms::TextBox^  turnLRightLampNoteTextBox;
	private: System::Windows::Forms::TextBox^  posLLeftLampNoteTextBox;
	private: System::Windows::Forms::TextBox^  posLRightLampNoteTextBox;

	private: System::Windows::Forms::Label^  dutyCycleLabel;

	private: System::Windows::Forms::TextBox^  dutyCycleTextBox;
	private: System::Windows::Forms::GroupBox^  detectedLampTypeGroupBox;
	private: System::Windows::Forms::Label^  detectedLampTypeLabel;
	private: System::Windows::Forms::TextBox^  fogLightResultTextBox;
	private: System::Windows::Forms::TextBox^  reverseLightResultTextBox;
	private: System::Windows::Forms::TextBox^  brakeLightResultTextBox;
	private: System::Windows::Forms::TextBox^  turnLLeftResultTextBox;
	private: System::Windows::Forms::TextBox^  turnLRightResultTextBox;
	private: System::Windows::Forms::TextBox^  posLLeftResultTextBox;
	private: System::Windows::Forms::TextBox^  posLRightResultTextBox;
	private: System::Windows::Forms::Label^  waitTimeLabel;
	private: System::Windows::Forms::TextBox^  waitTimeTextBox;
	private: System::Windows::Forms::ToolStripMenuItem^  toe8872ControlToolStripMenuItem;
private: System::Windows::Forms::Button^  turnLButton;
private: System::Windows::Forms::Button^  posLButton;
private: System::Windows::Forms::Button^  brakeLButton;




	//***********************************************************************************************************
	// methods & delegates:
	//***********************************************************************************************************
	public:
		//creating the delegate for function updateComLogBoxMethod()
		//-> event serialPort1_DataReceived() is executed in a different thread than the function it should call
		//-> therefore a delegate has to be invoked in order to call the function
		delegate void updateComLogBox();
		updateComLogBox^ updateComLogBoxDelegate;

	
	//***********************************************************************************************************
	// updateComLogBoxMethod()
	// -----------------------	
	// method to read the incoming data from the serial port 
	// serialPort.ReadLine() is executed in an extra thread and can therefore not
	// access the components of form1
	//
	//***********************************************************************************************************
	public:
		void updateComLogBoxMethod()
		{
			String^ tempInputBuffer;

			//read the serial port until less than 6 Bytes are left in the internal input buffer.
			while(serialPort1->BytesToRead > 6)
			{
				try
				{

					tempInputBuffer = serialPort1->ReadExisting();
					//store only messages beginning with the '<' character
					if (tempInputBuffer[0] == '<')
					{
						incomingDataBuffer[incomingDataBufferIndex] = tempInputBuffer;

						//check incoming message and proceed 
						if (incomingDataBuffer[incomingDataBufferIndex] == "<C1_FIN")
						{
							//end of transmission for characterisation 1 reached -> process messages
							stateMachine_state = PROCESS_RESULTS;
							stateMachineHandler();
						}
						else
						{
							incomingDataBufferIndex++;
							if (incomingDataBufferIndex == INCOMING_DATA_BUFFER_SIZE)
							{
								commLogBox->AppendText("[ERROR]: did not receive classification finished msg. " + Environment::NewLine);
								commLogBox->AppendText("[ERROR]: please repeat classification. " + Environment::NewLine);
								incomingDataBufferIndex=0;
							}
						}
					}
					else
					{	
						//display messages that do not comply with the classification client syntax
						//(which do not start with '<'
						commLogBox->AppendText("[ECU]: " + tempInputBuffer + Environment::NewLine);
					}		
				}
				catch (TimeoutException ^)
				{
					commLogBox->AppendText("[ERROR]: serial port read data timeout " + Environment::NewLine);
					//clear internal serial port input buffer
					serialPort1->DiscardInBuffer();
				}
			}
		}
	//***********************************************************************************************************
	// incomingMsgHandler()
	// -----------------------	
	// method to determine what kind of message is incoming and to proceed the next steps necessary
	//
	// each message in the incomingDataBuffer is processed.	
	//***********************************************************************************************************
	public:
		void incomingMsgHandler()
		{
			String^ incomingMessageType;
			int incomingDataBufferSize = incomingDataBufferIndex;
			int i;
			bool isFirstMessageOfTypeC1 = true;
			bool isFirstMessageOfTypeC2 = true;
			bool isFirstMessageOfTypeC3 = true;
			bool isFirstMessageOfTypeC4 = true;
			bool isFirstMessageOfTypeC5 = true;
			bool isFirstMessageOfTypeC6 = true;
			bool isFirstMessageOfTypeC7 = true;
			bool isFirstMessageOfTypeIL = true;
			bool isFirstMessageOfTypeRE = true;
			bool isFirstMessageOfTypeOL = true;

			String^ tempBuffer;

			//variable to store the first line after the measurement info header
			int startLineDataBlock;

			//process all messages in the buffer
			for (i=0; i<incomingDataBufferSize; i++)
			{
				incomingMessageType = incomingDataBuffer[i]->Substring(1,2);

				if (incomingMessageType == "C1")	
				{
					//!! At the moment slope values are not processed -> this is done via vba code in the excel sheet

					//characteristic 1 - slope analysis
					/* if (isFirstMessageOfTypeC1 == true)
					{
						currentLineSlopeResults++;
						//write measurement info header into worksheet
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResults, 1, "Characteristic");
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResults, 2, "slopeAnalysis");
						currentLineSlopeResults++;
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResults, 1, "Lamp Channel");
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResults, 2, lampChannelLookUp[currentWorklistIndex]);
						currentLineSlopeResults++;
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResults, 1, "Lamp ID");
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResults, 2, lampNumberLookUp[currentWorklistIndex] + lampNoteLookUp[currentWorklistIndex]);
						currentLineSlopeResults++;
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResults, 1, "Lamp Type");
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResults, 2, lampTypeLookUp[currentWorklistIndex]);	
						currentLineSlopeResults += 1;
						startLineDataBlock = currentLineSlopeResults;
						isFirstMessageOfTypeC1 = false;
					}
					processC1Message(i, startLineDataBlock);
					*/
				}	 	
				else if (incomingMessageType == "C2")	
				{
					//characteristic 2 - time to final value
				}
				else if (incomingMessageType == "C3")	
				{
					//characteristic 3 - curve integration
				}
				else if (incomingMessageType == "C4")	
				{
					//characteristic 4 - monotonous slope
				}
				else if (incomingMessageType == "C5")	
				{
					//characteristic 5 - oscillation
				}
				else if (incomingMessageType == "C6")	
				{
					//characteristic 6 - peak magnitude
				}
				else if (incomingMessageType == "C7")	
				{
					//characteristic 7 - final value
				}
				else if (incomingMessageType == "IL")
				{
					//current sample messages
					if (isFirstMessageOfTypeIL == true)
					{
						//read the current voltage of the toellner power supply
						 
						tempBuffer = Convert::ToString(toeControlDialog->getCurrentOutputVoltage());
						if (tempBuffer == "0")
						{
							//power supply control is deactivated
							tempBuffer = "";
						}
						else
						{
							tempBuffer = "+" + tempBuffer;
						}
						currentLineCurrentSamples++;
						//write measurement info header into worksheet
						myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, currentLineCurrentSamples, 1, "Current Samples");
						currentLineCurrentSamples++;
						myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, currentLineCurrentSamples, 1, "Lamp Channel");
						myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, currentLineCurrentSamples, 2, lampChannelLookUp[currentWorklistIndex]);
						currentLineCurrentSamples++;
						myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, currentLineCurrentSamples, 1, "Lamp ID");
						myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, currentLineCurrentSamples, 2, lampNumberLookUp[currentWorklistIndex] + lampNoteLookUp[currentWorklistIndex] + tempBuffer);
						currentLineCurrentSamples++;
						myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, currentLineCurrentSamples, 1, "Lamp Type");
						myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, currentLineCurrentSamples, 2, lampTypeLookUp[currentWorklistIndex]);	
						currentLineCurrentSamples += 1;
						startLineDataBlock = currentLineCurrentSamples;
						isFirstMessageOfTypeIL = false;
					}
					processILMessage(i, startLineDataBlock);
				}
				else if (incomingMessageType == "RE")
				{
					//lamp type detection result (old or new)
					processREMessage(i);
				}
				else if (incomingMessageType == "OL")
				{
					//lamp type detection result (old or new)
					processOLMessage(i, startLineDataBlock);
				}
				else
				{
					commLogBox->AppendText("[ECU]: " + incomingData + Environment::NewLine);
				}
			}

			//set C1 write cursor to the second line after the longest C1 data block
			if (currentLineSlopeResultsDetail > currentLineSlopeResultsSample)
			{
				currentLineSlopeResults = currentLineSlopeResultsDetail + 1;
				myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, 1, 2, Convert::ToString(currentLineSlopeResults));
			}
			else if (currentLineSlopeResultsDetail != 0 && currentLineSlopeResultsSample != 0)
			{
				currentLineSlopeResults = currentLineSlopeResultsSample + 1;
				myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, 1, 2, Convert::ToString(currentLineSlopeResults));
			}


			//all messages stored in the inputBuffer are processed
			stateMachine_state = SETUP_NEXT_RUN;
			stateMachineHandler();
		}
	
	//***********************************************************************************************************
	// processC1Message()
	// -----------------------	
	// Aim of this function is to decode the incoming messages for characteristic 1 and write the values into
	// the correct destination in the Excel sheet.
	//
	// bufferIndex -> the index of the currently processed message in the incomingMessages array.
	//***********************************************************************************************************
	public:
		void processC1Message(int bufferIndex, int startLineDataBlock)
		{
			String^ incomingValueType;
			String^ valueTimeWindow;
			String^ incomingValue;

			incomingValueType = incomingDataBuffer[bufferIndex]->Substring(4,3);
			valueTimeWindow = incomingDataBuffer[bufferIndex]->Substring(8,3);
			incomingValue = incomingDataBuffer[bufferIndex]->Substring(12);

			//write data block labels (on first call of this function)
			if (currentLineSlopeResults == startLineDataBlock)
			{	
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock, 1, "Standard Mode Results:");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+2 , 1, "Maximum neg. slope result:");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+3 , 1, "Magnitude result:");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+4 , 1, "Mean value result:");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+5 , 1, "Overall result:");
					
					if (selectedInformationModeNumber == INFOMODE_DETAIL || selectedInformationModeNumber == INFOMODE_SAMPLE) 
					{
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock, 3, "Detail Mode Results:");
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 3, "Value Type");
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 4, "Time Window:");
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 5, "Value:");
					}

					if (selectedInformationModeNumber == INFOMODE_SAMPLE)
					{
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock, 6, "Sample Mode Results:");
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 6, "Time window:");
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 7, "Value:");
					}
					currentLineSlopeResults++;
					currentLineSlopeResultsDetail = startLineDataBlock + 2;
					currentLineSlopeResultsSample = startLineDataBlock + 2;
			}


			if (incomingValueType == "MNS")
			{
				//incoming value for maximum negative slope (detail mode)
				//store variable in excel, depending on time window
				if (valueTimeWindow == "LED")
				{
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 3, "Max. neg. slope");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 4, "Led");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 5, incomingValue);
					currentLineSlopeResultsDetail += 1;
				}
				else if (valueTimeWindow == "BUL")
				{
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 3, "Max. neg. slope");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 4, "Bulb");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 5, incomingValue);
					currentLineSlopeResultsDetail += 1;
				}
				else if (valueTimeWindow == "END")
				{
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 3, "Max. neg. slope");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 4, "End");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 5, incomingValue);
					currentLineSlopeResultsDetail += 1;
				}
				else
				{
					commLogBox->AppendText("[INFO]: time window for max negative slope not readable" + Environment::NewLine);
				}
			}
			else if (incomingValueType == "MAG")
			{
				//incoming value for magnitude of negative slope (detail mode)
				//store variable, depending on time window
				if (valueTimeWindow == "LED")
				{
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 3, "Magnitude");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 4, "Led");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 5, incomingValue);
					currentLineSlopeResultsDetail += 1;
				}
				else if (valueTimeWindow == "BUL")
				{
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 3, "Magnitude");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 4, "Bulb");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 5, incomingValue);
					currentLineSlopeResultsDetail += 1;
				}
				else if (valueTimeWindow == "END")
				{
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 3, "Magnitude");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 4, "End");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 5, incomingValue);
					currentLineSlopeResultsDetail += 1;
				}
				else
				{
					commLogBox->AppendText("[INFO]: time window for magnitude neg slope not readable" + Environment::NewLine);
				}
			}
			else if (incomingValueType == "MEA")
			{
				//incoming value for mean values (detail mode)
				//store variable, depending on time window
				if (valueTimeWindow == "LED")
				{
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 3, "Mean value");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 4, "Led");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 5, incomingValue);
					currentLineSlopeResultsDetail += 1;
				}
				else if (valueTimeWindow == "BUL")
				{
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 3, "Mean value");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 4, "Bulb");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 5, incomingValue);
					currentLineSlopeResultsDetail += 1;
				}
				else if (valueTimeWindow == "END")
				{
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 3, "Mean value");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 4, "End");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsDetail, 5, incomingValue);
					currentLineSlopeResultsDetail += 1;
				}
				else
				{
					commLogBox->AppendText("[INFO]: time window for mean value neg slope not readable" + Environment::NewLine);
				}
			}
			else if (incomingValueType == "SLO")
			{
				//incoming value for slope values (sample mode)
				//store variable, depending on time window
				if (valueTimeWindow == "LED")
				{
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsSample, 6, "Led");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsSample, 7, incomingValue);
					currentLineSlopeResultsSample += 1;
				}
				else if (valueTimeWindow == "BUL")
				{
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsSample, 6, "Bulb");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsSample, 7, incomingValue);
					currentLineSlopeResultsSample += 1;
				}
				else if (valueTimeWindow == "END")
				{
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsSample, 6, "End");
					myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, currentLineSlopeResultsSample, 7, incomingValue);
					currentLineSlopeResultsSample += 1;
				}
				else
				{
					commLogBox->AppendText("[INFO]: time window for slope value not readable" + Environment::NewLine);
				}
			}
			else if (incomingValueType == "RES")
			{
				//incoming value for classification results (standard mode)
				//store variable, depending on result type
				if (valueTimeWindow == "MNS")
				{
					if (incomingValue == "FLASH")
					{
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 1, "Maximum neg. slope result:");
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 2, "flashing");
					}
					else if (incomingValue == "NONFL")
					{
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 1, "Maximum neg. slope result:");
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 2, "non-flashing");
					}
					else
					{
						commLogBox->AppendText("[INFO]: result for characteristic 1 - MNS not readable" + Environment::NewLine);
					}
				}
				else if (valueTimeWindow == "MAG")
				{
					if (incomingValue == "FLASH")
					{
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 1, "Magnitude result:");
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 2, "flashing");
					}
					else if (incomingValue == "NONFL")
					{
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 1, "Magnitude result:");
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 2, "non-flashing");
					}
					else
					{
						commLogBox->AppendText("[INFO]: result for characteristic 1 - MAG not readable" + Environment::NewLine);
					}
				}
				else if (valueTimeWindow == "MEA")
				{
					if (incomingValue == "FLASH")
					{
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 1, "Mean value result:");
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 2, "flashing");
					}
					else if (incomingValue == "NONFL")
					{
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 1, "Mean value result:");
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 2, "non-flashing");
					}
					else
					{
						commLogBox->AppendText("[INFO]: result for characteristic 1 - MEA not readable" + Environment::NewLine);
					}
				}
				else if (valueTimeWindow == "ALL")
				{
					if (incomingValue == "FLASH")
					{
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 1, "Overall result:");
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 2, "flashing");
					}
					else if (incomingValue == "NONFL")
					{
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 1, "Overall result:");
						myExcelInstance->writeExcelCell(SLOPEANALYSIS_SHEET, startLineDataBlock+1, 2, "non-flashing");
					}
					else
					{
						commLogBox->AppendText("[INFO]: result for characteristic 1 - overall not readable" + Environment::NewLine);
					}
				}
				else
				{
					commLogBox->AppendText("[INFO]: result type for characteristic 1 not readable" + Environment::NewLine);
				}
			}
			else
			{
				commLogBox->AppendText("[INFO]: value type for characteristic 1 not readable" + Environment::NewLine);
			}
		}

	//***********************************************************************************************************
	// processILMessage()
	// -----------------------	
	// Aim of this function is to decode the incoming messages for the current samples and write the values into
	// the correct destination in the Excel sheet.
	//
	// bufferIndex -> the index of the currently processed message in the incomingMessages array.
	//***********************************************************************************************************
	public:
		void processILMessage(int bufferIndex, int startLineDataBlock)
		{
			String^ incomingValueType;
			String^ incomingNormedType;
			String^ incomingValue;

			incomingValueType = incomingDataBuffer[bufferIndex]->Substring(4,3);
			incomingNormedType = incomingDataBuffer[bufferIndex]->Substring(8,3);
			incomingValue = incomingDataBuffer[bufferIndex]->Substring(12);

			//write data block labels (on first call of this function)
			if (currentLineCurrentSamples == startLineDataBlock)
			{	
					myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, startLineDataBlock, 1, "LedWindowSamples");
					myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, startLineDataBlock, 2, "LedWindowSamples - normed");

					myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, startLineDataBlock, 4, "BulbWindowSamples");
					myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, startLineDataBlock, 5, "BulbWindowSamples - normed");

					myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, startLineDataBlock, 7, "EndWindowSamples");
					myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, startLineDataBlock, 8, "EndWindowSamples - normed");

					currentLineCurrentSamplesLed = startLineDataBlock + 2;
					currentLineCurrentSamplesLedNormed = startLineDataBlock + 2;
					currentLineCurrentSamplesBulb = startLineDataBlock + 2;
					currentLineCurrentSamplesBulbNormed = startLineDataBlock + 2;
					currentLineCurrentSamplesEnd = startLineDataBlock + 2;
					currentLineCurrentSamplesEndNormed = startLineDataBlock + 2;
			}


			if (incomingValueType == "LED")
			{
				//incoming value for led window sample, not normed
				if (incomingNormedType == "NOT")
				{
					myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, currentLineCurrentSamplesLed, 1, incomingValue);
					currentLineCurrentSamplesLed += 1;
				}
				//incoming value for led window sample, normed
				else if (incomingNormedType == "NOR")
				{
					myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, currentLineCurrentSamplesLedNormed, 2, incomingValue);
					currentLineCurrentSamplesLedNormed += 1;
				}
				else
				{
					commLogBox->AppendText("[INFO]: sample value for led time window not readable" + Environment::NewLine);
				}
			}

			else if (incomingValueType == "BUL")
			{
				//incoming value for bulb window sample, not normed
				if (incomingNormedType == "NOT")
				{
					myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, currentLineCurrentSamplesBulb, 4, incomingValue);
					currentLineCurrentSamplesBulb += 1;
				}
				//incoming value for bulb window sample, normed
				else if (incomingNormedType == "NOR")
				{
					myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, currentLineCurrentSamplesBulbNormed, 5, incomingValue);
					currentLineCurrentSamplesBulbNormed += 1;
				}
				else
				{
					commLogBox->AppendText("[INFO]: sample value for bulb time window not readable" + Environment::NewLine);
				}
			}

			else if (incomingValueType == "END")
			{
				//incoming value for end window sample, not normed
				if (incomingNormedType == "NOT")
				{
					myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, currentLineCurrentSamplesEnd, 7, incomingValue);
					currentLineCurrentSamplesEnd += 1;
				}
				//incoming value for end window sample, normed
				else if (incomingNormedType == "NOR")
				{
					myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, currentLineCurrentSamplesEndNormed, 8, incomingValue);
					currentLineCurrentSamplesEndNormed += 1;
				}
				else
				{
					commLogBox->AppendText("[INFO]: sample value for end time window not readable" + Environment::NewLine);
				}
			}

			//set the write cursor for the current sample worksheet to the third line after the last sample line
			currentLineCurrentSamples = currentLineCurrentSamplesLed+1;
			myExcelInstance->writeExcelCell(CURRENTSAMPLES_SHEET, 1, 2, Convert::ToString(currentLineCurrentSamples));
		}

	//***********************************************************************************************************
	// processREMessage()
	// -----------------------	
	// Aim of this function is to decode the incoming messages for the lamp type detection and display the content
	// of these messages on the graphical user inteface of the software.
	//
	// bufferIndex -> the index of the currently processed message in the incomingMessages array.
	//***********************************************************************************************************
	public:
		void processREMessage(int bufferIndex)
		{	
			String^ incomingAlgorithmType;
			String^ incomingDetectedLampType;
			String^ incomingFlashingType;

			String^ incomingMessage;


			
			incomingAlgorithmType = incomingDataBuffer[bufferIndex]->Substring(4,3);
			incomingDetectedLampType = incomingDataBuffer[bufferIndex]->Substring(8,3);
			incomingMessage = incomingDataBuffer[bufferIndex];
			if (incomingMessage->Length > 11)
			{
				//only try to extract the flashing type when the message is long enough to be a message of the new 
				//detection algorithm (the flashing information is not available in the messages of the old detection algorithm)
				incomingFlashingType = incomingDataBuffer[bufferIndex]->Substring(12);
			}

			//decode the message information into readable stuff
			if (incomingDetectedLampType == "BUL")
			{
				incomingDetectedLampType = "Bulb";
				incomingFlashingType = "Non-Flashing";
			}
			else if (incomingDetectedLampType == "LED")
			{
				incomingDetectedLampType = "Led";
				incomingFlashingType = "Flashing";
			}
			else if (incomingDetectedLampType == "OPE")
			{
				incomingDetectedLampType = "Open Load";
				incomingFlashingType = "";
			}
			else if (incomingDetectedLampType == "UNK")
			{
				incomingDetectedLampType = "Unknown";
				incomingFlashingType = "";
			}

			if (incomingFlashingType == "NON")
			{
				incomingFlashingType = "Non-Flashing";
			}
			else if (incomingFlashingType == "FLA")
			{
				incomingFlashingType = "Flashing";
			}

			//process messages of the "old" lamp detection algorithm
			if (incomingAlgorithmType == "OLD")
			{
				if (incomingDetectedLampType == "VOL")
				{
					//message contains the operating voltage, measured by the ecu
					ecuOperatingVoltage = incomingFlashingType;
				}
				else
				{

					//display the detected lamp type in the "resultTextBox" for the current lamp channel.
					if (currentWorklistIndex == POSLRIGHT)
					{
						posLRightResultTextBox->Text = incomingDetectedLampType + incomingFlashingType;
					}
					else if (currentWorklistIndex == POSLLEFT)
					{
						posLLeftResultTextBox->Text = incomingDetectedLampType + incomingFlashingType;
					}
					else if (currentWorklistIndex == TURNLRIGHT)
					{
						turnLRightResultTextBox->Text = incomingDetectedLampType + incomingFlashingType;
					}
					else if (currentWorklistIndex == TURNLLEFT)
					{
						turnLLeftResultTextBox->Text = incomingDetectedLampType + incomingFlashingType;
					}
					else if (currentWorklistIndex == BRAKELIGHT)
					{
						brakeLightResultTextBox->Text = incomingDetectedLampType + incomingFlashingType;
					}
					else if (currentWorklistIndex == REVERSELIGHT)
					{
						reverseLightResultTextBox->Text = incomingDetectedLampType + incomingFlashingType;
					}
					else if (currentWorklistIndex == FOGLIGHT)
					{
						fogLightResultTextBox->Text = incomingDetectedLampType + incomingFlashingType;
					}


					
					//write timestamp
					char tmpBuf[128];
					String^ tmpString;
					_strtime_s( tmpBuf, 128 );
					String^ cliStr = gcnew String(tmpBuf);
					myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 1, cliStr);
					_strdate_s( tmpBuf, 128 );
					String^ cliStr2 = gcnew String(tmpBuf);
					myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 2, cliStr2);

					//read current operating voltage
					tmpString = Convert::ToString(toeControlDialog->getCurrentOutputVoltage());
					if (tmpString == "0")
					{
						//power supply control is deactivated
						tmpString = "";
					}
					else
					{	
						//if valid voltage has been read -> add the '+' character in front of the value
						tmpString = "+" + tmpString;
					}

					//write the classification method
					myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 3, "classification old");
					
					//write the lamp channel
					myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 4, lampChannelLookUp[currentWorklistIndex]);

					//write the lamp device name + voltage information
					myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 5, lampNumberLookUp[currentWorklistIndex] + lampNoteLookUp[currentWorklistIndex] + tmpString);

					//write the user defined flashing type for the current device
					myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 6, lampTypeLookUp[currentWorklistIndex]);	

					//write the operating voltage of the ecu
					myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 7, ecuOperatingVoltage);

					//write the overall classification result
					myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 8, incomingDetectedLampType + incomingFlashingType);

					currentLineClassificationErrors += 1;

					//reset the values for the single classification results
					slopeAnalysisResult = "not set";
					currentIntegralResult = "not set";
					statisticalAnalysisResult = "not set";
					ecuOperatingVoltage = "not set";

					//update the write cursor for the classification error worksheet
					myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, 1, 2, Convert::ToString(currentLineClassificationErrors));
					
				}
				
			}
			// process messages of the "new" matlab classification algorithm
			else if (incomingAlgorithmType == "NEW")
			{
				//check the type of message, coming from the matlab classification algorithm
				if (incomingDetectedLampType == "ALL")
				{
					//message contains the overall result for the matlab classification algorithm
					//-> compare the incoming result with the value of the flashing type check box
					//   -> if classification result differs from the set flashing type
					//	     -> write message in the classificationErrors worksheet
					if (currentWorklistIndex == POSLRIGHT)
					{
						posLRightResultTextBox->Text = incomingFlashingType;
					}
					else if (currentWorklistIndex == POSLLEFT)
					{
						posLLeftResultTextBox->Text = incomingFlashingType;
					}
					else if (currentWorklistIndex == TURNLRIGHT)
					{
						turnLRightResultTextBox->Text = incomingFlashingType;
					}
					else if (currentWorklistIndex == TURNLLEFT)
					{
						turnLLeftResultTextBox->Text = incomingFlashingType;
					}
					else if (currentWorklistIndex == BRAKELIGHT)
					{
						brakeLightResultTextBox->Text = incomingFlashingType;
					}
					else if (currentWorklistIndex == REVERSELIGHT)
					{
						reverseLightResultTextBox->Text = incomingFlashingType;
					}
					else if (currentWorklistIndex == FOGLIGHT)
					{
						fogLightResultTextBox->Text = incomingFlashingType;
					}

					//check if the classification result is the same than the set flashing type for the currently processed lamp device
					//-> if not write info in the classificationErrors worksheet
					if (lampTypeLookUp[currentWorklistIndex] != incomingFlashingType)
					{
						//write timestamp
						char tmpBuf[128];
						String^ tmpString;
						_strtime_s( tmpBuf, 128 );
						String^ cliStr = gcnew String(tmpBuf);
						myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 1, cliStr);
						_strdate_s( tmpBuf, 128 );
						String^ cliStr2 = gcnew String(tmpBuf);
						myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 2, cliStr2);

						//read current operating voltage
						tmpString = Convert::ToString(toeControlDialog->getCurrentOutputVoltage());
						if (tmpString == "0")
						{
							//power supply control is deactivated
							tmpString = "";
						}
						else
						{	
							//if valid voltage has been read -> add the '+' character in front of the value
							tmpString = "+" + tmpString;
						}

						//write the classification method
						if (selectedCharacteristicNumber == 205) 
						{
							myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 3, "matlab, defensive");
						}
						else if (selectedCharacteristicNumber == 206)
						{
							myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 3, "matlab, offensive");
						}
						//write the lamp channel
						myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 4, lampChannelLookUp[currentWorklistIndex]);

						//write the lamp device name + voltage information
						myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 5, lampNumberLookUp[currentWorklistIndex] + lampNoteLookUp[currentWorklistIndex] + tmpString);

						//write the user defined flashing type for the current device
						myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 6, lampTypeLookUp[currentWorklistIndex]);	

						//write the operating voltage of the ecu
						myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 7, ecuOperatingVoltage);

						//write the overall classification result
						myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 8, incomingFlashingType);

						//write the slope analysis result
						myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 9, slopeAnalysisResult);

						//write the current integral & peak result 
						myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 10, currentIntegralResult);

						//write the statistical analysis result
						myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, currentLineClassificationErrors, 11, statisticalAnalysisResult);

						currentLineClassificationErrors += 1;

						//reset the values for the single classification results
						slopeAnalysisResult = "not set";
						currentIntegralResult = "not set";
						statisticalAnalysisResult = "not set";
						ecuOperatingVoltage = "not set";

						//update the write cursor for the classification error worksheet
						myExcelInstance->writeExcelCell(CLASSIFICATIONERRORS_SHEET, 1, 2, Convert::ToString(currentLineClassificationErrors));
					}
				}
				else if (incomingDetectedLampType == "SLO")
				{
					//message contains the slope analysis result
					commLogBox->AppendText("[ECU]: slope result:             " + incomingFlashingType + Environment::NewLine);
					slopeAnalysisResult = incomingFlashingType;
				} 
				else if (incomingDetectedLampType == "INT")
				{
					//message contains the current integral & peak result
					commLogBox->AppendText("[ECU]: int & peak result:     " + incomingFlashingType + Environment::NewLine);
					currentIntegralResult = incomingFlashingType;
				}
				else if (incomingDetectedLampType == "STA")
				{
					//message contains the statistical analysis result
					commLogBox->AppendText("[ECU]: statistic result:         " + incomingFlashingType + Environment::NewLine);
					statisticalAnalysisResult = incomingFlashingType;
				}
				else if (incomingDetectedLampType == "VOL")
				{
					//message contains the operating voltage, measured by the ecu
					ecuOperatingVoltage = incomingFlashingType;
				}
			}

		}

//***********************************************************************************************************
	// processOLMessage()
	// -----------------------	
	// Aim of this function is to decode the incoming messages for the open load detection (on and off state) and display the content
	// of these messages on the graphical user inteface of the software.
	//
	// bufferIndex -> the index of the currently processed message in the incomingMessages array.
	//***********************************************************************************************************
	public:
		void processOLMessage(int bufferIndex, int startLineDataBlock)
		{	
			String^ incomingOpenLoadResult;
			
			incomingOpenLoadResult = incomingDataBuffer[bufferIndex]->Substring(8,3);

			//decode the message information into readable stuff
			if (incomingOpenLoadResult == "OLO")
			{
				incomingOpenLoadResult = "Open Load";
			}
			else if (incomingOpenLoadResult == "IOK")
			{
				incomingOpenLoadResult = "Lamp Device";
			}
			else if (incomingOpenLoadResult == "UNK")
			{
				incomingOpenLoadResult = "Unknown";
			}

			//display the detected lamp type in the "resultTextBox" for the current lamp channel.
			if (currentWorklistIndex == POSLRIGHT)
			{
				posLRightResultTextBox->Text = incomingOpenLoadResult;
			}
			else if (currentWorklistIndex == POSLLEFT)
			{
				posLLeftResultTextBox->Text = incomingOpenLoadResult;
			}
			else if (currentWorklistIndex == TURNLRIGHT)
			{
				turnLRightResultTextBox->Text = incomingOpenLoadResult;
			}
			else if (currentWorklistIndex == TURNLLEFT)
			{
				turnLLeftResultTextBox->Text = incomingOpenLoadResult;
			}
			else if (currentWorklistIndex == BRAKELIGHT)
			{
				brakeLightResultTextBox->Text = incomingOpenLoadResult;
			}
			else if (currentWorklistIndex == REVERSELIGHT)
			{
				reverseLightResultTextBox->Text = incomingOpenLoadResult;
			}
			else if (currentWorklistIndex == FOGLIGHT)
			{
				fogLightResultTextBox->Text = incomingOpenLoadResult;
			}
		}


	//***********************************************************************************************************
	// stateMachineHandler()
	// -----------------------	
	//
	// Aim of this function is to manage the state machine of the software. 
	//
	// - all lamp channels that are checked (activated) for testing are stored in the lampChannelLookUp array
	//   -> index represents the lamp number, e.g. POSLRIGHT = 0
	//   -> if lamp channel is active, the regarding array position is ==1
	//   -> the currently tested lamp channel is identified by variable "currentWorklistIndex"
	//***********************************************************************************************************
	public:
		void stateMachineHandler()
		{
			String^ currentChannel;
			
			switch(stateMachine_state)
			{
				case START_CLASSIFICATION:
					currentChannel = lampChannelLookUp[currentWorklistIndex];
					commLogBox->AppendText("[INFO]: processing channel: " + currentChannel + Environment::NewLine);
					Sleep(100);

					//send message to ECU
					sendHpmMessage();
					//determine next state, depending on current command or current mode
					if (dutyCycleLabel->Visible == false)
					{
						//no pwm mode -> continue with next state
						stateMachine_state = WAIT_FOR_RESULTS;
					}
					else
					{
						//pwm mode -> finish state machine
						stateMachine_state = IDLE;
					}
					break;

				case WAIT_FOR_RESULTS:					
					break;

				case PROCESS_RESULTS:
					incomingMsgHandler();
					break;

				case SETUP_NEXT_RUN:
					//INFO: The sequence how the channels are processed and the meaning of # Iterations is like that:
					//   
					//		-> if there are several channels selected -> these channels are processed after each other
					//         (worklist), with the specified wait time in between
					//
					//		-> the processing of the selected channels sequence is repeated n times. (n = # Iterations)
					//
					//		-> if the toellner power supply control modul is enabled the voltage switch is processed 
					//		   after the complete worklist has been processed x times. (x = step after ? iterations)
					//
					//first of all check if the end of the worklist is reached and if there are iterations left to run
					if (currentWorklistIndex == (lampChannelWorkList->Length)-1)
					{
						currentIteration++;

						//end of worklist is reached -> check for outstanding iterations
						if (currentIteration < numberOfIterations)
						{
							//update voltage output of power supply
							toeControlDialog->nextIterationNotify(currentIteration);
							
							//if there is a waiting time specified -> wait and then continue
							Sleep(waitingTimeBetweenIterations);

							//start next iteration run
							commLogBox->AppendText("[INFO]: starting iteration # " + currentIteration + Environment::NewLine);
							
							stateMachine_state = START_CLASSIFICATION;

							//check for first item on worklist
							for (int j=0; j<lampChannelWorkList->Length; j++)
							{
								if (lampChannelWorkList[j]==true)
								{
									currentWorklistIndex = j;
									break;
								}
							}
							
							//clear the incomingDataBuffer and the buffer index
							incomingDataBuffer->Clear(incomingDataBuffer, 0, incomingDataBuffer->Length);
							incomingDataBufferIndex=0;
							stateMachineHandler();
						}
						else
						{	
							//all iterations are done -> classifications are finished -> wait for user input
							commLogBox->AppendText("[INFO]: classification finished." + Environment::NewLine);
							currentIteration = 0;
							currentWorklistIndex=0;
							stateMachine_state = IDLE;
							stateMachineHandler();
						}
					}
					else
					{
						//check for next item on worklist
						for (int i=currentWorklistIndex+1; i<lampChannelWorkList->Length; i++)
						{
							if (lampChannelWorkList[i]==true)
							{
								//if there is a waiting time specified -> wait and then continue
								Sleep(waitingTimeBetweenIterations);

								//next lamp channel to be processed
								currentWorklistIndex = i;
								//start next iteration run
								stateMachine_state = START_CLASSIFICATION;
							
								//clear the incomingDataBuffer and the buffer index
								incomingDataBuffer->Clear(incomingDataBuffer, 0, incomingDataBuffer->Length);
								incomingDataBufferIndex=0;
								stateMachineHandler();
								break;
							}
							else
							{
								if (i == (lampChannelWorkList->Length)-1)
								{
									//end of worklist is reached
									//-> call stateMachineHandler again to see if there are iterations left to process
									currentWorklistIndex = lampChannelWorkList->Length-1;
									//currentIteration++;
									stateMachineHandler();
								}
							}
						}
					}
					break;

				case IDLE:
					//enable controls (not in pwm mode)
					if (dutyCycleLabel->Visible == false)
					{
						lampSelectionGroupBox->Enabled=true;
					}
					startClassificationButton->Enabled=true;
					usedCharacteristicComboBox->Enabled=true;
					usedCharacterisicLabel->Enabled=true;
					numberIterationTextBox->Enabled=true;
					selectTestTypeLabel->Enabled=true;
					informationModeComboBox->Enabled=true;
					informationModeLabel->Enabled=true;
					connectSerialButton->Enabled=true;
					disconnectSerialButton->Enabled=true;
					break;

				default:
					break;
			}
		}


	//***********************************************************************************************************
	// stateMachineHandler()
	// -----------------------	
	//
	// Aim of this function is to prepare and send the hpm message that triggers the next classification
	// on the ECU / hpm software.
	//***********************************************************************************************************
	public:
		void sendHpmMessage()
		{	
			int dutyCycle;
			
			// in pwm mode the lamp channel byte is used to transmit the duty cycle value.
			if (dutyCycleLabel->Visible == true)
			{
				dutyCycle = Convert::ToInt32(dutyCycleTextBox->Text);
				hpmMessage = "CmdClassifyLamp" + " " + Convert::ToString(dutyCycle) + "," + Convert::ToString(selectedCharacteristicNumber) 
								+ "," + Convert::ToString(selectedInformationModeNumber) + "\n";
			}
			else 
			{
				hpmMessage = "CmdClassifyLamp" + " " + Convert::ToString(currentWorklistIndex) + "," + Convert::ToString(selectedCharacteristicNumber) 
								+ "," + Convert::ToString(selectedInformationModeNumber) + "\n";
			}
			//commLogBox->AppendText("[INFO]: sending message: " + Environment::NewLine);
			//commLogBox->AppendText("[INFO]: " + hpmMessage + Environment::NewLine);

			try
			{
				serialPort1->Write(hpmMessage);
			}
			catch (Exception^)
			{
				commLogBox->AppendText("[ERROR]: problem during message send process." + Environment::NewLine);
				commLogBox->AppendText("[INFO]: please try again." + Environment::NewLine);
			}
		}
	//***********************************************************************************************************
	// form initialization
	//***********************************************************************************************************
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->connectSerialButton = (gcnew System::Windows::Forms::Button());
			this->serialConnectionLabel = (gcnew System::Windows::Forms::Label());
			this->speedGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->speed115200Button = (gcnew System::Windows::Forms::RadioButton());
			this->speed57600Button = (gcnew System::Windows::Forms::RadioButton());
			this->selectPortBox = (gcnew System::Windows::Forms::ComboBox());
			this->selectPortLabel = (gcnew System::Windows::Forms::Label());
			this->commLogBox = (gcnew System::Windows::Forms::TextBox());
			this->commDataLabel = (gcnew System::Windows::Forms::Label());
			this->menuBar = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statisticsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pwmViewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->standardViewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toe8872ControlToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->displaysSoftwareVersionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->displaysSoftwareVersionToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->disconnectSerialButton = (gcnew System::Windows::Forms::Button());
			this->classificationSetupGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->classificationSetupLabel = (gcnew System::Windows::Forms::Label());
			this->classificationSetupInnerGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->turnLButton = (gcnew System::Windows::Forms::Button());
			this->posLButton = (gcnew System::Windows::Forms::Button());
			this->brakeLButton = (gcnew System::Windows::Forms::Button());
			this->dutyCycleLabel = (gcnew System::Windows::Forms::Label());
			this->waitTimeLabel = (gcnew System::Windows::Forms::Label());
			this->dutyCycleTextBox = (gcnew System::Windows::Forms::TextBox());
			this->waitTimeTextBox = (gcnew System::Windows::Forms::TextBox());
			this->detectedLampTypeGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->detectedLampTypeLabel = (gcnew System::Windows::Forms::Label());
			this->fogLightResultTextBox = (gcnew System::Windows::Forms::TextBox());
			this->reverseLightResultTextBox = (gcnew System::Windows::Forms::TextBox());
			this->brakeLightResultTextBox = (gcnew System::Windows::Forms::TextBox());
			this->turnLLeftResultTextBox = (gcnew System::Windows::Forms::TextBox());
			this->turnLRightResultTextBox = (gcnew System::Windows::Forms::TextBox());
			this->posLLeftResultTextBox = (gcnew System::Windows::Forms::TextBox());
			this->posLRightResultTextBox = (gcnew System::Windows::Forms::TextBox());
			this->lampSelectionGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->lampNoteLabel = (gcnew System::Windows::Forms::Label());
			this->fogLightLampNoteTextBox = (gcnew System::Windows::Forms::TextBox());
			this->reverseLightLampNoteTextBox = (gcnew System::Windows::Forms::TextBox());
			this->brakeLightLampNoteTextBox = (gcnew System::Windows::Forms::TextBox());
			this->turnLLeftLampNoteTextBox = (gcnew System::Windows::Forms::TextBox());
			this->turnLRightLampNoteTextBox = (gcnew System::Windows::Forms::TextBox());
			this->posLLeftLampNoteTextBox = (gcnew System::Windows::Forms::TextBox());
			this->posLRightLampNoteTextBox = (gcnew System::Windows::Forms::TextBox());
			this->lampNumberLabel = (gcnew System::Windows::Forms::Label());
			this->fogLightLampNrTextBox = (gcnew System::Windows::Forms::TextBox());
			this->reverseLightLampNrTextBox = (gcnew System::Windows::Forms::TextBox());
			this->brakeLightLampNrTextBox = (gcnew System::Windows::Forms::TextBox());
			this->turnLLeftLampNrTextBox = (gcnew System::Windows::Forms::TextBox());
			this->turnLRightLampNrTextBox = (gcnew System::Windows::Forms::TextBox());
			this->posLLeftLampNrTextBox = (gcnew System::Windows::Forms::TextBox());
			this->posLRightLampNrTextBox = (gcnew System::Windows::Forms::TextBox());
			this->lampTypeLabel = (gcnew System::Windows::Forms::Label());
			this->fogLightFlashingCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->reverseLightFlashingCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->brakeLightFlashingCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->turnLLeftFlashingCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->turnLRightFlashingCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->posLRightFlashingCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->posLLeftFlashingCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->fogLightSelectCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->reverseLightSelectCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->brakeLightSelectCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->turnLLeftSelectCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->turnLRightSelectCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->posLRightSelectCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->posLLeftSelectCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->allLampsSelectCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->abortClassificationButton = (gcnew System::Windows::Forms::Button());
			this->startClassificationButton = (gcnew System::Windows::Forms::Button());
			this->informationModeComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->informationModeLabel = (gcnew System::Windows::Forms::Label());
			this->lampChannelLabel = (gcnew System::Windows::Forms::Label());
			this->usedCharacteristicComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->usedCharacterisicLabel = (gcnew System::Windows::Forms::Label());
			this->numberIterationTextBox = (gcnew System::Windows::Forms::TextBox());
			this->selectTestTypeLabel = (gcnew System::Windows::Forms::Label());
			this->clearLogButton = (gcnew System::Windows::Forms::Button());
			this->clearResultsButton = (gcnew System::Windows::Forms::Button());
			this->speedGroupBox->SuspendLayout();
			this->menuBar->SuspendLayout();
			this->contextMenuStrip1->SuspendLayout();
			this->classificationSetupGroupBox->SuspendLayout();
			this->classificationSetupInnerGroupBox->SuspendLayout();
			this->detectedLampTypeGroupBox->SuspendLayout();
			this->lampSelectionGroupBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// connectSerialButton
			// 
			this->connectSerialButton->Location = System::Drawing::Point(26, 252);
			this->connectSerialButton->Name = L"connectSerialButton";
			this->connectSerialButton->Size = System::Drawing::Size(104, 38);
			this->connectSerialButton->TabIndex = 0;
			this->connectSerialButton->Text = L"Connect";
			this->connectSerialButton->UseVisualStyleBackColor = true;
			this->connectSerialButton->Click += gcnew System::EventHandler(this, &Form1::connectSerialButton_Click);
			// 
			// serialConnectionLabel
			// 
			this->serialConnectionLabel->AutoSize = true;
			this->serialConnectionLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->serialConnectionLabel->Location = System::Drawing::Point(22, 37);
			this->serialConnectionLabel->Name = L"serialConnectionLabel";
			this->serialConnectionLabel->Size = System::Drawing::Size(169, 20);
			this->serialConnectionLabel->TabIndex = 3;
			this->serialConnectionLabel->Text = L"Serial connection ECU";
			// 
			// speedGroupBox
			// 
			this->speedGroupBox->Controls->Add(this->speed115200Button);
			this->speedGroupBox->Controls->Add(this->speed57600Button);
			this->speedGroupBox->Location = System::Drawing::Point(26, 131);
			this->speedGroupBox->Name = L"speedGroupBox";
			this->speedGroupBox->Size = System::Drawing::Size(234, 101);
			this->speedGroupBox->TabIndex = 4;
			this->speedGroupBox->TabStop = false;
			this->speedGroupBox->Text = L"Speed (baud)";
			// 
			// speed115200Button
			// 
			this->speed115200Button->AutoSize = true;
			this->speed115200Button->Checked = true;
			this->speed115200Button->Enabled = false;
			this->speed115200Button->Location = System::Drawing::Point(26, 71);
			this->speed115200Button->Name = L"speed115200Button";
			this->speed115200Button->Size = System::Drawing::Size(61, 17);
			this->speed115200Button->TabIndex = 1;
			this->speed115200Button->TabStop = true;
			this->speed115200Button->Text = L"115200";
			this->speed115200Button->UseVisualStyleBackColor = true;
			// 
			// speed57600Button
			// 
			this->speed57600Button->AutoSize = true;
			this->speed57600Button->Enabled = false;
			this->speed57600Button->Location = System::Drawing::Point(26, 33);
			this->speed57600Button->Name = L"speed57600Button";
			this->speed57600Button->Size = System::Drawing::Size(55, 17);
			this->speed57600Button->TabIndex = 0;
			this->speed57600Button->TabStop = true;
			this->speed57600Button->Text = L"57600";
			this->speed57600Button->UseVisualStyleBackColor = true;
			// 
			// selectPortBox
			// 
			this->selectPortBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->selectPortBox->FormattingEnabled = true;
			this->selectPortBox->Items->AddRange(gcnew cli::array< System::Object^  >(8) {L"COM1", L"COM2", L"COM3", L"COM4", L"COM5", 
				L"COM6", L"COM7", L"COM8"});
			this->selectPortBox->Location = System::Drawing::Point(26, 84);
			this->selectPortBox->Name = L"selectPortBox";
			this->selectPortBox->Size = System::Drawing::Size(121, 21);
			this->selectPortBox->TabIndex = 5;
			this->selectPortBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::selectPortBox_SelectedIndexChanged);
			// 
			// selectPortLabel
			// 
			this->selectPortLabel->AutoSize = true;
			this->selectPortLabel->Location = System::Drawing::Point(23, 68);
			this->selectPortLabel->Name = L"selectPortLabel";
			this->selectPortLabel->Size = System::Drawing::Size(89, 13);
			this->selectPortLabel->TabIndex = 6;
			this->selectPortLabel->Text = L"Select COM-Port:";
			// 
			// commLogBox
			// 
			this->commLogBox->Location = System::Drawing::Point(294, 68);
			this->commLogBox->Multiline = true;
			this->commLogBox->Name = L"commLogBox";
			this->commLogBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->commLogBox->Size = System::Drawing::Size(217, 223);
			this->commLogBox->TabIndex = 7;
			// 
			// commDataLabel
			// 
			this->commDataLabel->AutoSize = true;
			this->commDataLabel->Location = System::Drawing::Point(291, 44);
			this->commDataLabel->Name = L"commDataLabel";
			this->commDataLabel->Size = System::Drawing::Size(106, 13);
			this->commDataLabel->TabIndex = 8;
			this->commDataLabel->Text = L"Communication data:";
			// 
			// menuBar
			// 
			this->menuBar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fileToolStripMenuItem, 
				this->statisticsToolStripMenuItem});
			this->menuBar->Location = System::Drawing::Point(0, 0);
			this->menuBar->Name = L"menuBar";
			this->menuBar->Size = System::Drawing::Size(540, 24);
			this->menuBar->TabIndex = 9;
			this->menuBar->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->aboutMenuItem, 
				this->toolStripSeparator1, this->exitMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(35, 20);
			this->fileToolStripMenuItem->Text = L"&File";
			// 
			// aboutMenuItem
			// 
			this->aboutMenuItem->Name = L"aboutMenuItem";
			this->aboutMenuItem->Size = System::Drawing::Size(114, 22);
			this->aboutMenuItem->Text = L"&About";
			this->aboutMenuItem->Click += gcnew System::EventHandler(this, &Form1::aboutMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(111, 6);
			// 
			// exitMenuItem
			// 
			this->exitMenuItem->Name = L"exitMenuItem";
			this->exitMenuItem->Size = System::Drawing::Size(114, 22);
			this->exitMenuItem->Text = L"&Exit";
			this->exitMenuItem->Click += gcnew System::EventHandler(this, &Form1::exitMenuItem_Click);
			// 
			// statisticsToolStripMenuItem
			// 
			this->statisticsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->pwmViewToolStripMenuItem, 
				this->standardViewToolStripMenuItem, this->toe8872ControlToolStripMenuItem});
			this->statisticsToolStripMenuItem->Name = L"statisticsToolStripMenuItem";
			this->statisticsToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->statisticsToolStripMenuItem->Text = L"View";
			// 
			// pwmViewToolStripMenuItem
			// 
			this->pwmViewToolStripMenuItem->Name = L"pwmViewToolStripMenuItem";
			this->pwmViewToolStripMenuItem->Size = System::Drawing::Size(161, 22);
			this->pwmViewToolStripMenuItem->Text = L"PWM view";
			this->pwmViewToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::pwmViewToolStripMenuItem_Click);
			// 
			// standardViewToolStripMenuItem
			// 
			this->standardViewToolStripMenuItem->Name = L"standardViewToolStripMenuItem";
			this->standardViewToolStripMenuItem->Size = System::Drawing::Size(161, 22);
			this->standardViewToolStripMenuItem->Text = L"Standard view";
			this->standardViewToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::standardViewToolStripMenuItem_Click);
			// 
			// toe8872ControlToolStripMenuItem
			// 
			this->toe8872ControlToolStripMenuItem->Name = L"toe8872ControlToolStripMenuItem";
			this->toe8872ControlToolStripMenuItem->Size = System::Drawing::Size(161, 22);
			this->toe8872ControlToolStripMenuItem->Text = L"toe8872 control";
			this->toe8872ControlToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::toe8872ControlToolStripMenuItem_Click);
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->displaysSoftwareVersionToolStripMenuItem});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(115, 26);
			// 
			// displaysSoftwareVersionToolStripMenuItem
			// 
			this->displaysSoftwareVersionToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->displaysSoftwareVersionToolStripMenuItem1});
			this->displaysSoftwareVersionToolStripMenuItem->Name = L"displaysSoftwareVersionToolStripMenuItem";
			this->displaysSoftwareVersionToolStripMenuItem->Size = System::Drawing::Size(114, 22);
			this->displaysSoftwareVersionToolStripMenuItem->Text = L"About";
			// 
			// displaysSoftwareVersionToolStripMenuItem1
			// 
			this->displaysSoftwareVersionToolStripMenuItem1->Name = L"displaysSoftwareVersionToolStripMenuItem1";
			this->displaysSoftwareVersionToolStripMenuItem1->Size = System::Drawing::Size(212, 22);
			this->displaysSoftwareVersionToolStripMenuItem1->Text = L"Displays software version.";
			// 
			// serialPort1
			// 
			this->serialPort1->PortName = L"COM3";
			this->serialPort1->ErrorReceived += gcnew System::IO::Ports::SerialErrorReceivedEventHandler(this, &Form1::serialPort1_ErrorReceived);
			this->serialPort1->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(this, &Form1::serialPort1_DataReceived);
			// 
			// disconnectSerialButton
			// 
			this->disconnectSerialButton->Location = System::Drawing::Point(156, 252);
			this->disconnectSerialButton->Name = L"disconnectSerialButton";
			this->disconnectSerialButton->Size = System::Drawing::Size(104, 39);
			this->disconnectSerialButton->TabIndex = 10;
			this->disconnectSerialButton->Text = L"Disconnect";
			this->disconnectSerialButton->UseVisualStyleBackColor = true;
			this->disconnectSerialButton->Click += gcnew System::EventHandler(this, &Form1::DisconnectButton_Click_1);
			// 
			// classificationSetupGroupBox
			// 
			this->classificationSetupGroupBox->BackColor = System::Drawing::Color::Gainsboro;
			this->classificationSetupGroupBox->Controls->Add(this->classificationSetupLabel);
			this->classificationSetupGroupBox->Controls->Add(this->classificationSetupInnerGroupBox);
			this->classificationSetupGroupBox->Location = System::Drawing::Point(1, 310);
			this->classificationSetupGroupBox->Name = L"classificationSetupGroupBox";
			this->classificationSetupGroupBox->Size = System::Drawing::Size(539, 416);
			this->classificationSetupGroupBox->TabIndex = 12;
			this->classificationSetupGroupBox->TabStop = false;
			// 
			// classificationSetupLabel
			// 
			this->classificationSetupLabel->AutoSize = true;
			this->classificationSetupLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->classificationSetupLabel->Location = System::Drawing::Point(21, 21);
			this->classificationSetupLabel->Name = L"classificationSetupLabel";
			this->classificationSetupLabel->Size = System::Drawing::Size(146, 20);
			this->classificationSetupLabel->TabIndex = 15;
			this->classificationSetupLabel->Text = L"Classification setup";
			// 
			// classificationSetupInnerGroupBox
			// 
			this->classificationSetupInnerGroupBox->BackColor = System::Drawing::Color::Transparent;
			this->classificationSetupInnerGroupBox->Controls->Add(this->turnLButton);
			this->classificationSetupInnerGroupBox->Controls->Add(this->posLButton);
			this->classificationSetupInnerGroupBox->Controls->Add(this->brakeLButton);
			this->classificationSetupInnerGroupBox->Controls->Add(this->dutyCycleLabel);
			this->classificationSetupInnerGroupBox->Controls->Add(this->waitTimeLabel);
			this->classificationSetupInnerGroupBox->Controls->Add(this->dutyCycleTextBox);
			this->classificationSetupInnerGroupBox->Controls->Add(this->waitTimeTextBox);
			this->classificationSetupInnerGroupBox->Controls->Add(this->detectedLampTypeGroupBox);
			this->classificationSetupInnerGroupBox->Controls->Add(this->lampSelectionGroupBox);
			this->classificationSetupInnerGroupBox->Controls->Add(this->abortClassificationButton);
			this->classificationSetupInnerGroupBox->Controls->Add(this->startClassificationButton);
			this->classificationSetupInnerGroupBox->Controls->Add(this->informationModeComboBox);
			this->classificationSetupInnerGroupBox->Controls->Add(this->informationModeLabel);
			this->classificationSetupInnerGroupBox->Controls->Add(this->lampChannelLabel);
			this->classificationSetupInnerGroupBox->Controls->Add(this->usedCharacteristicComboBox);
			this->classificationSetupInnerGroupBox->Controls->Add(this->usedCharacterisicLabel);
			this->classificationSetupInnerGroupBox->Controls->Add(this->numberIterationTextBox);
			this->classificationSetupInnerGroupBox->Controls->Add(this->selectTestTypeLabel);
			this->classificationSetupInnerGroupBox->Location = System::Drawing::Point(13, 44);
			this->classificationSetupInnerGroupBox->Name = L"classificationSetupInnerGroupBox";
			this->classificationSetupInnerGroupBox->Size = System::Drawing::Size(514, 319);
			this->classificationSetupInnerGroupBox->TabIndex = 30;
			this->classificationSetupInnerGroupBox->TabStop = false;
			// 
			// turnLButton
			// 
			this->turnLButton->Enabled = false;
			this->turnLButton->Location = System::Drawing::Point(307, 65);
			this->turnLButton->Name = L"turnLButton";
			this->turnLButton->Size = System::Drawing::Size(48, 23);
			this->turnLButton->TabIndex = 54;
			this->turnLButton->Text = L"turnL";
			this->turnLButton->UseVisualStyleBackColor = true;
			this->turnLButton->Visible = false;
			this->turnLButton->Click += gcnew System::EventHandler(this, &Form1::turnLButton_Click);
			// 
			// posLButton
			// 
			this->posLButton->Enabled = false;
			this->posLButton->Location = System::Drawing::Point(252, 66);
			this->posLButton->Name = L"posLButton";
			this->posLButton->Size = System::Drawing::Size(48, 23);
			this->posLButton->TabIndex = 54;
			this->posLButton->Text = L"posL";
			this->posLButton->UseVisualStyleBackColor = true;
			this->posLButton->Visible = false;
			this->posLButton->Click += gcnew System::EventHandler(this, &Form1::posLButton_Click);
			// 
			// brakeLButton
			// 
			this->brakeLButton->Enabled = false;
			this->brakeLButton->Location = System::Drawing::Point(198, 66);
			this->brakeLButton->Name = L"brakeLButton";
			this->brakeLButton->Size = System::Drawing::Size(48, 23);
			this->brakeLButton->TabIndex = 54;
			this->brakeLButton->Text = L"brakeL";
			this->brakeLButton->UseVisualStyleBackColor = true;
			this->brakeLButton->Visible = false;
			this->brakeLButton->Click += gcnew System::EventHandler(this, &Form1::brakeLButton_Click);
			// 
			// dutyCycleLabel
			// 
			this->dutyCycleLabel->AutoSize = true;
			this->dutyCycleLabel->Location = System::Drawing::Point(374, 269);
			this->dutyCycleLabel->Name = L"dutyCycleLabel";
			this->dutyCycleLabel->Size = System::Drawing::Size(75, 13);
			this->dutyCycleLabel->TabIndex = 53;
			this->dutyCycleLabel->Text = L"Duty Cycle [%]";
			this->dutyCycleLabel->Visible = false;
			// 
			// waitTimeLabel
			// 
			this->waitTimeLabel->AutoSize = true;
			this->waitTimeLabel->Location = System::Drawing::Point(358, 16);
			this->waitTimeLabel->Name = L"waitTimeLabel";
			this->waitTimeLabel->Size = System::Drawing::Size(76, 13);
			this->waitTimeLabel->TabIndex = 29;
			this->waitTimeLabel->Text = L"Wait time [ms]:";
			// 
			// dutyCycleTextBox
			// 
			this->dutyCycleTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->dutyCycleTextBox->ForeColor = System::Drawing::Color::Black;
			this->dutyCycleTextBox->Location = System::Drawing::Point(377, 288);
			this->dutyCycleTextBox->Name = L"dutyCycleTextBox";
			this->dutyCycleTextBox->Size = System::Drawing::Size(40, 18);
			this->dutyCycleTextBox->TabIndex = 52;
			this->dutyCycleTextBox->Visible = false;
			this->dutyCycleTextBox->TextChanged += gcnew System::EventHandler(this, &Form1::dutyCycleTextBox_TextChanged);
			// 
			// waitTimeTextBox
			// 
			this->waitTimeTextBox->Location = System::Drawing::Point(359, 36);
			this->waitTimeTextBox->Name = L"waitTimeTextBox";
			this->waitTimeTextBox->Size = System::Drawing::Size(134, 20);
			this->waitTimeTextBox->TabIndex = 28;
			this->waitTimeTextBox->TextChanged += gcnew System::EventHandler(this, &Form1::waitTimeTextBox_TextChanged);
			// 
			// detectedLampTypeGroupBox
			// 
			this->detectedLampTypeGroupBox->Controls->Add(this->detectedLampTypeLabel);
			this->detectedLampTypeGroupBox->Controls->Add(this->fogLightResultTextBox);
			this->detectedLampTypeGroupBox->Controls->Add(this->reverseLightResultTextBox);
			this->detectedLampTypeGroupBox->Controls->Add(this->brakeLightResultTextBox);
			this->detectedLampTypeGroupBox->Controls->Add(this->turnLLeftResultTextBox);
			this->detectedLampTypeGroupBox->Controls->Add(this->turnLRightResultTextBox);
			this->detectedLampTypeGroupBox->Controls->Add(this->posLLeftResultTextBox);
			this->detectedLampTypeGroupBox->Controls->Add(this->posLRightResultTextBox);
			this->detectedLampTypeGroupBox->Location = System::Drawing::Point(359, 82);
			this->detectedLampTypeGroupBox->Name = L"detectedLampTypeGroupBox";
			this->detectedLampTypeGroupBox->Size = System::Drawing::Size(135, 177);
			this->detectedLampTypeGroupBox->TabIndex = 27;
			this->detectedLampTypeGroupBox->TabStop = false;
			// 
			// detectedLampTypeLabel
			// 
			this->detectedLampTypeLabel->AutoSize = true;
			this->detectedLampTypeLabel->Location = System::Drawing::Point(12, 13);
			this->detectedLampTypeLabel->Name = L"detectedLampTypeLabel";
			this->detectedLampTypeLabel->Size = System::Drawing::Size(110, 13);
			this->detectedLampTypeLabel->TabIndex = 66;
			this->detectedLampTypeLabel->Text = L"Detected Lamp Type:";
			// 
			// fogLightResultTextBox
			// 
			this->fogLightResultTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->fogLightResultTextBox->ForeColor = System::Drawing::Color::Black;
			this->fogLightResultTextBox->Location = System::Drawing::Point(18, 151);
			this->fogLightResultTextBox->Name = L"fogLightResultTextBox";
			this->fogLightResultTextBox->Size = System::Drawing::Size(97, 18);
			this->fogLightResultTextBox->TabIndex = 65;
			this->fogLightResultTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->fogLightResultTextBox->Visible = false;
			// 
			// reverseLightResultTextBox
			// 
			this->reverseLightResultTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->reverseLightResultTextBox->ForeColor = System::Drawing::Color::Black;
			this->reverseLightResultTextBox->Location = System::Drawing::Point(18, 131);
			this->reverseLightResultTextBox->Name = L"reverseLightResultTextBox";
			this->reverseLightResultTextBox->Size = System::Drawing::Size(97, 18);
			this->reverseLightResultTextBox->TabIndex = 64;
			this->reverseLightResultTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->reverseLightResultTextBox->Visible = false;
			// 
			// brakeLightResultTextBox
			// 
			this->brakeLightResultTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->brakeLightResultTextBox->ForeColor = System::Drawing::Color::Black;
			this->brakeLightResultTextBox->Location = System::Drawing::Point(18, 111);
			this->brakeLightResultTextBox->Name = L"brakeLightResultTextBox";
			this->brakeLightResultTextBox->Size = System::Drawing::Size(97, 18);
			this->brakeLightResultTextBox->TabIndex = 63;
			this->brakeLightResultTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->brakeLightResultTextBox->Visible = false;
			// 
			// turnLLeftResultTextBox
			// 
			this->turnLLeftResultTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->turnLLeftResultTextBox->ForeColor = System::Drawing::Color::Black;
			this->turnLLeftResultTextBox->Location = System::Drawing::Point(18, 92);
			this->turnLLeftResultTextBox->Name = L"turnLLeftResultTextBox";
			this->turnLLeftResultTextBox->Size = System::Drawing::Size(97, 18);
			this->turnLLeftResultTextBox->TabIndex = 62;
			this->turnLLeftResultTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->turnLLeftResultTextBox->Visible = false;
			// 
			// turnLRightResultTextBox
			// 
			this->turnLRightResultTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->turnLRightResultTextBox->ForeColor = System::Drawing::Color::Black;
			this->turnLRightResultTextBox->Location = System::Drawing::Point(18, 72);
			this->turnLRightResultTextBox->Name = L"turnLRightResultTextBox";
			this->turnLRightResultTextBox->Size = System::Drawing::Size(97, 18);
			this->turnLRightResultTextBox->TabIndex = 61;
			this->turnLRightResultTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->turnLRightResultTextBox->Visible = false;
			// 
			// posLLeftResultTextBox
			// 
			this->posLLeftResultTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->posLLeftResultTextBox->ForeColor = System::Drawing::Color::Black;
			this->posLLeftResultTextBox->Location = System::Drawing::Point(18, 52);
			this->posLLeftResultTextBox->Name = L"posLLeftResultTextBox";
			this->posLLeftResultTextBox->Size = System::Drawing::Size(97, 18);
			this->posLLeftResultTextBox->TabIndex = 60;
			this->posLLeftResultTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->posLLeftResultTextBox->Visible = false;
			// 
			// posLRightResultTextBox
			// 
			this->posLRightResultTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->posLRightResultTextBox->ForeColor = System::Drawing::Color::Black;
			this->posLRightResultTextBox->Location = System::Drawing::Point(18, 32);
			this->posLRightResultTextBox->Name = L"posLRightResultTextBox";
			this->posLRightResultTextBox->Size = System::Drawing::Size(97, 18);
			this->posLRightResultTextBox->TabIndex = 59;
			this->posLRightResultTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->posLRightResultTextBox->Visible = false;
			// 
			// lampSelectionGroupBox
			// 
			this->lampSelectionGroupBox->Controls->Add(this->lampNoteLabel);
			this->lampSelectionGroupBox->Controls->Add(this->fogLightLampNoteTextBox);
			this->lampSelectionGroupBox->Controls->Add(this->reverseLightLampNoteTextBox);
			this->lampSelectionGroupBox->Controls->Add(this->brakeLightLampNoteTextBox);
			this->lampSelectionGroupBox->Controls->Add(this->turnLLeftLampNoteTextBox);
			this->lampSelectionGroupBox->Controls->Add(this->turnLRightLampNoteTextBox);
			this->lampSelectionGroupBox->Controls->Add(this->posLLeftLampNoteTextBox);
			this->lampSelectionGroupBox->Controls->Add(this->posLRightLampNoteTextBox);
			this->lampSelectionGroupBox->Controls->Add(this->lampNumberLabel);
			this->lampSelectionGroupBox->Controls->Add(this->fogLightLampNrTextBox);
			this->lampSelectionGroupBox->Controls->Add(this->reverseLightLampNrTextBox);
			this->lampSelectionGroupBox->Controls->Add(this->brakeLightLampNrTextBox);
			this->lampSelectionGroupBox->Controls->Add(this->turnLLeftLampNrTextBox);
			this->lampSelectionGroupBox->Controls->Add(this->turnLRightLampNrTextBox);
			this->lampSelectionGroupBox->Controls->Add(this->posLLeftLampNrTextBox);
			this->lampSelectionGroupBox->Controls->Add(this->posLRightLampNrTextBox);
			this->lampSelectionGroupBox->Controls->Add(this->lampTypeLabel);
			this->lampSelectionGroupBox->Controls->Add(this->fogLightFlashingCheckBox);
			this->lampSelectionGroupBox->Controls->Add(this->reverseLightFlashingCheckBox);
			this->lampSelectionGroupBox->Controls->Add(this->brakeLightFlashingCheckBox);
			this->lampSelectionGroupBox->Controls->Add(this->turnLLeftFlashingCheckBox);
			this->lampSelectionGroupBox->Controls->Add(this->turnLRightFlashingCheckBox);
			this->lampSelectionGroupBox->Controls->Add(this->posLRightFlashingCheckBox);
			this->lampSelectionGroupBox->Controls->Add(this->posLLeftFlashingCheckBox);
			this->lampSelectionGroupBox->Controls->Add(this->fogLightSelectCheckBox);
			this->lampSelectionGroupBox->Controls->Add(this->reverseLightSelectCheckBox);
			this->lampSelectionGroupBox->Controls->Add(this->brakeLightSelectCheckBox);
			this->lampSelectionGroupBox->Controls->Add(this->turnLLeftSelectCheckBox);
			this->lampSelectionGroupBox->Controls->Add(this->turnLRightSelectCheckBox);
			this->lampSelectionGroupBox->Controls->Add(this->posLRightSelectCheckBox);
			this->lampSelectionGroupBox->Controls->Add(this->posLLeftSelectCheckBox);
			this->lampSelectionGroupBox->Controls->Add(this->allLampsSelectCheckBox);
			this->lampSelectionGroupBox->Location = System::Drawing::Point(15, 82);
			this->lampSelectionGroupBox->Name = L"lampSelectionGroupBox";
			this->lampSelectionGroupBox->Size = System::Drawing::Size(325, 177);
			this->lampSelectionGroupBox->TabIndex = 24;
			this->lampSelectionGroupBox->TabStop = false;
			// 
			// lampNoteLabel
			// 
			this->lampNoteLabel->AutoSize = true;
			this->lampNoteLabel->Location = System::Drawing::Point(180, 13);
			this->lampNoteLabel->Name = L"lampNoteLabel";
			this->lampNoteLabel->Size = System::Drawing::Size(33, 13);
			this->lampNoteLabel->TabIndex = 51;
			this->lampNoteLabel->Text = L"Note:";
			this->lampNoteLabel->Visible = false;
			// 
			// fogLightLampNoteTextBox
			// 
			this->fogLightLampNoteTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->fogLightLampNoteTextBox->ForeColor = System::Drawing::Color::Black;
			this->fogLightLampNoteTextBox->Location = System::Drawing::Point(183, 151);
			this->fogLightLampNoteTextBox->Name = L"fogLightLampNoteTextBox";
			this->fogLightLampNoteTextBox->Size = System::Drawing::Size(40, 18);
			this->fogLightLampNoteTextBox->TabIndex = 50;
			this->fogLightLampNoteTextBox->Visible = false;
			// 
			// reverseLightLampNoteTextBox
			// 
			this->reverseLightLampNoteTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->reverseLightLampNoteTextBox->ForeColor = System::Drawing::Color::Black;
			this->reverseLightLampNoteTextBox->Location = System::Drawing::Point(183, 131);
			this->reverseLightLampNoteTextBox->Name = L"reverseLightLampNoteTextBox";
			this->reverseLightLampNoteTextBox->Size = System::Drawing::Size(40, 18);
			this->reverseLightLampNoteTextBox->TabIndex = 49;
			this->reverseLightLampNoteTextBox->Visible = false;
			// 
			// brakeLightLampNoteTextBox
			// 
			this->brakeLightLampNoteTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->brakeLightLampNoteTextBox->ForeColor = System::Drawing::Color::Black;
			this->brakeLightLampNoteTextBox->Location = System::Drawing::Point(183, 111);
			this->brakeLightLampNoteTextBox->Name = L"brakeLightLampNoteTextBox";
			this->brakeLightLampNoteTextBox->Size = System::Drawing::Size(40, 18);
			this->brakeLightLampNoteTextBox->TabIndex = 48;
			this->brakeLightLampNoteTextBox->Visible = false;
			// 
			// turnLLeftLampNoteTextBox
			// 
			this->turnLLeftLampNoteTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->turnLLeftLampNoteTextBox->ForeColor = System::Drawing::Color::Black;
			this->turnLLeftLampNoteTextBox->Location = System::Drawing::Point(183, 92);
			this->turnLLeftLampNoteTextBox->Name = L"turnLLeftLampNoteTextBox";
			this->turnLLeftLampNoteTextBox->Size = System::Drawing::Size(40, 18);
			this->turnLLeftLampNoteTextBox->TabIndex = 47;
			this->turnLLeftLampNoteTextBox->Visible = false;
			// 
			// turnLRightLampNoteTextBox
			// 
			this->turnLRightLampNoteTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->turnLRightLampNoteTextBox->ForeColor = System::Drawing::Color::Black;
			this->turnLRightLampNoteTextBox->Location = System::Drawing::Point(183, 72);
			this->turnLRightLampNoteTextBox->Name = L"turnLRightLampNoteTextBox";
			this->turnLRightLampNoteTextBox->Size = System::Drawing::Size(40, 18);
			this->turnLRightLampNoteTextBox->TabIndex = 46;
			this->turnLRightLampNoteTextBox->Visible = false;
			// 
			// posLLeftLampNoteTextBox
			// 
			this->posLLeftLampNoteTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->posLLeftLampNoteTextBox->ForeColor = System::Drawing::Color::Black;
			this->posLLeftLampNoteTextBox->Location = System::Drawing::Point(183, 52);
			this->posLLeftLampNoteTextBox->Name = L"posLLeftLampNoteTextBox";
			this->posLLeftLampNoteTextBox->Size = System::Drawing::Size(40, 18);
			this->posLLeftLampNoteTextBox->TabIndex = 45;
			this->posLLeftLampNoteTextBox->Visible = false;
			// 
			// posLRightLampNoteTextBox
			// 
			this->posLRightLampNoteTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->posLRightLampNoteTextBox->ForeColor = System::Drawing::Color::Black;
			this->posLRightLampNoteTextBox->Location = System::Drawing::Point(183, 32);
			this->posLRightLampNoteTextBox->Name = L"posLRightLampNoteTextBox";
			this->posLRightLampNoteTextBox->Size = System::Drawing::Size(40, 18);
			this->posLRightLampNoteTextBox->TabIndex = 44;
			this->posLRightLampNoteTextBox->Visible = false;
			// 
			// lampNumberLabel
			// 
			this->lampNumberLabel->AutoSize = true;
			this->lampNumberLabel->Location = System::Drawing::Point(111, 13);
			this->lampNumberLabel->Name = L"lampNumberLabel";
			this->lampNumberLabel->Size = System::Drawing::Size(46, 13);
			this->lampNumberLabel->TabIndex = 43;
			this->lampNumberLabel->Text = L"Lamp #:";
			this->lampNumberLabel->Visible = false;
			// 
			// fogLightLampNrTextBox
			// 
			this->fogLightLampNrTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->fogLightLampNrTextBox->ForeColor = System::Drawing::Color::Black;
			this->fogLightLampNrTextBox->Location = System::Drawing::Point(114, 151);
			this->fogLightLampNrTextBox->Name = L"fogLightLampNrTextBox";
			this->fogLightLampNrTextBox->Size = System::Drawing::Size(40, 18);
			this->fogLightLampNrTextBox->TabIndex = 42;
			this->fogLightLampNrTextBox->Visible = false;
			this->fogLightLampNrTextBox->TextChanged += gcnew System::EventHandler(this, &Form1::fogLightLampNrTextBox_TextChanged);
			// 
			// reverseLightLampNrTextBox
			// 
			this->reverseLightLampNrTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->reverseLightLampNrTextBox->ForeColor = System::Drawing::Color::Black;
			this->reverseLightLampNrTextBox->Location = System::Drawing::Point(114, 131);
			this->reverseLightLampNrTextBox->Name = L"reverseLightLampNrTextBox";
			this->reverseLightLampNrTextBox->Size = System::Drawing::Size(40, 18);
			this->reverseLightLampNrTextBox->TabIndex = 41;
			this->reverseLightLampNrTextBox->Visible = false;
			this->reverseLightLampNrTextBox->TextChanged += gcnew System::EventHandler(this, &Form1::reverseLightLampNrTextBox_TextChanged);
			// 
			// brakeLightLampNrTextBox
			// 
			this->brakeLightLampNrTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->brakeLightLampNrTextBox->ForeColor = System::Drawing::Color::Black;
			this->brakeLightLampNrTextBox->Location = System::Drawing::Point(114, 111);
			this->brakeLightLampNrTextBox->Name = L"brakeLightLampNrTextBox";
			this->brakeLightLampNrTextBox->Size = System::Drawing::Size(40, 18);
			this->brakeLightLampNrTextBox->TabIndex = 40;
			this->brakeLightLampNrTextBox->Visible = false;
			this->brakeLightLampNrTextBox->TextChanged += gcnew System::EventHandler(this, &Form1::brakeLightLampNrTextBox_TextChanged);
			// 
			// turnLLeftLampNrTextBox
			// 
			this->turnLLeftLampNrTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->turnLLeftLampNrTextBox->ForeColor = System::Drawing::Color::Black;
			this->turnLLeftLampNrTextBox->Location = System::Drawing::Point(114, 92);
			this->turnLLeftLampNrTextBox->Name = L"turnLLeftLampNrTextBox";
			this->turnLLeftLampNrTextBox->Size = System::Drawing::Size(40, 18);
			this->turnLLeftLampNrTextBox->TabIndex = 39;
			this->turnLLeftLampNrTextBox->Visible = false;
			this->turnLLeftLampNrTextBox->TextChanged += gcnew System::EventHandler(this, &Form1::turnLLeftLampNrTextBox_TextChanged);
			// 
			// turnLRightLampNrTextBox
			// 
			this->turnLRightLampNrTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->turnLRightLampNrTextBox->ForeColor = System::Drawing::Color::Black;
			this->turnLRightLampNrTextBox->Location = System::Drawing::Point(114, 72);
			this->turnLRightLampNrTextBox->Name = L"turnLRightLampNrTextBox";
			this->turnLRightLampNrTextBox->Size = System::Drawing::Size(40, 18);
			this->turnLRightLampNrTextBox->TabIndex = 38;
			this->turnLRightLampNrTextBox->Visible = false;
			this->turnLRightLampNrTextBox->TextChanged += gcnew System::EventHandler(this, &Form1::turnLRightLampNrTextBox_TextChanged);
			// 
			// posLLeftLampNrTextBox
			// 
			this->posLLeftLampNrTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->posLLeftLampNrTextBox->ForeColor = System::Drawing::Color::Black;
			this->posLLeftLampNrTextBox->Location = System::Drawing::Point(114, 52);
			this->posLLeftLampNrTextBox->Name = L"posLLeftLampNrTextBox";
			this->posLLeftLampNrTextBox->Size = System::Drawing::Size(40, 18);
			this->posLLeftLampNrTextBox->TabIndex = 37;
			this->posLLeftLampNrTextBox->Visible = false;
			this->posLLeftLampNrTextBox->TextChanged += gcnew System::EventHandler(this, &Form1::posLLeftLampNrTextBox_TextChanged);
			// 
			// posLRightLampNrTextBox
			// 
			this->posLRightLampNrTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->posLRightLampNrTextBox->ForeColor = System::Drawing::Color::Black;
			this->posLRightLampNrTextBox->Location = System::Drawing::Point(114, 32);
			this->posLRightLampNrTextBox->Name = L"posLRightLampNrTextBox";
			this->posLRightLampNrTextBox->Size = System::Drawing::Size(40, 18);
			this->posLRightLampNrTextBox->TabIndex = 36;
			this->posLRightLampNrTextBox->Visible = false;
			this->posLRightLampNrTextBox->TextChanged += gcnew System::EventHandler(this, &Form1::posLRightLampNrTextBox_TextChanged);
			// 
			// lampTypeLabel
			// 
			this->lampTypeLabel->AutoSize = true;
			this->lampTypeLabel->Location = System::Drawing::Point(250, 13);
			this->lampTypeLabel->Name = L"lampTypeLabel";
			this->lampTypeLabel->Size = System::Drawing::Size(63, 13);
			this->lampTypeLabel->TabIndex = 35;
			this->lampTypeLabel->Text = L"Lamp Type:";
			this->lampTypeLabel->Visible = false;
			// 
			// fogLightFlashingCheckBox
			// 
			this->fogLightFlashingCheckBox->AutoSize = true;
			this->fogLightFlashingCheckBox->Location = System::Drawing::Point(253, 151);
			this->fogLightFlashingCheckBox->Name = L"fogLightFlashingCheckBox";
			this->fogLightFlashingCheckBox->Size = System::Drawing::Size(68, 17);
			this->fogLightFlashingCheckBox->TabIndex = 34;
			this->fogLightFlashingCheckBox->Text = L"flashing\?";
			this->fogLightFlashingCheckBox->UseVisualStyleBackColor = true;
			this->fogLightFlashingCheckBox->Visible = false;
			this->fogLightFlashingCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::fogLightFlashingCheckBox_CheckedChanged);
			// 
			// reverseLightFlashingCheckBox
			// 
			this->reverseLightFlashingCheckBox->AutoSize = true;
			this->reverseLightFlashingCheckBox->Location = System::Drawing::Point(253, 131);
			this->reverseLightFlashingCheckBox->Name = L"reverseLightFlashingCheckBox";
			this->reverseLightFlashingCheckBox->Size = System::Drawing::Size(68, 17);
			this->reverseLightFlashingCheckBox->TabIndex = 33;
			this->reverseLightFlashingCheckBox->Text = L"flashing\?";
			this->reverseLightFlashingCheckBox->UseVisualStyleBackColor = true;
			this->reverseLightFlashingCheckBox->Visible = false;
			this->reverseLightFlashingCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::reverseLightFlashingCheckBox_CheckedChanged);
			// 
			// brakeLightFlashingCheckBox
			// 
			this->brakeLightFlashingCheckBox->AutoSize = true;
			this->brakeLightFlashingCheckBox->Location = System::Drawing::Point(253, 112);
			this->brakeLightFlashingCheckBox->Name = L"brakeLightFlashingCheckBox";
			this->brakeLightFlashingCheckBox->Size = System::Drawing::Size(68, 17);
			this->brakeLightFlashingCheckBox->TabIndex = 32;
			this->brakeLightFlashingCheckBox->Text = L"flashing\?";
			this->brakeLightFlashingCheckBox->UseVisualStyleBackColor = true;
			this->brakeLightFlashingCheckBox->Visible = false;
			this->brakeLightFlashingCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::brakeLightFlashingCheckBox_CheckedChanged);
			// 
			// turnLLeftFlashingCheckBox
			// 
			this->turnLLeftFlashingCheckBox->AutoSize = true;
			this->turnLLeftFlashingCheckBox->Location = System::Drawing::Point(253, 92);
			this->turnLLeftFlashingCheckBox->Name = L"turnLLeftFlashingCheckBox";
			this->turnLLeftFlashingCheckBox->Size = System::Drawing::Size(68, 17);
			this->turnLLeftFlashingCheckBox->TabIndex = 31;
			this->turnLLeftFlashingCheckBox->Text = L"flashing\?";
			this->turnLLeftFlashingCheckBox->UseVisualStyleBackColor = true;
			this->turnLLeftFlashingCheckBox->Visible = false;
			this->turnLLeftFlashingCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::turnLLeftFlashingCheckBox_CheckedChanged);
			// 
			// turnLRightFlashingCheckBox
			// 
			this->turnLRightFlashingCheckBox->AutoSize = true;
			this->turnLRightFlashingCheckBox->Location = System::Drawing::Point(253, 72);
			this->turnLRightFlashingCheckBox->Name = L"turnLRightFlashingCheckBox";
			this->turnLRightFlashingCheckBox->Size = System::Drawing::Size(68, 17);
			this->turnLRightFlashingCheckBox->TabIndex = 30;
			this->turnLRightFlashingCheckBox->Text = L"flashing\?";
			this->turnLRightFlashingCheckBox->UseVisualStyleBackColor = true;
			this->turnLRightFlashingCheckBox->Visible = false;
			this->turnLRightFlashingCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::turnLRightFlashingCheckBox_CheckedChanged);
			// 
			// posLRightFlashingCheckBox
			// 
			this->posLRightFlashingCheckBox->AutoSize = true;
			this->posLRightFlashingCheckBox->Location = System::Drawing::Point(253, 32);
			this->posLRightFlashingCheckBox->Name = L"posLRightFlashingCheckBox";
			this->posLRightFlashingCheckBox->Size = System::Drawing::Size(68, 17);
			this->posLRightFlashingCheckBox->TabIndex = 29;
			this->posLRightFlashingCheckBox->Text = L"flashing\?";
			this->posLRightFlashingCheckBox->UseVisualStyleBackColor = true;
			this->posLRightFlashingCheckBox->Visible = false;
			this->posLRightFlashingCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::posLRightFlashingCheckBox_CheckedChanged);
			// 
			// posLLeftFlashingCheckBox
			// 
			this->posLLeftFlashingCheckBox->AutoSize = true;
			this->posLLeftFlashingCheckBox->Location = System::Drawing::Point(253, 52);
			this->posLLeftFlashingCheckBox->Name = L"posLLeftFlashingCheckBox";
			this->posLLeftFlashingCheckBox->Size = System::Drawing::Size(68, 17);
			this->posLLeftFlashingCheckBox->TabIndex = 28;
			this->posLLeftFlashingCheckBox->Text = L"flashing\?";
			this->posLLeftFlashingCheckBox->UseVisualStyleBackColor = true;
			this->posLLeftFlashingCheckBox->Visible = false;
			this->posLLeftFlashingCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::posLLeftFlashingCheckBox_CheckedChanged);
			// 
			// fogLightSelectCheckBox
			// 
			this->fogLightSelectCheckBox->AutoSize = true;
			this->fogLightSelectCheckBox->Location = System::Drawing::Point(9, 151);
			this->fogLightSelectCheckBox->Name = L"fogLightSelectCheckBox";
			this->fogLightSelectCheckBox->Size = System::Drawing::Size(64, 17);
			this->fogLightSelectCheckBox->TabIndex = 27;
			this->fogLightSelectCheckBox->Text = L"fogLight";
			this->fogLightSelectCheckBox->UseVisualStyleBackColor = true;
			this->fogLightSelectCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::fogLightSelectCheckBox_CheckedChanged);
			// 
			// reverseLightSelectCheckBox
			// 
			this->reverseLightSelectCheckBox->AutoSize = true;
			this->reverseLightSelectCheckBox->Location = System::Drawing::Point(9, 131);
			this->reverseLightSelectCheckBox->Name = L"reverseLightSelectCheckBox";
			this->reverseLightSelectCheckBox->Size = System::Drawing::Size(84, 17);
			this->reverseLightSelectCheckBox->TabIndex = 26;
			this->reverseLightSelectCheckBox->Text = L"reverseLight";
			this->reverseLightSelectCheckBox->UseVisualStyleBackColor = true;
			this->reverseLightSelectCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::reverseLightSelectCheckBox_CheckedChanged);
			// 
			// brakeLightSelectCheckBox
			// 
			this->brakeLightSelectCheckBox->AutoSize = true;
			this->brakeLightSelectCheckBox->Location = System::Drawing::Point(9, 112);
			this->brakeLightSelectCheckBox->Name = L"brakeLightSelectCheckBox";
			this->brakeLightSelectCheckBox->Size = System::Drawing::Size(76, 17);
			this->brakeLightSelectCheckBox->TabIndex = 25;
			this->brakeLightSelectCheckBox->Text = L"brakeLight";
			this->brakeLightSelectCheckBox->UseVisualStyleBackColor = true;
			this->brakeLightSelectCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::brakeLightSelectCheckBox_CheckedChanged);
			// 
			// turnLLeftSelectCheckBox
			// 
			this->turnLLeftSelectCheckBox->AutoSize = true;
			this->turnLLeftSelectCheckBox->Location = System::Drawing::Point(9, 92);
			this->turnLLeftSelectCheckBox->Name = L"turnLLeftSelectCheckBox";
			this->turnLLeftSelectCheckBox->Size = System::Drawing::Size(68, 17);
			this->turnLLeftSelectCheckBox->TabIndex = 4;
			this->turnLLeftSelectCheckBox->Text = L"turnLLeft";
			this->turnLLeftSelectCheckBox->UseVisualStyleBackColor = true;
			this->turnLLeftSelectCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::turnLLeftSelectCheckBox_CheckedChanged);
			// 
			// turnLRightSelectCheckBox
			// 
			this->turnLRightSelectCheckBox->AutoSize = true;
			this->turnLRightSelectCheckBox->Location = System::Drawing::Point(9, 72);
			this->turnLRightSelectCheckBox->Name = L"turnLRightSelectCheckBox";
			this->turnLRightSelectCheckBox->Size = System::Drawing::Size(75, 17);
			this->turnLRightSelectCheckBox->TabIndex = 3;
			this->turnLRightSelectCheckBox->Text = L"turnLRight";
			this->turnLRightSelectCheckBox->UseVisualStyleBackColor = true;
			this->turnLRightSelectCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::turnLRightSelectCheckBox_CheckedChanged);
			// 
			// posLRightSelectCheckBox
			// 
			this->posLRightSelectCheckBox->AutoSize = true;
			this->posLRightSelectCheckBox->Location = System::Drawing::Point(9, 32);
			this->posLRightSelectCheckBox->Name = L"posLRightSelectCheckBox";
			this->posLRightSelectCheckBox->Size = System::Drawing::Size(74, 17);
			this->posLRightSelectCheckBox->TabIndex = 2;
			this->posLRightSelectCheckBox->Text = L"posLRight";
			this->posLRightSelectCheckBox->UseVisualStyleBackColor = true;
			this->posLRightSelectCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::posLRightSelectCheckBox_CheckedChanged);
			// 
			// posLLeftSelectCheckBox
			// 
			this->posLLeftSelectCheckBox->AutoSize = true;
			this->posLLeftSelectCheckBox->Location = System::Drawing::Point(9, 52);
			this->posLLeftSelectCheckBox->Name = L"posLLeftSelectCheckBox";
			this->posLLeftSelectCheckBox->Size = System::Drawing::Size(67, 17);
			this->posLLeftSelectCheckBox->TabIndex = 1;
			this->posLLeftSelectCheckBox->Text = L"posLLeft";
			this->posLLeftSelectCheckBox->UseVisualStyleBackColor = true;
			this->posLLeftSelectCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::posLLeftSelectCheckBox_CheckedChanged);
			// 
			// allLampsSelectCheckBox
			// 
			this->allLampsSelectCheckBox->AutoSize = true;
			this->allLampsSelectCheckBox->Location = System::Drawing::Point(9, 12);
			this->allLampsSelectCheckBox->Name = L"allLampsSelectCheckBox";
			this->allLampsSelectCheckBox->Size = System::Drawing::Size(36, 17);
			this->allLampsSelectCheckBox->TabIndex = 0;
			this->allLampsSelectCheckBox->Text = L"all";
			this->allLampsSelectCheckBox->UseVisualStyleBackColor = true;
			this->allLampsSelectCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::allLampsSelectCheckBox_CheckedChanged);
			// 
			// abortClassificationButton
			// 
			this->abortClassificationButton->Location = System::Drawing::Point(162, 274);
			this->abortClassificationButton->Name = L"abortClassificationButton";
			this->abortClassificationButton->Size = System::Drawing::Size(116, 28);
			this->abortClassificationButton->TabIndex = 23;
			this->abortClassificationButton->Text = L"Abort classification";
			this->abortClassificationButton->UseVisualStyleBackColor = true;
			this->abortClassificationButton->Click += gcnew System::EventHandler(this, &Form1::abortClassificationButton_Click);
			// 
			// startClassificationButton
			// 
			this->startClassificationButton->Location = System::Drawing::Point(12, 274);
			this->startClassificationButton->Name = L"startClassificationButton";
			this->startClassificationButton->Size = System::Drawing::Size(116, 28);
			this->startClassificationButton->TabIndex = 22;
			this->startClassificationButton->Text = L"Start classification";
			this->startClassificationButton->UseVisualStyleBackColor = true;
			this->startClassificationButton->Click += gcnew System::EventHandler(this, &Form1::startClassificationButton_Click);
			// 
			// informationModeComboBox
			// 
			this->informationModeComboBox->FormattingEnabled = true;
			this->informationModeComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"standard mode", L"detail mode", 
				L"sample mode"});
			this->informationModeComboBox->Location = System::Drawing::Point(232, 34);
			this->informationModeComboBox->Name = L"informationModeComboBox";
			this->informationModeComboBox->Size = System::Drawing::Size(108, 21);
			this->informationModeComboBox->TabIndex = 21;
			// 
			// informationModeLabel
			// 
			this->informationModeLabel->AutoSize = true;
			this->informationModeLabel->Location = System::Drawing::Point(229, 13);
			this->informationModeLabel->Name = L"informationModeLabel";
			this->informationModeLabel->Size = System::Drawing::Size(57, 13);
			this->informationModeLabel->TabIndex = 20;
			this->informationModeLabel->Text = L"Info mode:";
			// 
			// lampChannelLabel
			// 
			this->lampChannelLabel->AutoSize = true;
			this->lampChannelLabel->Location = System::Drawing::Point(12, 66);
			this->lampChannelLabel->Name = L"lampChannelLabel";
			this->lampChannelLabel->Size = System::Drawing::Size(167, 13);
			this->lampChannelLabel->TabIndex = 18;
			this->lampChannelLabel->Text = L"Used lamp channels and devices:";
			// 
			// usedCharacteristicComboBox
			// 
			this->usedCharacteristicComboBox->FormattingEnabled = true;
			this->usedCharacteristicComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(11) {L"none", L"all characteristics", 
				L"slope analysis", L"integration & peak", L"mean & devMean", L"pwm mode", L"oldLampDetection", L"oldOpenLoadTestOnState", L"oldOpenLoadTestOffState", 
				L"matlabClassificationDefensive", L"matlabClassificationOffensive"});
			this->usedCharacteristicComboBox->Location = System::Drawing::Point(12, 35);
			this->usedCharacteristicComboBox->Name = L"usedCharacteristicComboBox";
			this->usedCharacteristicComboBox->Size = System::Drawing::Size(149, 21);
			this->usedCharacteristicComboBox->TabIndex = 17;
			this->usedCharacteristicComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::usedCharacteristicComboBox_SelectedIndexChanged);
			// 
			// usedCharacterisicLabel
			// 
			this->usedCharacterisicLabel->AutoSize = true;
			this->usedCharacterisicLabel->Location = System::Drawing::Point(9, 14);
			this->usedCharacterisicLabel->Name = L"usedCharacterisicLabel";
			this->usedCharacterisicLabel->Size = System::Drawing::Size(74, 13);
			this->usedCharacterisicLabel->TabIndex = 16;
			this->usedCharacterisicLabel->Text = L"Characteristic:";
			// 
			// numberIterationTextBox
			// 
			this->numberIterationTextBox->Location = System::Drawing::Point(169, 35);
			this->numberIterationTextBox->Name = L"numberIterationTextBox";
			this->numberIterationTextBox->Size = System::Drawing::Size(55, 20);
			this->numberIterationTextBox->TabIndex = 15;
			this->numberIterationTextBox->TextChanged += gcnew System::EventHandler(this, &Form1::numberIterationTextBox_TextChanged);
			// 
			// selectTestTypeLabel
			// 
			this->selectTestTypeLabel->AutoSize = true;
			this->selectTestTypeLabel->Location = System::Drawing::Point(166, 14);
			this->selectTestTypeLabel->Name = L"selectTestTypeLabel";
			this->selectTestTypeLabel->Size = System::Drawing::Size(63, 13);
			this->selectTestTypeLabel->TabIndex = 14;
			this->selectTestTypeLabel->Text = L"# Iterations:";
			// 
			// clearLogButton
			// 
			this->clearLogButton->Location = System::Drawing::Point(451, 41);
			this->clearLogButton->Name = L"clearLogButton";
			this->clearLogButton->Size = System::Drawing::Size(60, 21);
			this->clearLogButton->TabIndex = 13;
			this->clearLogButton->Text = L"clear log";
			this->clearLogButton->UseVisualStyleBackColor = true;
			this->clearLogButton->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// clearResultsButton
			// 
			this->clearResultsButton->Location = System::Drawing::Point(518, 71);
			this->clearResultsButton->Name = L"clearResultsButton";
			this->clearResultsButton->Size = System::Drawing::Size(20, 218);
			this->clearResultsButton->TabIndex = 14;
			this->clearResultsButton->Text = L"clear    resul ts";
			this->clearResultsButton->UseVisualStyleBackColor = true;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::GradientInactiveCaption;
			this->ClientSize = System::Drawing::Size(540, 675);
			this->ContextMenuStrip = this->contextMenuStrip1;
			this->Controls->Add(this->clearResultsButton);
			this->Controls->Add(this->clearLogButton);
			this->Controls->Add(this->classificationSetupGroupBox);
			this->Controls->Add(this->disconnectSerialButton);
			this->Controls->Add(this->commLogBox);
			this->Controls->Add(this->selectPortLabel);
			this->Controls->Add(this->selectPortBox);
			this->Controls->Add(this->commDataLabel);
			this->Controls->Add(this->speedGroupBox);
			this->Controls->Add(this->serialConnectionLabel);
			this->Controls->Add(this->connectSerialButton);
			this->Controls->Add(this->menuBar);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MainMenuStrip = this->menuBar;
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Text = L"Classification Client Control Center";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->speedGroupBox->ResumeLayout(false);
			this->speedGroupBox->PerformLayout();
			this->menuBar->ResumeLayout(false);
			this->menuBar->PerformLayout();
			this->contextMenuStrip1->ResumeLayout(false);
			this->classificationSetupGroupBox->ResumeLayout(false);
			this->classificationSetupGroupBox->PerformLayout();
			this->classificationSetupInnerGroupBox->ResumeLayout(false);
			this->classificationSetupInnerGroupBox->PerformLayout();
			this->detectedLampTypeGroupBox->ResumeLayout(false);
			this->detectedLampTypeGroupBox->PerformLayout();
			this->lampSelectionGroupBox->ResumeLayout(false);
			this->lampSelectionGroupBox->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

	//***********************************************************************************************************
	// event handlers:
	//***********************************************************************************************************
#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
				
				 //show map dialog
				//mapDialog->Show(this);			
			 }


	private: System::Void selectPortBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				
				 if (selectPortBox->SelectedItem=="COM1")
				 {
					 serialPort1->PortName = L"COM1";
					 commLogBox->Text += "[USER]: selected COM1.\r\n";
				 }
				 if (selectPortBox->SelectedItem=="COM2")
				 {
					 serialPort1->PortName = L"COM2";
					 commLogBox->Text += "[USER]: selected COM2.\r\n";
				 }
				 if (selectPortBox->SelectedItem=="COM3")
				 {
					 serialPort1->PortName = L"COM3";
					 commLogBox->Text += "[USER]: selected COM3.\r\n";
				 }
				 if (selectPortBox->SelectedItem=="COM4")
				 {
					 serialPort1->PortName = L"COM4";
					 commLogBox->Text += "[USER]: selected COM4.\r\n";
				 }
				 if (selectPortBox->SelectedItem=="COM5")
				 {
					 serialPort1->PortName = L"COM5";
					 commLogBox->Text += "[USER]: selected COM5.\r\n";
				 }
				 if (selectPortBox->SelectedItem=="COM6")
				 {
					 serialPort1->PortName = L"COM6";
					 commLogBox->Text += "[USER]: selected COM6.\r\n";
				 }
				 if (selectPortBox->SelectedItem=="COM7")
				 {
					 serialPort1->PortName = L"COM7";
					 commLogBox->Text += "[USER]: selected COM7.\r\n";
				 }
			 }


	private: System::Void radioButton1_Click(System::Object^  sender, System::EventArgs^  e) {
				 
				if (sender==speed57600Button)
				commLogBox->AppendText("[USER]: selected 57600 baud." + Environment::NewLine);
				else if (sender==speed115200Button)
				commLogBox->AppendText("[USER]: selected 115200 baud." + Environment::NewLine);
			 }

	private: System::Void aboutMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				
				MessageBox::Show(L"V0.1 | OhlbergA | 8th June 2012",L"Version");
			 }

	private: System::Void exitMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				
				Application::Exit();
			 }


	//Serial port: data received
	private: System::Void serialPort1_DataReceived(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) {
				
				 //call delegate to transfer read data to textBox
				try
				{
					Invoke(updateComLogBoxDelegate);
				}
				catch (Exception ^)
				{
					
				}
			 }

	//Serial port: error received	
	private: System::Void serialPort1_ErrorReceived(System::Object^  sender, System::IO::Ports::SerialErrorReceivedEventArgs^  e) {
				 
				 //commLogBox->AppendText("[SYSTEM]: Error data received"+ Environment::NewLine);
			 }



	private: System::Void DisconnectButton_Click_1(System::Object^  sender, System::EventArgs^  e) {
				
				 serialPort1->Close();
				if (serialPort1->IsOpen!=true)
					commLogBox->AppendText("[INFO]: Serial port is closed." + Environment::NewLine);
					
			 }
	
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
				
				 commLogBox->Clear();
			 }




private: System::Void connectSerialButton_Click(System::Object^  sender, System::EventArgs^  e) {

				if (serialPort1->IsOpen==false)
				{
					serialPort1->ReadBufferSize=1048576;
					serialPort1->ReceivedBytesThreshold = 6;
				}
				//commLogBox->AppendText("[INFO]: serial input buffer size: " + serialPort1->ReadBufferSize + Environment::NewLine);				try
				
				try
				{
					serialPort1->Open();
				}
				catch (IOException ^)//!!does not work correctly.
				{
					commLogBox->AppendText("[ERROR]: Serial port is in an invalid state." + Environment::NewLine);
				}
				catch (UnauthorizedAccessException ^)
				{
					commLogBox->AppendText("[ERROR]: Access to serial port is denied." + Environment::NewLine);
				}
				catch (InvalidOperationException ^)
				{
					commLogBox->AppendText("[INFO]: Serial port is already open." + Environment::NewLine);
				}
				
				if (serialPort1->IsOpen==true)
				{
					serialPort1->DiscardInBuffer();    // clear receive buffer
					serialPort1->DiscardOutBuffer();   // clear transmit buffer
					commLogBox->AppendText("[INFO]: Serial port is open." + Environment::NewLine);
				}


				//commLogBox->AppendText("bytes to read:" + serialPort1->BytesToRead + Environment::NewLine);
				//commLogBox->AppendText("read buffer size:" + serialPort1->ReadBufferSize + Environment::NewLine);
				//commLogBox->AppendText("receivedBytesThres:" + serialPort1->ReceivedBytesThreshold + Environment::NewLine);

		 }

private: System::Void posLRightSelectCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

				if (posLRightSelectCheckBox->Checked == true)
				{
					posLRightFlashingCheckBox->Visible = true;	
					posLRightLampNrTextBox->Visible = true;
					lampNumberLabel->Visible = true;
					lampTypeLabel->Visible = true;
					lampNoteLabel->Visible = true;
					posLRightLampNoteTextBox->Visible = true;
					posLRightResultTextBox->Visible = true;
				}
				else
				{
					posLRightFlashingCheckBox->Visible = false;
					posLRightLampNrTextBox->Visible = false;
					posLRightLampNoteTextBox->Visible = false;
					posLRightResultTextBox->Visible = false;
					if (posLLeftSelectCheckBox->Checked == false &&
						turnLRightSelectCheckBox->Checked == false &&
						turnLLeftSelectCheckBox->Checked == false &&
						brakeLightSelectCheckBox->Checked == false &&
						reverseLightSelectCheckBox->Checked == false &&
						fogLightSelectCheckBox->Checked == false)
					{
						lampNumberLabel->Visible = false;
						lampTypeLabel->Visible = false;
						lampNoteLabel->Visible = false;
					}
				}
		 }
private: System::Void posLLeftSelectCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				
				if (posLLeftSelectCheckBox->Checked == true)
				{
					posLLeftFlashingCheckBox->Visible = true;
					posLLeftLampNrTextBox->Visible = true;
					lampNumberLabel->Visible = true;
					lampTypeLabel->Visible = true;
					lampNoteLabel->Visible = true;
					posLLeftLampNoteTextBox->Visible = true;
					posLLeftResultTextBox->Visible = true;
				}
				else
				{
					posLLeftFlashingCheckBox->Visible = false;
					posLLeftLampNrTextBox->Visible = false;
					posLLeftLampNoteTextBox->Visible = false;
					posLLeftResultTextBox->Visible = false;
					if (posLRightSelectCheckBox->Checked == false &&
						turnLRightSelectCheckBox->Checked == false &&
						turnLLeftSelectCheckBox->Checked == false &&
						brakeLightSelectCheckBox->Checked == false &&
						reverseLightSelectCheckBox->Checked == false &&
						fogLightSelectCheckBox->Checked == false)
					{
						lampNumberLabel->Visible = false;
						lampTypeLabel->Visible = false;
						lampNoteLabel->Visible = false;
					}
				}
		 }
private: System::Void turnLRightSelectCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				
				if (turnLRightSelectCheckBox->Checked == true)
				{
					turnLRightFlashingCheckBox->Visible = true;	
					turnLRightLampNrTextBox->Visible = true;
					lampNumberLabel->Visible = true;
					lampTypeLabel->Visible = true;
					lampNoteLabel->Visible = true;
					turnLRightLampNoteTextBox->Visible = true;
					turnLRightResultTextBox->Visible = true;
				}
				else
				{
					turnLRightFlashingCheckBox->Visible = false;
					turnLRightLampNrTextBox->Visible = false;
					turnLRightLampNoteTextBox->Visible = false;
					turnLRightResultTextBox->Visible = false;
					if (posLRightSelectCheckBox->Checked == false &&
						posLLeftSelectCheckBox->Checked == false &&
						turnLLeftSelectCheckBox->Checked == false &&
						brakeLightSelectCheckBox->Checked == false &&
						reverseLightSelectCheckBox->Checked == false &&
						fogLightSelectCheckBox->Checked == false)
					{
						lampNumberLabel->Visible = false;
						lampTypeLabel->Visible = false;
						lampNoteLabel->Visible = false;
					}
				}
		 }
private: System::Void turnLLeftSelectCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

				if (turnLLeftSelectCheckBox->Checked == true)
				{
					turnLLeftFlashingCheckBox->Visible = true;
					turnLLeftLampNrTextBox->Visible = true;
					lampNumberLabel->Visible = true;
					lampTypeLabel->Visible = true;
					lampNoteLabel->Visible = true;
					turnLLeftLampNoteTextBox->Visible = true;
					turnLLeftResultTextBox->Visible = true;
				}
				else
				{
					turnLLeftFlashingCheckBox->Visible = false;
					turnLLeftLampNrTextBox->Visible = false;
					turnLLeftLampNoteTextBox->Visible = false;
					turnLLeftResultTextBox->Visible = false;
					if (posLRightSelectCheckBox->Checked == false &&
						posLLeftSelectCheckBox->Checked == false &&
						turnLRightSelectCheckBox->Checked == false &&
						brakeLightSelectCheckBox->Checked == false &&
						reverseLightSelectCheckBox->Checked == false &&
						fogLightSelectCheckBox->Checked == false)
					{
						lampNumberLabel->Visible = false;
						lampTypeLabel->Visible = false;
						lampNoteLabel->Visible = false;
					}
				}
		 }
private: System::Void brakeLightSelectCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

				if (brakeLightSelectCheckBox->Checked == true)
				{
					brakeLightFlashingCheckBox->Visible = true;	
					brakeLightLampNrTextBox->Visible = true;
					lampNumberLabel->Visible = true;
					lampTypeLabel->Visible = true;
					lampNoteLabel->Visible = true;
					brakeLightLampNoteTextBox->Visible = true;
					brakeLightResultTextBox->Visible = true;
				}
				else
				{
					brakeLightFlashingCheckBox->Visible = false;
					brakeLightLampNrTextBox->Visible = false;
					brakeLightLampNoteTextBox->Visible = false;
					brakeLightResultTextBox->Visible = false;
					if (posLRightSelectCheckBox->Checked == false &&
						posLLeftSelectCheckBox->Checked == false &&
						turnLRightSelectCheckBox->Checked == false &&
						turnLLeftSelectCheckBox->Checked == false &&
						reverseLightSelectCheckBox->Checked == false &&
						fogLightSelectCheckBox->Checked == false)
					{
						lampNumberLabel->Visible = false;
						lampTypeLabel->Visible = false;
						lampNoteLabel->Visible = false;
					}
				}
		 }
private: System::Void reverseLightSelectCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

				if (reverseLightSelectCheckBox->Checked == true)
				{
					reverseLightFlashingCheckBox->Visible = true;	
					reverseLightLampNrTextBox->Visible = true;
					lampNumberLabel->Visible = true;
					lampTypeLabel->Visible = true;
					lampNoteLabel->Visible = true;
					reverseLightLampNoteTextBox->Visible = true;
					reverseLightResultTextBox->Visible = true;
				}
				else
				{
					reverseLightFlashingCheckBox->Visible = false;
					reverseLightLampNrTextBox->Visible = false;
					reverseLightLampNoteTextBox->Visible = false;
					reverseLightResultTextBox->Visible = false;
					if (posLRightSelectCheckBox->Checked == false &&
						posLLeftSelectCheckBox->Checked == false &&
						turnLRightSelectCheckBox->Checked == false &&
						turnLLeftSelectCheckBox->Checked == false &&
						brakeLightSelectCheckBox->Checked == false &&
						fogLightSelectCheckBox->Checked == false)
					{
						lampNumberLabel->Visible = false;
						lampTypeLabel->Visible = false;
						lampNoteLabel->Visible = false;
					}
				}
		 }
private: System::Void fogLightSelectCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

				if (fogLightSelectCheckBox->Checked == true)
				{
					fogLightFlashingCheckBox->Visible = true;
					fogLightLampNrTextBox->Visible = true;
					lampNumberLabel->Visible = true;
					lampTypeLabel->Visible = true;
					lampNoteLabel->Visible = true;
					fogLightLampNoteTextBox->Visible = true;
					fogLightResultTextBox->Visible = true;
				}
				else
				{
					fogLightFlashingCheckBox->Visible = false;
					fogLightLampNrTextBox->Visible = false;
					fogLightLampNoteTextBox->Visible = false;
					fogLightResultTextBox->Visible = false;
					if (posLRightSelectCheckBox->Checked == false &&
						posLLeftSelectCheckBox->Checked == false &&
						turnLRightSelectCheckBox->Checked == false &&
						turnLLeftSelectCheckBox->Checked == false &&
						brakeLightSelectCheckBox->Checked == false &&
						reverseLightSelectCheckBox->Checked == false)
					{
						lampNumberLabel->Visible = false;
						lampTypeLabel->Visible = false;
						lampNoteLabel->Visible = false;
					}
				}
		 }
private: System::Void allLampsSelectCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

				if (allLampsSelectCheckBox->Checked == true)
				{
					posLRightSelectCheckBox->Checked = true;	
					turnLRightSelectCheckBox->Checked = true;	
					turnLLeftSelectCheckBox->Checked = true;	
					brakeLightSelectCheckBox->Checked = true;	
					if (fogLightSelectCheckBox->Enabled == true)
					{
						fogLightSelectCheckBox->Checked = true;		
					}
					if (posLLeftSelectCheckBox->Enabled == true)
					{
						posLLeftSelectCheckBox->Checked = true;
					}
					if (reverseLightSelectCheckBox->Enabled == true)
					{
						reverseLightSelectCheckBox->Checked = true;
					}
				}
				else
				{
					posLRightSelectCheckBox->Checked = false;
					posLLeftSelectCheckBox->Checked = false;
					turnLRightSelectCheckBox->Checked = false;
					turnLLeftSelectCheckBox->Checked = false;
					brakeLightSelectCheckBox->Checked = false;
					reverseLightSelectCheckBox->Checked = false;
					fogLightSelectCheckBox->Checked = false;
				}
		 }

private: System::Void numberIterationTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 	
				//check user input for correctness
				try
				{
					if (numberIterationTextBox->Text != "")
					{
						numberOfIterations = Convert::ToInt32(numberIterationTextBox->Text);
					}
				}
				catch (Exception^)
				{
					commLogBox->AppendText("[INFO]: wrong value for # Iterations." + Environment::NewLine);
					commLogBox->AppendText("[INFO]: must be a number." + Environment::NewLine);	
					commLogBox->AppendText("[INFO]: '0' for continuous run." + Environment::NewLine);
					numberIterationTextBox->Text = "1";
				}
		 }

private: System::Void startClassificationButton_Click(System::Object^  sender, System::EventArgs^  e) {
				
			    if (serialPort1->IsOpen==false)
				{
					commLogBox->AppendText("[INFO]: please open serial port first. " + Environment::NewLine);
				}
				else
				{
					//read user selection
					//selectedCharacteristic = usedCharacteristicComboBox->SelectedValue;
					selectedCharacteristicNumber = Convert::ToInt32(usedCharacteristicComboBox->SelectedIndex);


					//convert selected characteristic index into ecu characteristic number
					if (selectedCharacteristicNumber == 1)  //all characteristics
					{
						//all lamp channels are to be classified
						selectedCharacteristicNumber = 200;
					}
					else if (selectedCharacteristicNumber == 2)
					{
						//slope analysis
						selectedCharacteristicNumber = 1;
					}
					else if (selectedCharacteristicNumber == 3)
					{
						//integral and peak analysis
						selectedCharacteristicNumber = 2;
					}
					else if (selectedCharacteristicNumber == 4)
					{
						//mean and deviation mean analysis
						selectedCharacteristicNumber = 3;
					}
					else if (selectedCharacteristicNumber == 5)
					{
						//pwm mode
						selectedCharacteristicNumber = 201;
					}
					else if (selectedCharacteristicNumber == 6)
					{
						//old characteristic
						selectedCharacteristicNumber = 202;
					}
					else if (selectedCharacteristicNumber == 7)
					{
						//old open load , on state
						selectedCharacteristicNumber = 203;
					}
					else if (selectedCharacteristicNumber == 8)
					{
						//old open load , off state
						selectedCharacteristicNumber = 204;
					}
					else if (selectedCharacteristicNumber == 9)
					{
						//matlab classification, defensive
						selectedCharacteristicNumber = 205;
					}
					else if (selectedCharacteristicNumber == 10)
					{
						//matlab classification, offensive
						selectedCharacteristicNumber = 206;
					}
					
					
					
					//initialize toellner power supply control module
					toeControlDialog->initToeModule();
	
					//selectedInformationMode = informationModeComboBox->SelectedValue;
					selectedInformationModeNumber = informationModeComboBox->SelectedIndex + 1;
					

					//clear the incomingDataBuffer and the buffer index
					incomingDataBuffer->Clear(incomingDataBuffer, 0, incomingDataBuffer->Length);
					incomingDataBufferIndex=0;

					//read selected lamp channels and update work list
					lampChannelWorkList[POSLRIGHT]    = posLRightSelectCheckBox->Checked;
					lampChannelWorkList[POSLLEFT]     = posLLeftSelectCheckBox->Checked;
					lampChannelWorkList[TURNLRIGHT]   = turnLRightSelectCheckBox->Checked;
					lampChannelWorkList[TURNLLEFT]    = turnLLeftSelectCheckBox->Checked;
					lampChannelWorkList[BRAKELIGHT]   = brakeLightSelectCheckBox->Checked;
					lampChannelWorkList[REVERSELIGHT] = reverseLightSelectCheckBox->Checked;
					lampChannelWorkList[FOGLIGHT]     = fogLightSelectCheckBox->Checked;			

					//read lamp numbers and update lookup table
					lampNumberLookUp[POSLRIGHT] = posLRightLampNrTextBox->Text;
					lampNumberLookUp[POSLLEFT] = posLLeftLampNrTextBox->Text;
					lampNumberLookUp[TURNLRIGHT] = turnLRightLampNrTextBox->Text;
					lampNumberLookUp[TURNLLEFT] = turnLLeftLampNrTextBox->Text;
					lampNumberLookUp[BRAKELIGHT] = brakeLightLampNrTextBox->Text;
					lampNumberLookUp[REVERSELIGHT] = reverseLightLampNrTextBox->Text;
					lampNumberLookUp[FOGLIGHT] = fogLightLampNrTextBox->Text;
			

					//read lamp notes and update lookup table
					lampNoteLookUp[POSLRIGHT] = posLRightLampNoteTextBox->Text;
					lampNoteLookUp[POSLLEFT] = posLLeftLampNoteTextBox->Text;
					lampNoteLookUp[TURNLRIGHT] = turnLRightLampNoteTextBox->Text;
					lampNoteLookUp[TURNLLEFT] = turnLLeftLampNoteTextBox->Text;
					lampNoteLookUp[BRAKELIGHT] = brakeLightLampNoteTextBox->Text;
					lampNoteLookUp[REVERSELIGHT] = reverseLightLampNoteTextBox->Text;
					lampNoteLookUp[FOGLIGHT] = fogLightLampNoteTextBox->Text;



					//read number of selected iterations
					//-> in case field is empty -> set to 1
					if (numberIterationTextBox->Text == "")
					{
						numberIterationTextBox->Text = "1";
					}


					//clear the incomingDataBuffer and the buffer index
					incomingDataBuffer->Clear(incomingDataBuffer, 0, incomingDataBuffer->Length);
					incomingDataBufferIndex=0;

					//check if at least one lamp channel is selected for the lamp classification
					if (selectedCharacteristicNumber != 0 && 
						(lampChannelWorkList[POSLRIGHT] == true ||
						 lampChannelWorkList[POSLLEFT] == true ||
						 lampChannelWorkList[TURNLRIGHT] == true ||
						 lampChannelWorkList[TURNLLEFT] == true ||
						 lampChannelWorkList[BRAKELIGHT] == true ||
						 lampChannelWorkList[REVERSELIGHT] == true || 
						 lampChannelWorkList[FOGLIGHT] == true) )
					{
						//check if for all selected lamp channels the lamp number is set
						if ((posLRightSelectCheckBox->Checked == true && posLRightLampNrTextBox->Text == "") ||
							(posLLeftSelectCheckBox->Checked == true && posLLeftLampNrTextBox->Text == "") ||
							(turnLRightSelectCheckBox->Checked == true && turnLRightLampNrTextBox->Text == "") ||
							(turnLLeftSelectCheckBox->Checked == true && turnLLeftLampNrTextBox->Text == "") ||
							(brakeLightSelectCheckBox->Checked == true && brakeLightLampNrTextBox->Text == "") ||
							(reverseLightSelectCheckBox->Checked == true && reverseLightLampNrTextBox->Text == "") ||
							(fogLightSelectCheckBox->Checked == true && fogLightLampNrTextBox->Text == ""))
						{
							commLogBox->AppendText("[INFO]: please set Lamp# for all selected lamp channels." + Environment::NewLine);
						}
						else
						{
							//disable controls		
							usedCharacteristicComboBox->Enabled=false;
							usedCharacterisicLabel->Enabled=false;
							numberIterationTextBox->Enabled=false;
							selectTestTypeLabel->Enabled=false;
							informationModeComboBox->Enabled=false;
							informationModeLabel->Enabled=false;
							connectSerialButton->Enabled=false;
							disconnectSerialButton->Enabled=false;

							//do not disable the start classification button if pwm mode is active
							//the same  for the contents of the group box
							if (dutyCycleLabel->Visible == false)
							{
								startClassificationButton->Enabled=false;
								lampSelectionGroupBox->Enabled=false;
							}

							//check for next item on worklist
							for (int i=0; i<lampChannelWorkList->Length; i++)
							{
								if (lampChannelWorkList[i]==true)
								{
									//next lamp channel to be processed
									currentWorklistIndex = i;
									
									stateMachine_state = START_CLASSIFICATION;
									
									//clear the incomingDataBuffer and the buffer index
									incomingDataBuffer->Clear(incomingDataBuffer, 0, incomingDataBuffer->Length);
									incomingDataBufferIndex=0;
									break;
								}
								else
								{
									if (i == (lampChannelWorkList->Length)-1)
									{
										//end of worklist is reached (no lamp channel was selected)
										stateMachine_state = IDLE;
									}
								}
							}

							currentIteration=0;
							//call state machine handler to execute the first state							
							stateMachineHandler();
						}
					}
					else
					{
						commLogBox->AppendText("[INFO]: please select at least one characteristic and lamp channel." + Environment::NewLine);
					}
				}

		 }
private: System::Void posLRightLampNrTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
				
				int lampNumber;
				//check user input for correctness
				try
				{
					if (posLRightLampNrTextBox->Text != "")
					{
						lampNumber = Convert::ToInt32(posLRightLampNrTextBox->Text);
					}
				}
				catch (Exception^)
				{
					commLogBox->AppendText("[INFO]: wrong value for lamp number." + Environment::NewLine);
					commLogBox->AppendText("[INFO]: must be a number." + Environment::NewLine);	
					posLRightLampNrTextBox->Text = "";
				}
		 }
private: System::Void posLLeftLampNrTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
				
				int lampNumber;
				//check user input for correctness
				try
				{
					if (posLLeftLampNrTextBox->Text != "")
					{
						lampNumber = Convert::ToInt32(posLLeftLampNrTextBox->Text);
					}
				}
				catch (Exception^)
				{
					commLogBox->AppendText("[INFO]: wrong value for lamp number." + Environment::NewLine);
					commLogBox->AppendText("[INFO]: must be a number." + Environment::NewLine);	
					posLLeftLampNrTextBox->Text = "";
				}
		 }
private: System::Void turnLRightLampNrTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
				
				int lampNumber;
				//check user input for correctness
				try
				{
					if (turnLRightLampNrTextBox->Text != "")
					{
						lampNumber = Convert::ToInt32(turnLRightLampNrTextBox->Text);
					}
				}
				catch (Exception^)
				{
					commLogBox->AppendText("[INFO]: wrong value for lamp number." + Environment::NewLine);
					commLogBox->AppendText("[INFO]: must be a number." + Environment::NewLine);	
					turnLRightLampNrTextBox->Text = "";
				}
		 }
private: System::Void turnLLeftLampNrTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
				
				int lampNumber;
				//check user input for correctness
				try
				{
					if (turnLLeftLampNrTextBox->Text != "")
					{
						lampNumber = Convert::ToInt32(turnLLeftLampNrTextBox->Text);
					}
				}
				catch (Exception^)
				{
					commLogBox->AppendText("[INFO]: wrong value for lamp number." + Environment::NewLine);
					commLogBox->AppendText("[INFO]: must be a number." + Environment::NewLine);	
					turnLLeftLampNrTextBox->Text = "";
				}
		 }
private: System::Void brakeLightLampNrTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
				
				int lampNumber;
				//check user input for correctness
				try
				{
					if (brakeLightLampNrTextBox->Text != "")
					{
						lampNumber = Convert::ToInt32(brakeLightLampNrTextBox->Text);
					}
				}
				catch (Exception^)
				{
					commLogBox->AppendText("[INFO]: wrong value for lamp number." + Environment::NewLine);
					commLogBox->AppendText("[INFO]: must be a number." + Environment::NewLine);	
					brakeLightLampNrTextBox->Text = "";
				}
		 }
private: System::Void reverseLightLampNrTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
				
				int lampNumber;
				//check user input for correctness
				try
				{
					if (reverseLightLampNrTextBox->Text != "")
					{
						lampNumber = Convert::ToInt32(reverseLightLampNrTextBox->Text);
					}
				}
				catch (Exception^)
				{
					commLogBox->AppendText("[INFO]: wrong value for lamp number." + Environment::NewLine);
					commLogBox->AppendText("[INFO]: must be a number." + Environment::NewLine);	
					reverseLightLampNrTextBox->Text = "";
				}
		 }
private: System::Void fogLightLampNrTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
				
				int lampNumber;
				//check user input for correctness
				try
				{
					if (fogLightLampNrTextBox->Text != "")
					{
						lampNumber = Convert::ToInt32(fogLightLampNrTextBox->Text);
					}
				}
				catch (Exception^)
				{
					commLogBox->AppendText("[INFO]: wrong value for lamp number." + Environment::NewLine);
					commLogBox->AppendText("[INFO]: must be a number." + Environment::NewLine);	
					fogLightLampNrTextBox->Text = "";
				}
		 }
private: System::Void abortClassificationButton_Click(System::Object^  sender, System::EventArgs^  e) {
				//enable controls
				lampSelectionGroupBox->Enabled=true;
				startClassificationButton->Enabled=true;
				usedCharacteristicComboBox->Enabled=true;
				usedCharacterisicLabel->Enabled=true;
				numberIterationTextBox->Enabled=true;
				selectTestTypeLabel->Enabled=true;
				informationModeComboBox->Enabled=true;
				informationModeLabel->Enabled=true;
				connectSerialButton->Enabled=true;
				disconnectSerialButton->Enabled=true;
				currentIteration = 0;
				currentWorklistIndex = 0;
				stateMachine_state = IDLE;
				//cancel classification process iterations
				currentIteration = numberOfIterations;
		 }
private: System::Void posLRightFlashingCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

				if (posLRightFlashingCheckBox->Checked)
				{
					lampTypeLookUp[POSLRIGHT] = "Flashing";
				}
				else
				{
					lampTypeLookUp[POSLRIGHT] = "Non-Flashing";
				}
		 }
private: System::Void posLLeftFlashingCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				
				if (posLLeftFlashingCheckBox->Checked)
				{
					lampTypeLookUp[POSLLEFT] = "Flashing";
				}
				else
				{
					lampTypeLookUp[POSLLEFT] = "Non-Flashing";
				}
		 }
private: System::Void turnLRightFlashingCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 				
				if (turnLRightFlashingCheckBox->Checked)
				{
					lampTypeLookUp[TURNLRIGHT] = "Flashing";
				}
				else
				{
					lampTypeLookUp[TURNLRIGHT] = "Non-Flashing";
				}
		 }
private: System::Void turnLLeftFlashingCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 			 				
				if (turnLLeftFlashingCheckBox->Checked)
				{
					lampTypeLookUp[TURNLLEFT] = "Flashing";
				}
				else
				{
					lampTypeLookUp[TURNLLEFT] = "Non-Flashing";
				}
		 }
private: System::Void brakeLightFlashingCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 			 			 				
				if (brakeLightFlashingCheckBox->Checked)
				{
					lampTypeLookUp[BRAKELIGHT] = "Flashing";
				}
				else
				{
					lampTypeLookUp[BRAKELIGHT] = "Non-Flashing";
				}
		 }
private: System::Void reverseLightFlashingCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 			 			 			 				
				if (reverseLightFlashingCheckBox->Checked)
				{
					lampTypeLookUp[REVERSELIGHT] = "Flashing";
				}
				else
				{
					lampTypeLookUp[REVERSELIGHT] = "Non-Flashing";
				}
		 }
private: System::Void fogLightFlashingCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 			 			 			 			 				
				if (fogLightFlashingCheckBox->Checked)
				{
					lampTypeLookUp[FOGLIGHT] = "Flashing";
				}
				else
				{
					lampTypeLookUp[FOGLIGHT] = "Non-Flashing";
				}
		 }

private: System::Void pwmViewToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

				posLRightSelectCheckBox->Checked = true;
				
				allLampsSelectCheckBox->Enabled = false;
				posLLeftSelectCheckBox->Enabled = false;
				turnLRightSelectCheckBox->Enabled = false;
				turnLLeftSelectCheckBox->Enabled = false;
				brakeLightSelectCheckBox->Enabled = false;
				reverseLightSelectCheckBox->Enabled = false;
				fogLightSelectCheckBox->Enabled = false;

				allLampsSelectCheckBox->Checked = false;
				posLLeftSelectCheckBox->Checked = false;
				turnLRightSelectCheckBox->Checked = false;
				turnLLeftSelectCheckBox->Checked = false;
				brakeLightSelectCheckBox->Checked = false;
				reverseLightSelectCheckBox->Checked = false;
				fogLightSelectCheckBox->Checked = false;

				posLLeftLampNrTextBox->Enabled = false;
				turnLRightLampNrTextBox->Enabled = false;
				turnLLeftLampNrTextBox->Enabled = false;
				brakeLightLampNrTextBox->Enabled = false;
				reverseLightLampNrTextBox->Enabled = false;
				fogLightLampNrTextBox->Enabled = false;

				posLLeftLampNoteTextBox->Enabled = false;
				turnLRightLampNoteTextBox->Enabled = false;
				turnLLeftLampNoteTextBox->Enabled = false;
				brakeLightLampNoteTextBox->Enabled = false;
				reverseLightLampNoteTextBox->Enabled = false;
				fogLightLampNoteTextBox->Enabled = false;

				posLLeftFlashingCheckBox->Enabled = false;
				turnLRightFlashingCheckBox->Enabled = false;
				turnLLeftFlashingCheckBox->Enabled = false;
				brakeLightFlashingCheckBox->Enabled = false;
				reverseLightFlashingCheckBox->Enabled = false;
				fogLightFlashingCheckBox->Enabled = false;

				posLRightResultTextBox->Enabled=false;
				posLLeftResultTextBox->Enabled=false;
				turnLRightResultTextBox->Enabled=false;
				turnLLeftResultTextBox->Enabled=false;
				brakeLightResultTextBox->Enabled=false;
				reverseLightResultTextBox->Enabled=false;
				fogLightResultTextBox->Enabled=false;

				startClassificationButton->Text = "Update pwm";
				abortClassificationButton->Enabled = false;

				dutyCycleLabel->Visible = true;
				dutyCycleTextBox->Visible = true;
				//pwmGroupBox->Visible = true;

				usedCharacteristicComboBox->SelectedIndex = 5;
				usedCharacteristicComboBox->Enabled = false;

		 }
private: System::Void standardViewToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

			
				allLampsSelectCheckBox->Enabled = true;
				posLLeftSelectCheckBox->Enabled = true;
				turnLRightSelectCheckBox->Enabled = true;
				turnLLeftSelectCheckBox->Enabled = true;
				brakeLightSelectCheckBox->Enabled = true;
				reverseLightSelectCheckBox->Enabled = true;
				fogLightSelectCheckBox->Enabled = true;

				posLLeftLampNrTextBox->Enabled = true;
				turnLRightLampNrTextBox->Enabled = true;
				turnLLeftLampNrTextBox->Enabled = true;
				brakeLightLampNrTextBox->Enabled = true;
				reverseLightLampNrTextBox->Enabled = true;
				fogLightLampNrTextBox->Enabled = true;
				
				posLLeftLampNoteTextBox->Enabled = true;
				turnLRightLampNoteTextBox->Enabled = true;
				turnLLeftLampNoteTextBox->Enabled = true;
				brakeLightLampNoteTextBox->Enabled = true;
				reverseLightLampNoteTextBox->Enabled = true;
				fogLightLampNoteTextBox->Enabled = true;

				posLLeftFlashingCheckBox->Enabled = true;
				turnLRightFlashingCheckBox->Enabled = true;
				turnLLeftFlashingCheckBox->Enabled = true;
				brakeLightFlashingCheckBox->Enabled = true;
				reverseLightFlashingCheckBox->Enabled = true;
				fogLightFlashingCheckBox->Enabled = true;

				posLRightResultTextBox->Enabled=true;
				posLLeftResultTextBox->Enabled=true;
				turnLRightResultTextBox->Enabled=true;
				turnLLeftResultTextBox->Enabled=true;
				brakeLightResultTextBox->Enabled=true;
				reverseLightResultTextBox->Enabled=true;
				fogLightResultTextBox->Enabled=true;

				startClassificationButton->Text = "Start classification";
				abortClassificationButton->Enabled = true;

				
				dutyCycleLabel->Visible = false;
				dutyCycleTextBox->Visible = false;


				usedCharacteristicComboBox->SelectedIndex = 0;
				usedCharacteristicComboBox->Enabled = true;
		 }
private: System::Void dutyCycleTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {

				int dutyCycle;
				//check user input for correctness
				try
				{
					if (dutyCycleTextBox->Text != "")
					{
						dutyCycle = Convert::ToInt32(dutyCycleTextBox->Text);
						if (dutyCycle < 0)
						{
							dutyCycleTextBox->Text = "0";
							commLogBox->AppendText("[INFO]: no negative number for duty cycle allowed." + Environment::NewLine);
							commLogBox->AppendText("[INFO]: duty cycle is set to 0%" + Environment::NewLine);
						}
						if (dutyCycle > 100)
						{
							dutyCycleTextBox->Text = "100";
							commLogBox->AppendText("[INFO]: maximum duty cycle is 100%." + Environment::NewLine);
							commLogBox->AppendText("[INFO]: duty cycle is set to 100%" + Environment::NewLine);
						}
					
					}
				}
				catch (Exception^)
				{
					commLogBox->AppendText("[INFO]: wrong value for duty cycle." + Environment::NewLine);
					commLogBox->AppendText("[INFO]: must be a number." + Environment::NewLine);	
					dutyCycleTextBox->Text = "";
				}
		 }
private: System::Void usedCharacteristicComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

				
				if (usedCharacteristicComboBox->SelectedIndex == 5)
				{
					//only allow to select the pwm mode entry when the software is in the pwm view
					//(the pwm view menu item has been clicked)
					if (dutyCycleLabel->Visible == true)
					{
						//pwm mode
						selectedCharacteristicNumber = 201;
					}
					else 
					{
						commLogBox->AppendText("[INFO]: please activate pwm view first. " + Environment::NewLine);
						commLogBox->AppendText("[INFO]: menu: view -> pwm view " + Environment::NewLine);
						usedCharacteristicComboBox->SelectedIndex = 0;
					}
				}


				if (dutyCycleLabel->Visible == true && usedCharacteristicComboBox->SelectedIndex != 5)
				{
					commLogBox->AppendText("[INFO]: action is not allowed in pwm mode " + Environment::NewLine);
					usedCharacteristicComboBox->SelectedIndex = 9;
				}
		 }
private: System::Void waitTimeTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {

				
				//check user input for correctness
				try
				{
					if (waitTimeTextBox->Text != "")
					{
						waitingTimeBetweenIterations = Convert::ToInt32(waitTimeTextBox->Text);
					}
				}
				catch (Exception^)
				{
					commLogBox->AppendText("[INFO]: wrong value for wait time." + Environment::NewLine);
					commLogBox->AppendText("[INFO]: must be a number." + Environment::NewLine);	
					waitTimeTextBox->Text = "0";
					waitingTimeBetweenIterations = 0;
				}
		 }

private: System::Void toe8872ControlToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

			 try 
			 {
				if (isToeDialogActive == true)
				{
					toeControlDialog->Hide();
					isToeDialogActive = false;
					toe8872ControlToolStripMenuItem->Checked = 0;
				}
				else
				{
					toeControlDialog->Show();
					toeControlDialog->Location = Point(653,100);
					isToeDialogActive = true;
					toe8872ControlToolStripMenuItem->Checked = 1;
				}
			 }
			 catch (Exception ^)
			 {
				 //toe form has been closed by user (with X button)
				 toeControlDialog = gcnew toeControl;
				 toeControlDialog->Show();
				 isToeDialogActive = true;
				 toe8872ControlToolStripMenuItem->Checked = 1;
			 }

		 }
private: System::Void brakeLButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 posLRightLampNoteTextBox->Text = "brakeL";
				 posLLeftLampNoteTextBox->Text = "brakeL";
				 turnLRightLampNoteTextBox->Text = "brakeL";
				 turnLLeftLampNoteTextBox->Text = "brakeL";
				 brakeLightLampNoteTextBox->Text = "brakeL";
				 reverseLightLampNoteTextBox->Text = "brakeL";
				 fogLightLampNoteTextBox->Text = "brakeL";
		 }
private: System::Void posLButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 posLRightLampNoteTextBox->Text = "posL";
				 posLLeftLampNoteTextBox->Text = "posL";
				 turnLRightLampNoteTextBox->Text = "posL";
				 turnLLeftLampNoteTextBox->Text = "posL";
				 brakeLightLampNoteTextBox->Text = "posL";
				 reverseLightLampNoteTextBox->Text = "posL";
				 fogLightLampNoteTextBox->Text = "posL";
		 }
private: System::Void turnLButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 	 posLRightLampNoteTextBox->Text = "turnL";
				 posLLeftLampNoteTextBox->Text = "turnL";
				 turnLRightLampNoteTextBox->Text = "turnL";
				 turnLLeftLampNoteTextBox->Text = "turnL";
				 brakeLightLampNoteTextBox->Text = "turnL";
				 reverseLightLampNoteTextBox->Text = "turnL";
				 fogLightLampNoteTextBox->Text = "turnL";
		 }
};
}

