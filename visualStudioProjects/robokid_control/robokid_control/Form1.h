#include "showStatisticsForm.h"
#include "mapForm.h"

#include <windows.h>


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
			selectPortBox->SelectedIndex=4;
			//selectMsgBox->SelectedIndex=0;

			//create sub-dialog	
			showStatisticsDialog = gcnew showStatisticsForm;
			mapDialog = gcnew mapForm;

			//Assign delegate to updateComLogBoxMethod
			updateComLogBoxDelegate = gcnew updateComLogBox(this, &Form1::updateComLogBoxMethod);
			
			//Assign delegate to sendObstacle method
			
			//formFunctionPointer += gcnew functionCall(this, &Form1::sendObstacle);
			//mapDialog->userFunctionPointer = formFunctionPointer;
			//mapDialog->MdiParent = this;
			
			//set form start position
			this->Location = Point(100, 100);
			this->xCoordBox->Enabled = false;
			this->yCoordBox->Enabled = false;
			this->xTargetCoordBox->Enabled = false;
			this->yTargetCoordBox->Enabled = false;
			this->startModeButton->Enabled = false;
			this->label3->Enabled = false;
			this->label4->Enabled = false;
			this->label7->Enabled = false;
			this->label8->Enabled = false;
			this->xRobot=99;
			this->yRobot=99;
			this->xTarget=99;
			this->yTarget=99;
			this->forwardButton->Visible = false;
			this->backwardButton->Visible = false;
			this->rightButton->Visible = false;
			this->leftButton->Visible = false;
			this->stopButton->Visible = false;
			this->updateControlVariablesButton->Enabled = false;
			controlPValue=99;
			controlIValue=99;
			controlDValue=99;

			selectControllerBox->SelectedIndex=0;
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

	//class members:

	//***********************************************************************************************************
	// GUI parts:
	//***********************************************************************************************************
	private: System::Windows::Forms::Button^  connectRobokidbutton;
	private: System::Windows::Forms::Label^  serialConnectionLabel;
	private: System::Windows::Forms::GroupBox^  speedGroupBox;
	private: System::Windows::Forms::RadioButton^  speed9600Button;
	private: System::Windows::Forms::RadioButton^  speed57600Button;
	private: System::Windows::Forms::ComboBox^  selectPortBox;
	private: System::Windows::Forms::Label^  selectPortLabel;
	public: System::Windows::Forms::TextBox^  commLogBox;
	private: System::Windows::Forms::Label^  commDataLabel;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exitMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  displaysSoftwareVersionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  displaysSoftwareVersionToolStripMenuItem1;
	private: System::IO::Ports::SerialPort^  serialPort1;
	private: System::Windows::Forms::ToolStripMenuItem^  statisticsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showStatsToolStripMenuItem;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Button^  disconnectRobokidButton;



	//statistics form
	private: showStatisticsForm^ showStatisticsDialog;
	//map form
	private: mapForm^ mapDialog;
	
	//***********************************************************************************************************
	// data variables:
	//***********************************************************************************************************
	//string for the incoming data of the serial port
	public: 
			String^ incomingData;
			int indexLeft, sampleLeft;
			int indexRight, sampleRight;
			int controlPValue, controlIValue, controlDValue;
	private:
			String^ modeMessage;

	private: System::Windows::Forms::GroupBox^  groupBox1;


	private: int xRobot, yRobot;
	private: int xTarget, yTarget;
	private: System::Windows::Forms::Button^  clearLogButton;


	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  yCoordBox;

	private: System::Windows::Forms::TextBox^  xCoordBox;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ComboBox^  selectModeBox;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  startModeButton;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::TextBox^  yTargetCoordBox;
	private: System::Windows::Forms::TextBox^  xTargetCoordBox;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Button^  backwardButton;

	private: System::Windows::Forms::Button^  forwardButton;
	private: System::Windows::Forms::Button^  leftButton;

	private: System::Windows::Forms::Button^  rightButton;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Button^  stopButton;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::ComboBox^  selectControllerBox;
	private: System::Windows::Forms::TextBox^  controlDBox;
	private: System::Windows::Forms::TextBox^  controlIBox;
	private: System::Windows::Forms::TextBox^  controlPBox;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::GroupBox^  groupBox4;
	private: System::Windows::Forms::Button^  updateControlVariablesButton;

	private: System::Windows::Forms::ToolStripMenuItem^  standardViewToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  xbeeConfigToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  readBaudRateToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  changeBaudRateToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  changeBaudRateMenuItem57600;
	private: System::Windows::Forms::ToolStripMenuItem^  changeBaudRateMenuItem9600;
private: System::Windows::Forms::ToolStripMenuItem^  checkConnectionMenuItem;







	//***********************************************************************************************************
	// methods & delegates:
	//***********************************************************************************************************
	public:
		//creating the delegate for function updateComLogBoxMethod()
		delegate void updateComLogBox();
		updateComLogBox^ updateComLogBoxDelegate;

	public:
		//creating the delegate that allows the child form "mapForm" to call the parent´s member function "sendObstacle"
		//delegate void functionCall(int x, int y);
	private:
		//event functionCall^ formFunctionPointer;
			
	
	//***********************************************************************************************************
	// updateComLogBoxMethod()
	// -----------------------	
	//method to display the read data from the serial port in the logTextBox
	//serialPort.read() is executed in an extra thread and can therefore not
	//access the components of form1
	//
	// -> the robot messages, while transferring the robot map to the host, are not displayed
	//
	// -> I changed the reading of the serial port incoming data to "ReadExisting". Otherwise with the old
	//    readLine() the software hangs when entering the Xbee command mode. (Xbee sends "OK<cr>")
	//***********************************************************************************************************
	public:
		void updateComLogBoxMethod()
		{	
			//flag to determine the source of an incoming message
			//=1 -> xbee message
			int xbeeMessageFlag=0;
			array<wchar_t,1>^ singleChar;
			singleChar = gcnew array<wchar_t,1> (1);
			
			serialPort1->Read(singleChar,0,1);

			incomingData = Convert::ToString(singleChar[0]);


			//determine the source of the message
			try
			{
				//message from robot starts not with the following characters 
				if (incomingData[0]!='O' && incomingData[0]!='3' && incomingData[0]!='6')
				{
					//read timeout (exception) if no line end is received inside 100ms boundary
					serialPort1->ReadTimeout=5000;
					try
					{
							incomingData += serialPort1->ReadLine();
							xbeeMessageFlag=0;
					}
					catch (TimeoutException^)
					{
						commLogBox->AppendText("Serial port timeout!" + Environment::NewLine);	
					}
				}
				//everything else is a message from the xbee device
				else
				{
					try
					{
						incomingData += serialPort1->ReadExisting();
						xbeeMessageFlag=1;
					}
					catch(Exception^)
					{
						commLogBox->AppendText("Serial port -> read existing exception!" + Environment::NewLine);	
					}
				}
			}
			catch(Exception^)
			{
				commLogBox->AppendText("Serial port -> read exception!" + Environment::NewLine);	
			}
			
			
			//display the incoming messages on the commLogBox and state the source of the message
			//(depending on the beginning of the message frame)
			try
			{	
				//-> do not display the messages during a map transmission (in this case incomingData[1]=='M')
				if (xbeeMessageFlag==0 && incomingData->Length>1)
				{
					if (incomingData[1] != 'M')
					{
						commLogBox->AppendText("[ROBOT]: " + incomingData + Environment::NewLine);
					}
				}
				else
				{
					commLogBox->AppendText("[Xbee]: " + incomingData + Environment::NewLine);
				}
			}
			catch (Exception^)
			{
				commLogBox->AppendText("Incoming message exception!" + Environment::NewLine);
			}

			//call message handler
			incomingMsgHandler();

		}
	//***********************************************************************************************************
	// incomingMsgHandler()
	// -----------------------	
	//method to determine what kind of message is incoming and to proceed the next steps necessary
	//***********************************************************************************************************
	public:
		void incomingMsgHandler()
		{
			try
			{
				//for incoming messages from the XBee module
				if (incomingData[0] != 'b')
				{
					switch(incomingData[0])
					{
					case '3':
						commLogBox->AppendText("[Xbee]: baud rate = 9600" + Environment::NewLine);
						//exit command mode
						serialPort1->Write("ATCN\r");
						break;
					case '6':
						commLogBox->AppendText("[Xbee]: baud rate = 57600" + Environment::NewLine);
						//exit command mode
						serialPort1->Write("ATCN\r");
						break;
					}
				}
				else
				{
					//for incoming messages from the robot
					switch(incomingData[1])
					{
						case 'F':	
							displayFrontData();
							break;
						case 'M':
							storeIncomingMap();
							break;
						case 'P':
							storeIncomingPath();
							break;
						default:
							//commLogBox->AppendText("[INFO]: received unknown message type" + Environment::NewLine);
							break;
					}
				}
			}
			catch (Exception^)
			{
				commLogBox->AppendText("Message handler exception!" + Environment::NewLine);
			}
		}
	//***********************************************************************************************************
	// displayFrontData()
	// -----------------------	
	// - chops the incoming data string with the front sensor data down to its pieces
	// - saves this pieces into the correct variables.
	// - calls a function to load the new sensor data into the map form
	//***********************************************************************************************************
	public:
		void displayFrontData()
		{	
			//variable to determine whether one sample msg is lost
			//->checks if the indizes are in right order
			static int lastIndexLeft=99;
			static int lastIndexRight=99;
			int errorLeftIndex=1;
			int errorRightIndex=1;
			
			String^ partString;
			
			//read indexLeft from incoming string 
			partString = incomingData->Substring(2,2);
			indexLeft = Convert::ToInt32(partString);
			//read sampleLeft from incoming string
			partString = incomingData->Substring(4,2);
			sampleLeft = Convert::ToInt32(partString);

			//read indexRight from incoming string 
			partString = incomingData->Substring(6,2);
			indexRight = Convert::ToInt32(partString);
			//read sampleRight from incoming string
			partString = incomingData->Substring(8,2);
			sampleRight = Convert::ToInt32(partString);

			//commLogBox->AppendText("indexLeft: "+ indexLeft + Environment::NewLine);
			//commLogBox->AppendText("lastIndexLeft: "+ lastIndexLeft + Environment::NewLine);
			//commLogBox->AppendText("indexRight: "+ indexRight + Environment::NewLine);
			//commLogBox->AppendText("lastIndexLeft: "+ lastIndexRight + Environment::NewLine);

			if (lastIndexLeft!=99)
			{
				//check if LEFT indizes are in right order
				if ((indexLeft-lastIndexLeft)==1) 
				{
					//-> right order if no transition 14->0
					errorLeftIndex=0;
				}
				else if ((lastIndexLeft==14) && (indexLeft==0))
				{
					//-> right order at transition 14->0
					errorLeftIndex=0;
				}
				else
					errorLeftIndex=1;

				//check if RIGHT indizes are in right order
				if ((indexRight-lastIndexRight)==1) 
				{
					//-> right order if no transition 29->15
					errorRightIndex=0;
				
				}
				else if ((lastIndexRight==29) && (indexRight==15))
				{
					//-> right order at transition 29->15
					errorRightIndex=0;
				}
				else
					errorRightIndex=1;

				//check if indizes have proper values between 0 & 29
				//if wrong value set index to 0
				//-> this produces an index order error in the next function rung but
				//   prevents an array out of boundary error.
				if (indexLeft>=0 && indexLeft<30)
				{
					errorLeftIndex=0;
				}
				else
				{
					errorLeftIndex=1;
					indexLeft=0;
				}
				if (indexRight>=0 && indexRight<30)
				{
					errorRightIndex=0;
				}
				else
				{
					errorRightIndex=1;
					indexRight=0;
				}

				if (errorLeftIndex)
				{
					commLogBox->AppendText("[SYSTEM]: left sensor index error." + Environment::NewLine);

				}
				if (errorRightIndex)
				{
					commLogBox->AppendText("[SYSTEM]: right sensor index error." + Environment::NewLine);
				}

				if ((errorLeftIndex!=1) && (errorRightIndex!=1))
				{
					loadDataIntoMap();
				}
			}
			
			if (indexLeft==14)
			{	
				//after one complete cycle of front sensor readings:
				// -> update robot´s pose (!!under construction!!)
				// -> refresh mapForm to draw samples on map
				// BETTER: robot is updating its pose by itself and is sending an extra message
				//mapDialog->setRobotPose(34,34,0);
				mapDialog->obstacleUpdateFlag=1;
				mapDialog->Refresh();
			}

			lastIndexLeft=indexLeft;
			lastIndexRight=indexRight;

			
			//after one complete sensor turn refresh map form!!!

			//test output in commLogBox
			/*
			commLogBox->AppendText("[INFO]: indexLeft:" + indexLeft  + Environment::NewLine);
			commLogBox->AppendText("[INFO]: sampleLeft:" + sampleLeft  + Environment::NewLine);
			commLogBox->AppendText("[INFO]: indexRight:" + indexRight  + Environment::NewLine);
			commLogBox->AppendText("[INFO]: sampleRight:" + sampleRight  + Environment::NewLine);
			*/
		}
	//***********************************************************************************************************
	// loadDataIntoMap()
	// -----------------------	
	// - calls the setFrontSensorData method of class mapForm (object mapDialog) to store the new data in the mapForm
	// - calls Refresh() function which forces the map form to redraw and therefore to trigger its paint event
	//***********************************************************************************************************
	public:
		void loadDataIntoMap()
		{
			mapDialog->setFrontSensorData(indexLeft, sampleLeft, indexRight, sampleRight);
		}
	//***********************************************************************************************************
	// storeIncomingMap()
	// -----------------------	
	// The robot is now sending its internal globalMap bytewise to the host.
	// Function:
	//	- stores each incoming byte in a temporary map (member variable of mapDialog)
	//  - the robot is also sending a controlIndex variable that can be used to keep track to which map cell
	//    the currently sent value belongs to. -> if msgs are lost -> no wrong data in the tempMap (just missing data)
	//    -> values of controlIndex: 0->4899 (70x70 map)
	//  
	// Message frame:      bMxxxyyyy\r\n
	//   
	//					   -> xxx:  3-digit map value
	//					   -> yyyy: 4-digit controlIndex
	//
	//
	//   Due to speed problems the controlIndex feature is deactivated.
	//   -> Maybe later I can use the host software the switch the XBEE to
	//      a higher tranfer rate. (using remote programming)
	//***********************************************************************************************************
	public:
		void storeIncomingMap()
		{
			int controlIndex=0;
			int i=0,j=0;
			static int debugCounter=0;

			String^ partString;
			
			//read controlIndex from incoming string
			partString = incomingData->Substring(5,4);
			controlIndex = Convert::ToInt32(partString);
			
			//calculate map indizes from controlIndex
			if (controlIndex>=0 && controlIndex<4900)
			{
				i = (int) (controlIndex/70);
				j = controlIndex%70;
				//read mapValue from incoming string 
				partString = incomingData->Substring(2,3);
				mapDialog->tempGlobalMap[i,j] = Convert::ToInt32(partString);
				debugCounter++;

				//give user feedback that software is still reading the incoming map data
				if (controlIndex%100==0)
				{
					commLogBox->AppendText("[SYSTEM]: ...transferring map data..." + Environment::NewLine);
				}
			}

			//commLogBox->AppendText("[INFO]: debug counter: " + debugCounter + Environment::NewLine);

		
			//whole map is sent
			if (controlIndex==5000)
			{
				commLogBox->AppendText("[SYSTEM]: data received completely." + Environment::NewLine);
				mapDialog->useIncomingMap();
				mapDialog->Refresh();
				debugCounter=0;
			}

			if (serialPort1->BytesToRead>0)
			{
				commLogBox->AppendText("[SYSTEM]: bytes to read left: " + serialPort1->BytesToRead + Environment::NewLine);
			}
		}
	//***********************************************************************************************************
	// sendObstacle()
	// -----------------------	
	// 
	//***********************************************************************************************************
	public: 
		void sendObstacle(int obstacleX, int obstacleY)
		{
			String^ singleChar;
			int j=0;
			String^ xCoordString;
			String^ yCoordString;
			String^ xyCoordString;
			String^ obstacleMsg;

			//send obstacle to robot
			if (serialPort1->IsOpen==false)
			{
				//commLogBox->AppendText("[INFO]: please open serial port first. " + Environment::NewLine);
			}
			else if (String::IsNullOrEmpty(modeMessage)==false)
			{
				//add the robot´s start position to the message
				xCoordString = Convert::ToString(obstacleX);
				yCoordString = Convert::ToString(obstacleY);
				xyCoordString = xCoordString + yCoordString;
									obstacleMsg = "bO" + xyCoordString + "e";
				for (int i=0; i<obstacleMsg->Length; i++)
				{
					singleChar = obstacleMsg->Substring(i,1);
					serialPort1->Write(singleChar);
					commLogBox->AppendText("[USER]: send char "+ singleChar + Environment::NewLine);
					Sleep(10);
				}
			}
		}
	//***********************************************************************************************************
	// storeIncomingPath()
	// -----------------------	
	//
	//   Message frame:      bPxxyy\r\n
	//   
	//		  	  	  	   -> xx:  2-digit path x-coordinate
	//			  	  	   -> yy:  2-digit path y-coordinate
	// 
	//***********************************************************************************************************
	public:
		void storeIncomingPath(void)
		{
			String^ partString;
			static int i=0;
			int tempX=0, tempY=0;

			//read indexLeft from incoming string 
			partString = incomingData->Substring(2,2);
			if (partString != "00")
			{
				mapDialog->pathVector[0,i] = Convert::ToInt32(partString);
				//read sampleLeft from incoming string
				partString = incomingData->Substring(4,2);
				mapDialog->pathVector[1,i] = Convert::ToInt32(partString);
				
				//store coordinates in globalMap
				tempX=mapDialog->pathVector[0,i];
				tempY=mapDialog->pathVector[1,i];
				i++;

				if (mapDialog->globalMap[tempX,tempY]!=1)
				{
					mapDialog->globalMap[tempX,tempY]=(i+2);
				}
			}
			else
			{
				commLogBox->AppendText("[SYSTEM]: path received completely." + Environment::NewLine);
				i=0;
				mapDialog->Refresh();
			}
		}
	//***********************************************************************************************************
	// sendPingMsg()
	// -----------------------	
	//
	//   Message frame:      bZe
	//   
	//	 Function:			 To check if communication with the robot is established, the ping message can be send
	//						 If the robot receives the message correctly it will answer with "Pong!\r\n"
	// 
	//***********************************************************************************************************
	public:
		void sendPingMsg(void)
		{
			 //sends a "ping" message to the robot
			 //if communication is enabled between host and robot -> robot answers with "pong"
			 String^ pingMessage="bZe";
			 String^ singleChar;
			 int i=0;

			 commLogBox->AppendText("[SYSTEM]: BING! " + Environment::NewLine);
			 
			 //send message to robot
			 for (i=0; i<pingMessage->Length; i++)
			 {
				 singleChar = pingMessage->Substring(i,1);
				 serialPort1->Write(singleChar);
				 Sleep(20);
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->connectRobokidbutton = (gcnew System::Windows::Forms::Button());
			this->serialConnectionLabel = (gcnew System::Windows::Forms::Label());
			this->speedGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->speed9600Button = (gcnew System::Windows::Forms::RadioButton());
			this->speed57600Button = (gcnew System::Windows::Forms::RadioButton());
			this->selectPortBox = (gcnew System::Windows::Forms::ComboBox());
			this->selectPortLabel = (gcnew System::Windows::Forms::Label());
			this->commLogBox = (gcnew System::Windows::Forms::TextBox());
			this->commDataLabel = (gcnew System::Windows::Forms::Label());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statisticsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showStatsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->standardViewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->xbeeConfigToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->checkConnectionMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->readBaudRateToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->changeBaudRateToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->changeBaudRateMenuItem57600 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->changeBaudRateMenuItem9600 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->displaysSoftwareVersionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->displaysSoftwareVersionToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->disconnectRobokidButton = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->yTargetCoordBox = (gcnew System::Windows::Forms::TextBox());
			this->xTargetCoordBox = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->yCoordBox = (gcnew System::Windows::Forms::TextBox());
			this->xCoordBox = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->selectModeBox = (gcnew System::Windows::Forms::ComboBox());
			this->startModeButton = (gcnew System::Windows::Forms::Button());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->updateControlVariablesButton = (gcnew System::Windows::Forms::Button());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->selectControllerBox = (gcnew System::Windows::Forms::ComboBox());
			this->controlDBox = (gcnew System::Windows::Forms::TextBox());
			this->controlIBox = (gcnew System::Windows::Forms::TextBox());
			this->controlPBox = (gcnew System::Windows::Forms::TextBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->stopButton = (gcnew System::Windows::Forms::Button());
			this->leftButton = (gcnew System::Windows::Forms::Button());
			this->rightButton = (gcnew System::Windows::Forms::Button());
			this->backwardButton = (gcnew System::Windows::Forms::Button());
			this->forwardButton = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->clearLogButton = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->speedGroupBox->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->contextMenuStrip1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// connectRobokidbutton
			// 
			this->connectRobokidbutton->Location = System::Drawing::Point(26, 252);
			this->connectRobokidbutton->Name = L"connectRobokidbutton";
			this->connectRobokidbutton->Size = System::Drawing::Size(104, 38);
			this->connectRobokidbutton->TabIndex = 0;
			this->connectRobokidbutton->Text = L"Connect";
			this->connectRobokidbutton->UseVisualStyleBackColor = true;
			this->connectRobokidbutton->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// serialConnectionLabel
			// 
			this->serialConnectionLabel->AutoSize = true;
			this->serialConnectionLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->serialConnectionLabel->Location = System::Drawing::Point(22, 37);
			this->serialConnectionLabel->Name = L"serialConnectionLabel";
			this->serialConnectionLabel->Size = System::Drawing::Size(131, 20);
			this->serialConnectionLabel->TabIndex = 3;
			this->serialConnectionLabel->Text = L"Serial connection";
			// 
			// speedGroupBox
			// 
			this->speedGroupBox->Controls->Add(this->speed9600Button);
			this->speedGroupBox->Controls->Add(this->speed57600Button);
			this->speedGroupBox->Location = System::Drawing::Point(26, 131);
			this->speedGroupBox->Name = L"speedGroupBox";
			this->speedGroupBox->Size = System::Drawing::Size(234, 101);
			this->speedGroupBox->TabIndex = 4;
			this->speedGroupBox->TabStop = false;
			this->speedGroupBox->Text = L"Speed (baud)";
			this->speedGroupBox->Enter += gcnew System::EventHandler(this, &Form1::groupBox1_Enter);
			// 
			// speed9600Button
			// 
			this->speed9600Button->AutoSize = true;
			this->speed9600Button->Checked = true;
			this->speed9600Button->Location = System::Drawing::Point(26, 71);
			this->speed9600Button->Name = L"speed9600Button";
			this->speed9600Button->Size = System::Drawing::Size(49, 17);
			this->speed9600Button->TabIndex = 1;
			this->speed9600Button->TabStop = true;
			this->speed9600Button->Text = L"9600";
			this->speed9600Button->UseVisualStyleBackColor = true;
			this->speed9600Button->Click += gcnew System::EventHandler(this, &Form1::radioButton1_Click);
			// 
			// speed57600Button
			// 
			this->speed57600Button->AutoSize = true;
			this->speed57600Button->Location = System::Drawing::Point(26, 33);
			this->speed57600Button->Name = L"speed57600Button";
			this->speed57600Button->Size = System::Drawing::Size(55, 17);
			this->speed57600Button->TabIndex = 0;
			this->speed57600Button->TabStop = true;
			this->speed57600Button->Text = L"57600";
			this->speed57600Button->UseVisualStyleBackColor = true;
			this->speed57600Button->CheckedChanged += gcnew System::EventHandler(this, &Form1::speed57600Button_CheckedChanged);
			this->speed57600Button->Click += gcnew System::EventHandler(this, &Form1::radioButton1_Click);
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
			this->selectPortBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox1_SelectedIndexChanged);
			// 
			// selectPortLabel
			// 
			this->selectPortLabel->AutoSize = true;
			this->selectPortLabel->Location = System::Drawing::Point(23, 68);
			this->selectPortLabel->Name = L"selectPortLabel";
			this->selectPortLabel->Size = System::Drawing::Size(89, 13);
			this->selectPortLabel->TabIndex = 6;
			this->selectPortLabel->Text = L"Select COM-Port:";
			this->selectPortLabel->Click += gcnew System::EventHandler(this, &Form1::label1_Click_1);
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
			this->commDataLabel->Click += gcnew System::EventHandler(this, &Form1::label3_Click);
			// 
			// menuStrip1
			// 
			this->menuStrip1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"menuStrip1.BackgroundImage")));
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->fileToolStripMenuItem, 
				this->statisticsToolStripMenuItem, this->xbeeConfigToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(540, 24);
			this->menuStrip1->TabIndex = 9;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->aboutMenuItem, 
				this->toolStripSeparator1, this->exitMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"&File";
			// 
			// aboutMenuItem
			// 
			this->aboutMenuItem->Name = L"aboutMenuItem";
			this->aboutMenuItem->Size = System::Drawing::Size(107, 22);
			this->aboutMenuItem->Text = L"&About";
			this->aboutMenuItem->Click += gcnew System::EventHandler(this, &Form1::aboutMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(104, 6);
			// 
			// exitMenuItem
			// 
			this->exitMenuItem->Name = L"exitMenuItem";
			this->exitMenuItem->Size = System::Drawing::Size(107, 22);
			this->exitMenuItem->Text = L"&Exit";
			this->exitMenuItem->Click += gcnew System::EventHandler(this, &Form1::exitMenuItem_Click);
			// 
			// statisticsToolStripMenuItem
			// 
			this->statisticsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->showStatsToolStripMenuItem, 
				this->standardViewToolStripMenuItem});
			this->statisticsToolStripMenuItem->Name = L"statisticsToolStripMenuItem";
			this->statisticsToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->statisticsToolStripMenuItem->Text = L"View";
			// 
			// showStatsToolStripMenuItem
			// 
			this->showStatsToolStripMenuItem->Name = L"showStatsToolStripMenuItem";
			this->showStatsToolStripMenuItem->Size = System::Drawing::Size(148, 22);
			this->showStatsToolStripMenuItem->Text = L"Expert view";
			this->showStatsToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::showStatsToolStripMenuItem_Click);
			// 
			// standardViewToolStripMenuItem
			// 
			this->standardViewToolStripMenuItem->Name = L"standardViewToolStripMenuItem";
			this->standardViewToolStripMenuItem->Size = System::Drawing::Size(148, 22);
			this->standardViewToolStripMenuItem->Text = L"Standard view";
			this->standardViewToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::standardViewToolStripMenuItem_Click);
			// 
			// xbeeConfigToolStripMenuItem
			// 
			this->xbeeConfigToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->checkConnectionMenuItem, 
				this->readBaudRateToolStripMenuItem, this->changeBaudRateToolStripMenuItem});
			this->xbeeConfigToolStripMenuItem->Name = L"xbeeConfigToolStripMenuItem";
			this->xbeeConfigToolStripMenuItem->Size = System::Drawing::Size(81, 20);
			this->xbeeConfigToolStripMenuItem->Text = L"XbeeConfig";
			// 
			// checkConnectionMenuItem
			// 
			this->checkConnectionMenuItem->Name = L"checkConnectionMenuItem";
			this->checkConnectionMenuItem->Size = System::Drawing::Size(170, 22);
			this->checkConnectionMenuItem->Text = L"Check connection";
			this->checkConnectionMenuItem->Click += gcnew System::EventHandler(this, &Form1::checkConnectionMenuItem_Click);
			// 
			// readBaudRateToolStripMenuItem
			// 
			this->readBaudRateToolStripMenuItem->Name = L"readBaudRateToolStripMenuItem";
			this->readBaudRateToolStripMenuItem->Size = System::Drawing::Size(170, 22);
			this->readBaudRateToolStripMenuItem->Text = L"Read baud rate";
			this->readBaudRateToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::readBaudRateToolStripMenuItem_Click);
			// 
			// changeBaudRateToolStripMenuItem
			// 
			this->changeBaudRateToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->changeBaudRateMenuItem57600, 
				this->changeBaudRateMenuItem9600});
			this->changeBaudRateToolStripMenuItem->Name = L"changeBaudRateToolStripMenuItem";
			this->changeBaudRateToolStripMenuItem->Size = System::Drawing::Size(170, 22);
			this->changeBaudRateToolStripMenuItem->Text = L"Change baud rate";
			// 
			// changeBaudRateMenuItem57600
			// 
			this->changeBaudRateMenuItem57600->Name = L"changeBaudRateMenuItem57600";
			this->changeBaudRateMenuItem57600->Size = System::Drawing::Size(104, 22);
			this->changeBaudRateMenuItem57600->Text = L"57600";
			this->changeBaudRateMenuItem57600->Click += gcnew System::EventHandler(this, &Form1::ChangeBaudRateMenuItem57600_Click);
			// 
			// changeBaudRateMenuItem9600
			// 
			this->changeBaudRateMenuItem9600->Name = L"changeBaudRateMenuItem9600";
			this->changeBaudRateMenuItem9600->Size = System::Drawing::Size(104, 22);
			this->changeBaudRateMenuItem9600->Text = L"9600";
			this->changeBaudRateMenuItem9600->Click += gcnew System::EventHandler(this, &Form1::changeBaudRateMenuItem9600_Click);
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->displaysSoftwareVersionToolStripMenuItem});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(108, 26);
			// 
			// displaysSoftwareVersionToolStripMenuItem
			// 
			this->displaysSoftwareVersionToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->displaysSoftwareVersionToolStripMenuItem1});
			this->displaysSoftwareVersionToolStripMenuItem->Name = L"displaysSoftwareVersionToolStripMenuItem";
			this->displaysSoftwareVersionToolStripMenuItem->Size = System::Drawing::Size(107, 22);
			this->displaysSoftwareVersionToolStripMenuItem->Text = L"About";
			this->displaysSoftwareVersionToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::displaysSoftwareVersionToolStripMenuItem_Click);
			// 
			// displaysSoftwareVersionToolStripMenuItem1
			// 
			this->displaysSoftwareVersionToolStripMenuItem1->Name = L"displaysSoftwareVersionToolStripMenuItem1";
			this->displaysSoftwareVersionToolStripMenuItem1->Size = System::Drawing::Size(209, 22);
			this->displaysSoftwareVersionToolStripMenuItem1->Text = L"Displays software version.";
			// 
			// serialPort1
			// 
			this->serialPort1->PortName = L"COM3";
			this->serialPort1->ErrorReceived += gcnew System::IO::Ports::SerialErrorReceivedEventHandler(this, &Form1::serialPort1_ErrorReceived);
			this->serialPort1->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(this, &Form1::serialPort1_DataReceived);
			// 
			// disconnectRobokidButton
			// 
			this->disconnectRobokidButton->Location = System::Drawing::Point(156, 252);
			this->disconnectRobokidButton->Name = L"disconnectRobokidButton";
			this->disconnectRobokidButton->Size = System::Drawing::Size(104, 39);
			this->disconnectRobokidButton->TabIndex = 10;
			this->disconnectRobokidButton->Text = L"Disconnect";
			this->disconnectRobokidButton->UseVisualStyleBackColor = true;
			this->disconnectRobokidButton->Click += gcnew System::EventHandler(this, &Form1::DisconnectButton_Click_1);
			// 
			// groupBox1
			// 
			this->groupBox1->BackColor = System::Drawing::Color::DarkGray;
			this->groupBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->groupBox1->Controls->Add(this->groupBox4);
			this->groupBox1->Controls->Add(this->label10);
			this->groupBox1->Controls->Add(this->groupBox3);
			this->groupBox1->Controls->Add(this->label9);
			this->groupBox1->Controls->Add(this->groupBox2);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(1, 310);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(538, 270);
			this->groupBox1->TabIndex = 12;
			this->groupBox1->TabStop = false;
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->label6);
			this->groupBox4->Controls->Add(this->label7);
			this->groupBox4->Controls->Add(this->label8);
			this->groupBox4->Controls->Add(this->yTargetCoordBox);
			this->groupBox4->Controls->Add(this->xTargetCoordBox);
			this->groupBox4->Controls->Add(this->label5);
			this->groupBox4->Controls->Add(this->label4);
			this->groupBox4->Controls->Add(this->label3);
			this->groupBox4->Controls->Add(this->yCoordBox);
			this->groupBox4->Controls->Add(this->xCoordBox);
			this->groupBox4->Controls->Add(this->label2);
			this->groupBox4->Controls->Add(this->selectModeBox);
			this->groupBox4->Controls->Add(this->startModeButton);
			this->groupBox4->Location = System::Drawing::Point(13, 35);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(154, 230);
			this->groupBox4->TabIndex = 30;
			this->groupBox4->TabStop = false;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(6, 127);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(80, 13);
			this->label6->TabIndex = 25;
			this->label6->Text = L"Target position:";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(84, 152);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(20, 13);
			this->label7->TabIndex = 24;
			this->label7->Text = L"Y :";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(9, 152);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(20, 13);
			this->label8->TabIndex = 23;
			this->label8->Text = L"X :";
			// 
			// yTargetCoordBox
			// 
			this->yTargetCoordBox->Location = System::Drawing::Point(108, 149);
			this->yTargetCoordBox->MaxLength = 2;
			this->yTargetCoordBox->Name = L"yTargetCoordBox";
			this->yTargetCoordBox->Size = System::Drawing::Size(25, 20);
			this->yTargetCoordBox->TabIndex = 22;
			this->yTargetCoordBox->TextChanged += gcnew System::EventHandler(this, &Form1::yTargetCoordBox_TextChanged);
			// 
			// xTargetCoordBox
			// 
			this->xTargetCoordBox->Location = System::Drawing::Point(34, 149);
			this->xTargetCoordBox->MaxLength = 2;
			this->xTargetCoordBox->Name = L"xTargetCoordBox";
			this->xTargetCoordBox->Size = System::Drawing::Size(25, 20);
			this->xTargetCoordBox->TabIndex = 21;
			this->xTargetCoordBox->TextChanged += gcnew System::EventHandler(this, &Form1::xTargetCoordBox_TextChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(6, 70);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(78, 13);
			this->label5->TabIndex = 20;
			this->label5->Text = L"Robot position:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(84, 93);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(20, 13);
			this->label4->TabIndex = 19;
			this->label4->Text = L"Y :";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(9, 93);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(20, 13);
			this->label3->TabIndex = 18;
			this->label3->Text = L"X :";
			// 
			// yCoordBox
			// 
			this->yCoordBox->Location = System::Drawing::Point(108, 90);
			this->yCoordBox->MaxLength = 2;
			this->yCoordBox->Name = L"yCoordBox";
			this->yCoordBox->Size = System::Drawing::Size(25, 20);
			this->yCoordBox->TabIndex = 16;
			this->yCoordBox->TextChanged += gcnew System::EventHandler(this, &Form1::yCoordBox_TextChanged);
			// 
			// xCoordBox
			// 
			this->xCoordBox->Location = System::Drawing::Point(34, 90);
			this->xCoordBox->MaxLength = 2;
			this->xCoordBox->Name = L"xCoordBox";
			this->xCoordBox->Size = System::Drawing::Size(25, 20);
			this->xCoordBox->TabIndex = 15;
			this->xCoordBox->TextChanged += gcnew System::EventHandler(this, &Form1::xCoord_TextChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(6, 17);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(96, 13);
			this->label2->TabIndex = 14;
			this->label2->Text = L"Select robot mode:";
			// 
			// selectModeBox
			// 
			this->selectModeBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->selectModeBox->FormattingEnabled = true;
			this->selectModeBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Idle", L"Manual", L"Pathfinding"});
			this->selectModeBox->Location = System::Drawing::Point(9, 33);
			this->selectModeBox->Name = L"selectModeBox";
			this->selectModeBox->Size = System::Drawing::Size(123, 21);
			this->selectModeBox->TabIndex = 13;
			this->selectModeBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::selectModeBox_SelectedIndexChanged_1);
			// 
			// startModeButton
			// 
			this->startModeButton->Location = System::Drawing::Point(14, 185);
			this->startModeButton->Name = L"startModeButton";
			this->startModeButton->Size = System::Drawing::Size(107, 35);
			this->startModeButton->TabIndex = 11;
			this->startModeButton->Text = L"send obstacles && start mode";
			this->startModeButton->UseVisualStyleBackColor = true;
			this->startModeButton->Click += gcnew System::EventHandler(this, &Form1::sendModeButton_Click);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(370, 52);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(118, 13);
			this->label10->TabIndex = 29;
			this->label10->Text = L"PID control parameters:";
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->updateControlVariablesButton);
			this->groupBox3->Controls->Add(this->label14);
			this->groupBox3->Controls->Add(this->selectControllerBox);
			this->groupBox3->Controls->Add(this->controlDBox);
			this->groupBox3->Controls->Add(this->controlIBox);
			this->groupBox3->Controls->Add(this->controlPBox);
			this->groupBox3->Controls->Add(this->label13);
			this->groupBox3->Controls->Add(this->label12);
			this->groupBox3->Controls->Add(this->label11);
			this->groupBox3->Location = System::Drawing::Point(373, 68);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(137, 166);
			this->groupBox3->TabIndex = 28;
			this->groupBox3->TabStop = false;
			// 
			// updateControlVariablesButton
			// 
			this->updateControlVariablesButton->Location = System::Drawing::Point(17, 136);
			this->updateControlVariablesButton->Name = L"updateControlVariablesButton";
			this->updateControlVariablesButton->Size = System::Drawing::Size(110, 23);
			this->updateControlVariablesButton->TabIndex = 37;
			this->updateControlVariablesButton->Text = L"Update";
			this->updateControlVariablesButton->UseVisualStyleBackColor = true;
			this->updateControlVariablesButton->Click += gcnew System::EventHandler(this, &Form1::updateControlVariablesButton_Click);
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(10, 14);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(86, 13);
			this->label14->TabIndex = 30;
			this->label14->Text = L"Select controller:";
			// 
			// selectControllerBox
			// 
			this->selectControllerBox->FormattingEnabled = true;
			this->selectControllerBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"Drive", L"WallFollowing"});
			this->selectControllerBox->Location = System::Drawing::Point(13, 31);
			this->selectControllerBox->Name = L"selectControllerBox";
			this->selectControllerBox->Size = System::Drawing::Size(116, 21);
			this->selectControllerBox->TabIndex = 36;
			this->selectControllerBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::selectControllerBox_SelectedIndexChanged);
			// 
			// controlDBox
			// 
			this->controlDBox->Location = System::Drawing::Point(60, 110);
			this->controlDBox->MaxLength = 2;
			this->controlDBox->Name = L"controlDBox";
			this->controlDBox->Size = System::Drawing::Size(25, 20);
			this->controlDBox->TabIndex = 35;
			this->controlDBox->TextChanged += gcnew System::EventHandler(this, &Form1::controlDBox_TextChanged);
			// 
			// controlIBox
			// 
			this->controlIBox->Location = System::Drawing::Point(60, 85);
			this->controlIBox->MaxLength = 2;
			this->controlIBox->Name = L"controlIBox";
			this->controlIBox->Size = System::Drawing::Size(25, 20);
			this->controlIBox->TabIndex = 34;
			this->controlIBox->TextChanged += gcnew System::EventHandler(this, &Form1::controlIBox_TextChanged);
			// 
			// controlPBox
			// 
			this->controlPBox->Location = System::Drawing::Point(60, 59);
			this->controlPBox->MaxLength = 2;
			this->controlPBox->Name = L"controlPBox";
			this->controlPBox->Size = System::Drawing::Size(25, 20);
			this->controlPBox->TabIndex = 33;
			this->controlPBox->TextChanged += gcnew System::EventHandler(this, &Form1::controlPBox_TextChanged);
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(15, 115);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(39, 13);
			this->label13->TabIndex = 32;
			this->label13->Text = L"k -> D:";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(16, 89);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(34, 13);
			this->label12->TabIndex = 31;
			this->label12->Text = L"k -> I:";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(16, 62);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(38, 13);
			this->label11->TabIndex = 30;
			this->label11->Text = L"k -> P:";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(177, 52);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(79, 13);
			this->label9->TabIndex = 27;
			this->label9->Text = L"Robot controls:";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->stopButton);
			this->groupBox2->Controls->Add(this->leftButton);
			this->groupBox2->Controls->Add(this->rightButton);
			this->groupBox2->Controls->Add(this->backwardButton);
			this->groupBox2->Controls->Add(this->forwardButton);
			this->groupBox2->Location = System::Drawing::Point(180, 68);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(174, 167);
			this->groupBox2->TabIndex = 26;
			this->groupBox2->TabStop = false;
			// 
			// stopButton
			// 
			this->stopButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"stopButton.BackgroundImage")));
			this->stopButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->stopButton->Location = System::Drawing::Point(60, 60);
			this->stopButton->Name = L"stopButton";
			this->stopButton->Size = System::Drawing::Size(50, 50);
			this->stopButton->TabIndex = 4;
			this->stopButton->UseVisualStyleBackColor = true;
			this->stopButton->Click += gcnew System::EventHandler(this, &Form1::stopButton_Click);
			// 
			// leftButton
			// 
			this->leftButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"leftButton.BackgroundImage")));
			this->leftButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->leftButton->Location = System::Drawing::Point(8, 60);
			this->leftButton->Name = L"leftButton";
			this->leftButton->Size = System::Drawing::Size(50, 50);
			this->leftButton->TabIndex = 3;
			this->leftButton->UseVisualStyleBackColor = true;
			this->leftButton->Click += gcnew System::EventHandler(this, &Form1::leftButton_Click);
			// 
			// rightButton
			// 
			this->rightButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"rightButton.BackgroundImage")));
			this->rightButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->rightButton->Location = System::Drawing::Point(112, 60);
			this->rightButton->Name = L"rightButton";
			this->rightButton->Size = System::Drawing::Size(50, 50);
			this->rightButton->TabIndex = 2;
			this->rightButton->UseVisualStyleBackColor = true;
			this->rightButton->Click += gcnew System::EventHandler(this, &Form1::rightButton_Click);
			// 
			// backwardButton
			// 
			this->backwardButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"backwardButton.BackgroundImage")));
			this->backwardButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->backwardButton->Location = System::Drawing::Point(60, 111);
			this->backwardButton->Name = L"backwardButton";
			this->backwardButton->Size = System::Drawing::Size(50, 50);
			this->backwardButton->TabIndex = 1;
			this->backwardButton->UseVisualStyleBackColor = true;
			this->backwardButton->Click += gcnew System::EventHandler(this, &Form1::backwardButton_Click);
			// 
			// forwardButton
			// 
			this->forwardButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"forwardButton.BackgroundImage")));
			this->forwardButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->forwardButton->Location = System::Drawing::Point(60, 9);
			this->forwardButton->Name = L"forwardButton";
			this->forwardButton->Size = System::Drawing::Size(50, 50);
			this->forwardButton->TabIndex = 0;
			this->forwardButton->UseVisualStyleBackColor = true;
			this->forwardButton->Click += gcnew System::EventHandler(this, &Form1::forwardButton_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(18, 16);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(134, 20);
			this->label1->TabIndex = 12;
			this->label1->Text = L"Robot control box";
			this->label1->Click += gcnew System::EventHandler(this, &Form1::label1_Click_2);
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
			// button1
			// 
			this->button1->Location = System::Drawing::Point(518, 71);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(20, 218);
			this->button1->TabIndex = 14;
			this->button1->Text = L"c l ear    map";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::mapClear_Click_1);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::DarkGray;
			this->ClientSize = System::Drawing::Size(540, 582);
			this->ContextMenuStrip = this->contextMenuStrip1;
			this->Controls->Add(this->button1);
			this->Controls->Add(this->clearLogButton);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->disconnectRobokidButton);
			this->Controls->Add(this->commLogBox);
			this->Controls->Add(this->selectPortLabel);
			this->Controls->Add(this->selectPortBox);
			this->Controls->Add(this->commDataLabel);
			this->Controls->Add(this->speedGroupBox);
			this->Controls->Add(this->serialConnectionLabel);
			this->Controls->Add(this->connectRobokidbutton);
			this->Controls->Add(this->menuStrip1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Text = L"Robokid Control Center";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->speedGroupBox->ResumeLayout(false);
			this->speedGroupBox->PerformLayout();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->contextMenuStrip1->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}

	//***********************************************************************************************************
	// event handlers:
	//***********************************************************************************************************
#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
				//show map dialog
				mapDialog->Show(this);
				
							
			 }
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

				if (serialPort1->IsOpen==false)
				{
					serialPort1->ReadBufferSize=16384;
				}
				//commLogBox->AppendText("[INFO]: serial input buffer size: " + serialPort1->ReadBufferSize + Environment::NewLine);				try
				try
				{
					serialPort1->Open();
				}
				catch (IOException^ )//!!does not work correctly.
				{
					commLogBox->AppendText("[SYSTEM]: Cannot open serial port." + Environment::NewLine);
				}
				catch (InvalidOperationException^)
				{
					commLogBox->AppendText("[SYSTEM]: Serial port is already open." + Environment::NewLine);
				}
				
				

				if (serialPort1->IsOpen==true)
				{
					serialPort1->DiscardInBuffer();    // clear receive buffer
					serialPort1->DiscardOutBuffer();   // clear transmit buffer
					commLogBox->AppendText("[SYSTEM]: Serial port is open." + Environment::NewLine);
				}
				if (selectModeBox->SelectedItem=="Idle")
				{
					 modeMessage="bCIe";
				}
				else if (selectModeBox->SelectedItem=="Manual")
				{
					 modeMessage="bCMe";
						 }
				else if (selectModeBox->SelectedItem=="Pathfinding")
				{
					 modeMessage="bCPe";
				}

			}
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void groupBox1_Enter(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void label1_Click_1(System::Object^  sender, System::EventArgs^  e) {
				 }
	private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
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
	private: System::Void label3_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void radioButton1_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (sender==speed57600Button)
					commLogBox->AppendText("[USER]: selected 57600 baud." + Environment::NewLine);
				 else if (sender==speed9600Button)
					commLogBox->AppendText("[USER]: selected 9600 baud." + Environment::NewLine);
			 }
	private: System::Void helpToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void aboutMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
					MessageBox::Show(L"V0.1 | AO | 220611",L"Version");
			 }
	private: System::Void exitMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
					Application::Exit();
			 }
	private: System::Void displaysSoftwareVersionToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	//Serial port: data received
	private: System::Void serialPort1_DataReceived(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) {
					//call delegate to transfer read data to textBox
					Invoke(updateComLogBoxDelegate);
			 }
	//Serial port: error received	
	private: System::Void serialPort1_ErrorReceived(System::Object^  sender, System::IO::Ports::SerialErrorReceivedEventArgs^  e) {
					//commLogBox->AppendText("[SYSTEM]: Error data received"+ Environment::NewLine);
			 }
	private: System::Void speed57600Button_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void showStatsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
					//show stats dialog
					//showStatisticsDialog->ShowDialog(this);
				  this->label10->Visible=true;
			      this->groupBox3->Visible=true;
			 }

	private: System::Void DisconnectButton_Click_1(System::Object^  sender, System::EventArgs^  e) {
					serialPort1->Close();
					if (serialPort1->IsOpen!=true)
						commLogBox->AppendText("[SYSTEM]: Serial port is closed." + Environment::NewLine);
					
			 }
	private: System::Void sendModeButton_Click(System::Object^  sender, System::EventArgs^  e) {
					String^ singleChar;
					int i=0,j=0,k=0;
					int insertPosition=0;
					String^ xCoordString;
					String^ yCoordString;
					String^ xyCoordString;
					String^ xyObstacleString;

					if (serialPort1->IsOpen==false)
					{
						commLogBox->AppendText("[SYSTEM]: please open serial port first. " + Environment::NewLine);
					}
					else if (String::IsNullOrEmpty(modeMessage)==false)
					{
						//set robot´s initials pose
						mapDialog->setRobotPose(xRobot,yRobot,0);
						//set target position
						mapDialog->setTargetPosition(xTarget,yTarget);
						
						commLogBox->AppendText("[SYSTEM]: sending obstacles to robot... " + Environment::NewLine);
						//scan map for obstacles, if there is an obstacle -> send coordinates to robot
						for (i=0; i<(mapDialog->mapXSize); i++)
						{
							for (j=0; j<(mapDialog->mapYSize); j++)
							{	
								if (mapDialog->globalMap[i,j]==253)
								{
									if (i<10)
									{
										xCoordString = "0" + Convert::ToString(i);
									}
									else
									{
										xCoordString = Convert::ToString(i);
									}
									if (j<10)
									{
										yCoordString = "0" + Convert::ToString(j);
									}
									else
									{
										yCoordString = Convert::ToString(j);
									}
									xyObstacleString = "bO" + xCoordString + yCoordString +"e";
									commLogBox->AppendText("[USER]: send msg "+ xyObstacleString + Environment::NewLine);
									for (k=0; k<xyObstacleString->Length; k++)
									{
										singleChar = xyObstacleString->Substring(k,1);
										serialPort1->Write(singleChar);
										//commLogBox->AppendText("[USER]: send char "+ singleChar + Environment::NewLine);
										Sleep(50);
									}
								}
							}
						}
						
						//only pathfinding mode needs a target->other mode -> set target outside map
						if (selectModeBox->SelectedItem!="Pathfinding")
						{
							xTarget=99;
							yTarget=99;
						}	
						//add the robot´s start position to the message
						if (xRobot<10)
						{
							xCoordString = "0" + Convert::ToString(xRobot); 
						}
						else
						{
							xCoordString = Convert::ToString(xRobot);
						}
						if (yRobot<10)
						{
							yCoordString = "0" + Convert::ToString(yRobot);
						}
						else
						{
							yCoordString = Convert::ToString(yRobot);
						}
						xyCoordString = xCoordString + yCoordString;
						//add the target coordinates to the message
						if (xTarget<10)
						{
							xCoordString = "0" +  Convert::ToString(xTarget);
						}
						else
						{
							xCoordString = Convert::ToString(xTarget);
						}
						if (yTarget<10)
						{
							yCoordString = "0" + Convert::ToString(yTarget);
						}
						else
						{
							yCoordString = Convert::ToString(yTarget);
						}
						xyCoordString = xyCoordString + xCoordString + yCoordString + "e";
						//insertPosition = modeMessage->IndexOf('e');
						//modeMessage = modeMessage->Insert( insertPosition,  );
						modeMessage = modeMessage->Replace("e", xyCoordString);

						commLogBox->AppendText("[SYSTEM]: sending positions to robot... " + Environment::NewLine);

						for (i=0; i<modeMessage->Length; i++)
						{
							singleChar = modeMessage->Substring(i,1);
							serialPort1->Write(singleChar);
							commLogBox->AppendText("[USER]: send char "+ singleChar + Environment::NewLine);
							Sleep(20);
						}
						mapDialog->Refresh();
						this->startModeButton->Enabled = false;
						this->xCoordBox->Enabled = false;
						this->yCoordBox->Enabled = false;
						this->label3->Enabled = false;
						this->label4->Enabled = false;
						this->xTargetCoordBox->Enabled = false;
						this->yTargetCoordBox->Enabled = false;
						this->label7->Enabled = false;
						this->label8->Enabled = false;
						
						
					
					}
					else
					{
						commLogBox->AppendText("[SYSTEM]: please select mode first " + Environment::NewLine);
					}

					//enable robot controls in manual mode
					if (selectModeBox->SelectedItem=="Manual" && serialPort1->IsOpen==true)
					{
						this->forwardButton->Visible = true;
						this->backwardButton->Visible = true;
						this->rightButton->Visible = true;
						this->leftButton->Visible = true;
						this->stopButton->Visible = true;
					}
					else
					{
						this->forwardButton->Visible = false;
						this->backwardButton->Visible = false;
						this->rightButton->Visible = false;
						this->leftButton->Visible = false;
						this->stopButton->Visible = false;
					}
					//lock mapForm for the user -> no more obstacles can be set.
					//this->Refresh();
					mapDialog->obstacleSetFlag=0;
					mapDialog->Cursor = Cursors::No;
			 }

	private: System::Void label1_Click_2(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void selectModeBox_SelectedIndexChanged_1(System::Object^  sender, System::EventArgs^  e) {
					 
					 if (serialPort1->IsOpen==true)
					 {
						 if (selectModeBox->SelectedItem=="Idle")
						 {
							 modeMessage="bCIe";
							 this->xCoordBox->Enabled = true;
							 this->yCoordBox->Enabled = true;
							 this->label3->Enabled = true;
							 this->label4->Enabled = true;
							 this->xTargetCoordBox->Enabled = false;
							 this->yTargetCoordBox->Enabled = false;
							 this->label7->Enabled = false;
							 this->label8->Enabled = false;
							 //this->startModeButton->Enabled = true;
						 }
						 else if (selectModeBox->SelectedItem=="Manual")
						 {
							 modeMessage="bCMe";
							 this->xCoordBox->Enabled = true;
							 this->yCoordBox->Enabled = true;
							 this->label3->Enabled = true;
							 this->label4->Enabled = true;
							 this->xTargetCoordBox->Enabled = false;
							 this->yTargetCoordBox->Enabled = false;
							 this->label7->Enabled = false;
							 this->label8->Enabled = false;
						 }
						 else if (selectModeBox->SelectedItem=="Pathfinding")
						 {
							 modeMessage="bCPe";
							 this->xCoordBox->Enabled = true;
							 this->yCoordBox->Enabled = true;
							 this->label3->Enabled = true;
							 this->label4->Enabled = true;
							 this->xTargetCoordBox->Enabled = true;
							 this->yTargetCoordBox->Enabled = true;
							 this->label7->Enabled = true;
							 this->label8->Enabled = true;

						 }
						 commLogBox->AppendText("[SYSTEM]: enter or update the start position." + Environment::NewLine);
					 }	
					 else
					 {
						 modeMessage="";
						 this->xCoordBox->Enabled = true;
						 this->yCoordBox->Enabled = true;
						 this->label3->Enabled = true;
						 this->label4->Enabled = true;
						 if (selectModeBox->SelectedItem=="Pathfinding")
						 {
							this->xTargetCoordBox->Enabled = true;
							this->yTargetCoordBox->Enabled = true;
							this->label7->Enabled = true;
							this->label8->Enabled = true;
						 }
						 else
						 {
							this->xTargetCoordBox->Enabled = false;
							this->yTargetCoordBox->Enabled = false;
							this->label7->Enabled = false;
							this->label8->Enabled = false;
						 }

					 }
			 }
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
					commLogBox->Clear();
			 }



private: System::Void xCoord_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 
			 //check user input for correctness
			 //input must be:
			 // - number 0 -> 69
			 String^ userInput;
			 userInput = xCoordBox->Text;
			 try
			 {
				xRobot = Convert::ToInt32(userInput);
                if (xRobot<1 || xRobot>68)
				{
					commLogBox->AppendText("[SYSTEM]: wrong value for X coordinate." + Environment::NewLine);
					commLogBox->AppendText("[SYSTEM]: must be a number 1->68." + Environment::NewLine);	
					this->startModeButton->Enabled = false;
				}
			 }
			 catch (Exception^)
			 {
				commLogBox->AppendText("[SYSTEM]: wrong value for X coordinate." + Environment::NewLine);
				commLogBox->AppendText("[SYSTEM]: must be a number 1->68." + Environment::NewLine);
				this->startModeButton->Enabled = false;
				xRobot=99;
			 }

			 if (selectModeBox->SelectedItem=="Pathfinding")
			 {
				 if (xRobot>0 && xRobot<69 && yRobot>0 && yRobot<69 && xTarget>0 && xTarget<69 && yTarget>0 && yTarget<69)
				 {	
					this->startModeButton->Enabled = true;
				 }
			 }
			 else if (xRobot>0 && xRobot<69 && yRobot>0 && yRobot<69)
			 {	
				this->startModeButton->Enabled = true;
			 }

			 
		 }
private: System::Void yCoordBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 //check user input for correctness
			 //input must be:
			 // - number 0 -> 69
			 String^ userInput;
			 userInput = yCoordBox->Text;
			 try
			 {
				yRobot = Convert::ToInt32(userInput);
				if (yRobot<1 || yRobot>68)
				{
					commLogBox->AppendText("[SYSTEM]: wrong value for Y coordinate." + Environment::NewLine);
					commLogBox->AppendText("[SYSTEM]: must be a number 1->68." + Environment::NewLine);	
					this->startModeButton->Enabled = false;
				}
			 }
			 catch (Exception^)
			 {
				commLogBox->AppendText("[SYSTEM]: wrong value for Y coordinate." + Environment::NewLine);
				commLogBox->AppendText("[SYSTEM]: must be a number 1->68." + Environment::NewLine);
				this->startModeButton->Enabled = false;
				yRobot=99;
			 }

			 
			 if (selectModeBox->SelectedItem=="Pathfinding")
			 {
				 if (xRobot>0 && xRobot<69 && yRobot>0 && yRobot<69 && xTarget>0 && xTarget<69 && yTarget>0 && yTarget<69)
				 {	
					this->startModeButton->Enabled = true;
				 }
			 }
			 else if (xRobot>0 && xRobot<69 && yRobot>0 && yRobot<69)
			 {	
				this->startModeButton->Enabled = true;
			 }

		 }

private: System::Void mapClear_Click_1(System::Object^  sender, System::EventArgs^  e) {
			 mapDialog->startFlag=1;
			 int i=0, j=0;

			 for (i=0; i<mapDialog->mapXSize; i++)
			 {
				 for (j=0; j<mapDialog->mapYSize; j++)
				 {
					//if (mapDialog->globalMap[i,j]==
					mapDialog->globalMap[i,j]=0;
					mapDialog->tempGlobalMap[i,j]=0;
				 }
		     }
			 
			 mapDialog->Refresh();
			 //later -> send message to robot to delete all obstacles on its internal map
		 }
private: System::Void xTargetCoordBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 //check user input for correctness
			 //input must be:
			 // - number 0 -> 69
			 String^ userInput;
			 userInput = xTargetCoordBox->Text;
			 try
			 {
				xTarget = Convert::ToInt32(userInput);
                if (xTarget<1 || xTarget>68)
				{
					commLogBox->AppendText("[SYSTEM]: wrong value for target X coordinate." + Environment::NewLine);
					commLogBox->AppendText("[SYSTEM]: must be a number 1->68." + Environment::NewLine);	
					this->startModeButton->Enabled = false;
				}
			 }
			 catch (Exception^)
			 {
				commLogBox->AppendText("[SYSTEM]: wrong value for target X coordinate." + Environment::NewLine);
				commLogBox->AppendText("[SYSTEM]: must be a number 1->68." + Environment::NewLine);
				this->startModeButton->Enabled = false;
				xTarget=99;
			 }

			 if (selectModeBox->SelectedItem=="Pathfinding")
			 {
				 if (xRobot>0 && xRobot<69 && yRobot>0 && yRobot<69 && xTarget>0 && xTarget<69 && yTarget>0 && yTarget<69)
				 {	
					this->startModeButton->Enabled = true;
				 }
			 }
			 else if (xRobot>0 && xRobot<69 && yRobot>0 && yRobot<69)
			 {	
				this->startModeButton->Enabled = true;
			 }
		 }
private: System::Void yTargetCoordBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 //check user input for correctness
			 //input must be:
			 // - number 0 -> 69
			 String^ userInput;
			 userInput = yTargetCoordBox->Text;
			 try
			 {
				yTarget = Convert::ToInt32(userInput);
                if (yTarget<1 || yTarget>68)
				{
					commLogBox->AppendText("[SYSTEM]: wrong value for target Y coordinate." + Environment::NewLine);
					commLogBox->AppendText("[SYSTEM]: must be a number 1->68." + Environment::NewLine);	
					this->startModeButton->Enabled = false;
				}
			 }
			 catch (Exception^)
			 {
				commLogBox->AppendText("[SYSTEM]: wrong value for target Y coordinate." + Environment::NewLine);
				commLogBox->AppendText("[SYSTEM]: must be a number 1->68." + Environment::NewLine);
				this->startModeButton->Enabled = false;
				yTarget=99;
			 }

			 if (selectModeBox->SelectedItem=="Pathfinding")
			 {
				 if (xRobot>0 && xRobot<69 && yRobot>0 && yRobot<69 && xTarget>0 && xTarget<69 && yTarget>0 && yTarget<69)
				 {	
					this->startModeButton->Enabled = true;
				 }
			 }
			 else if (xRobot>0 && xRobot<=69 && yRobot>0 && yRobot<69)
			 {	
				this->startModeButton->Enabled = true;
			 }
		 }

private: System::Void forwardButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 
			 String^ singleChar;
			 int i=0;
			 String^ commandMessage;

			 commandMessage="bCFe";
				
			 for (i=0; i<commandMessage->Length; i++)
			 {
				singleChar = commandMessage->Substring(i,1);
				serialPort1->Write(singleChar);	
				Sleep(20);
			 }
			 commLogBox->AppendText("[USER]: send move forward command" + Environment::NewLine);
		 }
private: System::Void backwardButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 
			 String^ singleChar;
			 int i=0;
			 String^ commandMessage;

			 commandMessage="bCBe";

			 for (i=0; i<commandMessage->Length; i++)
			 {
				singleChar = commandMessage->Substring(i,1);
				serialPort1->Write(singleChar);
				Sleep(20);
			 }
			 commLogBox->AppendText("[USER]: send move backward command" + Environment::NewLine);
		 }
private: System::Void rightButton_Click(System::Object^  sender, System::EventArgs^  e) {
			
			 String^ singleChar;
			 int i=0;
			 String^ commandMessage;

			 commandMessage="bCRe";

			 for (i=0; i<commandMessage->Length; i++)
			 {
				singleChar = commandMessage->Substring(i,1);
				serialPort1->Write(singleChar);
				Sleep(20);
			 }
			 commLogBox->AppendText("[USER]: send rotate clockwise command" + Environment::NewLine);
		 }
private: System::Void leftButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 
			 String^ singleChar;
			 int i=0;
			 String^ commandMessage;

			 commandMessage="bCLe";

			 for (i=0; i<commandMessage->Length; i++)
			 {
				singleChar = commandMessage->Substring(i,1);
				serialPort1->Write(singleChar);
				Sleep(20);
			 }
			 commLogBox->AppendText("[USER]: send rotate anti-clockwise command" + Environment::NewLine);
		 }
private: System::Void stopButton_Click(System::Object^  sender, System::EventArgs^  e) {

			 String^ singleChar;
			 int i=0;
			 String^ commandMessage;

			 commandMessage="bCSe";

			 for (i=0; i<commandMessage->Length; i++)
			 {
				singleChar = commandMessage->Substring(i,1);
				serialPort1->Write(singleChar);
				Sleep(20);
			 }
			 commLogBox->AppendText("[USER]: send stop command" + Environment::NewLine);
		 }




private: System::Void standardViewToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

			 this->label10->Visible=false;
			 this->groupBox3->Visible=false;
		 }
private: System::Void controlPBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {

			 //check user input for correctness
			 //input must be:
			 // - number 0 -> 20
			 String^ userInput;

			 userInput = controlPBox->Text;
			 try
			 {
				controlPValue = Convert::ToInt32(userInput);
				if (controlPValue<0 || controlPValue>20)
				{
					commLogBox->AppendText("[SYSTEM]: wrong value for control variable." + Environment::NewLine);
					commLogBox->AppendText("[SYSTEM]: must be a number 0->20." + Environment::NewLine);	
					this->updateControlVariablesButton->Enabled = false;
				}
			 }
			 catch (Exception^)
			 {
				commLogBox->AppendText("[SYSTEM]: wrong value for control variable." + Environment::NewLine);
				commLogBox->AppendText("[SYSTEM]: must be a number 0->20." + Environment::NewLine);
				this->updateControlVariablesButton->Enabled = false;
				controlPValue=0;
			 }

			 if (controlPValue>=0 && controlPValue<=20 && controlIValue>=0 && controlIValue<=20 && controlDValue>=0 && controlDValue<=20)
			 {	
				this->updateControlVariablesButton->Enabled = true;
			 }			 
		 }
private: System::Void controlIBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {

			 //check user input for correctness
			 //input must be:
			 // - number 0 -> 20
			 String^ userInput;

			 userInput = controlIBox->Text;
			 try
			 {
				controlIValue = Convert::ToInt32(userInput);
				if (controlIValue<0 || controlIValue>20)
				{
					commLogBox->AppendText("[SYSTEM]: wrong value for control variable." + Environment::NewLine);
					commLogBox->AppendText("[SYSTEM]: must be a number 0->20." + Environment::NewLine);	
					this->updateControlVariablesButton->Enabled = false;
				}
			 }
			 catch (Exception^)
			 {
				commLogBox->AppendText("[SYSTEM]: wrong value for control variable." + Environment::NewLine);
				commLogBox->AppendText("[SYSTEM]: must be a number 0->20." + Environment::NewLine);
				this->updateControlVariablesButton->Enabled = false;
				controlIValue=0;
			 }

			 if (controlPValue>=0 && controlPValue<=20 && controlIValue>=0 && controlIValue<=20 && controlDValue>=0 && controlDValue<=20)
			 {	
				this->updateControlVariablesButton->Enabled = true;
			 }	
		 }
private: System::Void controlDBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {

			 //check user input for correctness
			 //input must be:
			 // - number 0 -> 20
			 String^ userInput;

			 userInput = controlDBox->Text;
			 try
			 {
				controlDValue = Convert::ToInt32(userInput);
				if (controlDValue<0 || controlDValue>20)
				{
					commLogBox->AppendText("[SYSTEM]: wrong value for control variable." + Environment::NewLine);
					commLogBox->AppendText("[SYSTEM]: must be a number 0->20." + Environment::NewLine);	
					this->updateControlVariablesButton->Enabled = false;
				}
			 }
			 catch (Exception^)
			 {
				commLogBox->AppendText("[SYSTEM]: wrong value for control variable." + Environment::NewLine);
				commLogBox->AppendText("[SYSTEM]: must be a number 0->20." + Environment::NewLine);
				this->updateControlVariablesButton->Enabled = false;
				controlDValue=0;
			 }

			 if (controlPValue>=0 && controlPValue<=20 && controlIValue>=0 && controlIValue<=20 && controlDValue>=0 && controlDValue<=20)
			 {	
				this->updateControlVariablesButton->Enabled = true;
			 }	
		 }
private: System::Void updateControlVariablesButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 
			 String^ singleChar;
			 int i=0;
			 String^ parameterMessage;
			 String^ parameterString;
			 String^ parameterPString;
			 String^ parameterIString;
			 String^ parameterDString;


			 if (selectControllerBox->SelectedItem=="Drive")
			 {
				parameterMessage="bUDe";
			 }
			 else if (selectControllerBox->SelectedItem=="WallFollowing")
			 {
				parameterMessage="bUSe";
			 }
			 

			 //if parameters are <0 insert a 0 in front of the number
			 if (controlPValue<10)
			 {
				parameterPString = "0" + Convert::ToString(controlPValue);
			 }
			 else
			 {
				parameterPString = Convert::ToString(controlPValue);
			 }

			 if (controlIValue<10)
			 {
				parameterIString = "0" + Convert::ToString(controlIValue);
			 }
			 else
			 {
				parameterIString = Convert::ToString(controlIValue);
			 }

			 if (controlDValue<10)
			 {
				parameterDString = "0" + Convert::ToString(controlDValue);
			 }
			 else
			 {
				parameterDString = Convert::ToString(controlDValue);
			 }				 

			 parameterString = parameterPString + parameterIString + parameterDString + "e";

			 parameterMessage = parameterMessage->Replace("e", parameterString);


			 for (i=0; i<parameterMessage->Length; i++)
			 {
				singleChar = parameterMessage->Substring(i,1);
				serialPort1->Write(singleChar);
				Sleep(20);
			 }
			 commLogBox->AppendText("[USER]: send control parameters to robot" + Environment::NewLine);

		 }
private: System::Void selectControllerBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 
			 
		 }

private: System::Void readBaudRateToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			//check if serial port is already open 
			if (serialPort1->IsOpen==true)
			{
				commLogBox->AppendText("[SYSTEM]: requesting baud rate..." + Environment::NewLine);
				Sleep(1100);
				//enter Xbee command mode (observe guard times before and afterwards)
				serialPort1->Write("+++");
				Sleep(1100);
				//read current baud rate of Xbee module
				serialPort1->Write("ATBD\r");

		
				//Sleep(600);
			}
			else
			{
				MessageBox::Show(L"Please open serial connection to Xbee device first.");
			}
		 }
private: System::Void ChangeBaudRateMenuItem57600_Click(System::Object^  sender, System::EventArgs^  e) {
			//String^ baudChangeMessage = "bB5e";
			//String^ singleChar;
			//int i=0;
			
			//-> not necessary to send a message to robot because the command to change the baud rate is also sent
			//   to all other xbee modules that are currently online

			//check if serial port is already open 
				
			if (serialPort1->IsOpen==true)
			{
				/*
				//tell robot to change its Xbee baud rate to 57600
				commLogBox->AppendText("[SYSTEM]: requesting baud rate change to 57600 from robot..." + Environment::NewLine);
				//send message to robot
				for (i=0; i<baudChangeMessage->Length; i++)
				{
					 singleChar = baudChangeMessage->Substring(i,1);
					 serialPort1->Write(singleChar);
					 Sleep(20);
				}
				*/
				//change baud rate of host XBee module
				commLogBox->AppendText("[SYSTEM]: setting baud rate to 57600..." + Environment::NewLine);
				Sleep(1100);
				serialPort1->Write("+++");
				Sleep(1100);
				serialPort1->Write("ATBD6\r");		
				Sleep(200);
				serialPort1->BaudRate = 57600;
				speed9600Button->Checked = false;
				speed57600Button->Checked = true;
				serialPort1->Write("ATCN\r");	

				Sleep(500);
				//check if communication with new baud rate is possible
				sendPingMsg();
			}
			else
			{
				MessageBox::Show(L"Please open serial connection to Xbee device first.");
			}
		 }

private: System::Void changeBaudRateMenuItem9600_Click(System::Object^  sender, System::EventArgs^  e) {
			//String^ baudChangeMessage = "bB9e";
			//String^ singleChar;
			//int i=0;
			
			
			//-> not necessary to send a message to robot because the command to change the baud rate is also sent
			//   to all other xbee modules that are currently online
			 			    
			//check if serial port is already open 
			if (serialPort1->IsOpen==true)
			{
				/*
					//tell robot to change its Xbee baud rate to 57600
				commLogBox->AppendText("[SYSTEM]: requesting baud rate change to 9600 from robot..." + Environment::NewLine);
				//send message to robot
				for (i=0; i<baudChangeMessage->Length; i++)
				{
					 singleChar = baudChangeMessage->Substring(i,1);
					 serialPort1->Write(singleChar);
					 Sleep(20);
				}
				*/

				//change baud rate of host XBee module
				commLogBox->AppendText("[SYSTEM]: setting baud rate to 9600..." + Environment::NewLine);
				Sleep(1100);
				serialPort1->Write("+++");
				Sleep(1100);
				serialPort1->Write("ATBD3\r");		
				Sleep(200);
				serialPort1->BaudRate = 9600;
				speed9600Button->Checked = true;
				speed57600Button->Checked = false;
				//exit command mode
				serialPort1->Write("ATCN\r");						
				
				Sleep(500);
				//check if communication with new baud rate is possible
				sendPingMsg();
			}
			else
			{
				MessageBox::Show(L"Please open serial connection to Xbee device first.");
			}
		 }

private: System::Void checkConnectionMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			//check if serial port is already open 
			if (serialPort1->IsOpen==true)
			{
				sendPingMsg();
			}
			else
			{
				MessageBox::Show(L"Please open serial connection to Xbee device first.");
			}
		 }
};
}

