#pragma once

#include <windows.h>

#define MAX_OUTPUT_VOLTAGE	17000
#define MIN_OUTPUT_VOLTAGE	7000


namespace robokid_control {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung für toeControl
	/// </summary>
	public ref class toeControl : public System::Windows::Forms::Form
	{
	public:
		toeControl(void)
		{
			InitializeComponent();
			//
			//TODO: Konstruktorcode hier hinzufügen.
			//

			//set start position of toe control form
			this->Location = Point(653,100);

			//initialize the setup text boxes on the form
			toeVoltageStartTextBox->Text = "7000";
			toeVoltageEndTextBox->Text = "17000";
			toeVoltageStepTextBox->Text = "1000";
			toeIterationStepTextBox->Text = "1";
			toeSerialPortStatusTextBox->Text = "Closed";
			//also initialize the corresponding variables
			voltageStartValue = 7000;
			voltageEndValue = 17000;
			voltageStepValue = 1000;
			numberIterationsVoltageSwitch = 1;
			toeCurrentVoltageTextBox->Text = "??";

			//at class generation the power control is deactivated
			powerControlActivated = false;
			

			//setup serial port
			toeSerialPort1->BaudRate = 38400;
			toeSelectPortBox->SelectedIndex=0;




		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~toeControl()
		{
			//set power supply to local control
			if (toeSerialPort1->IsOpen==true)
			{
				toeSerialPort1->Write(":SYST:LOC\n");
			}
			toeSerialPort1->Close();


			if (components)
			{
				delete components;
			}
		}


	//member variables:
	private:
		int voltageStartValue;
		int voltageEndValue;
		int voltageStepValue;
		int numberIterationsVoltageSwitch;

	private:
		int currentOutputVoltage;
	
	private:
		bool powerControlActivated;



	private: System::Windows::Forms::Button^  toeDisconnectSerialButton;
	protected: 
	private: System::Windows::Forms::Label^  toeSelectPortLabel;
	private: System::Windows::Forms::ComboBox^  toeSelectPortBox;
	private: System::Windows::Forms::GroupBox^  toeSpeedGroupBox;
	private: System::Windows::Forms::RadioButton^  toeSpeed38400Button;

	private: System::Windows::Forms::RadioButton^  towSpeed9600Button;

	private: System::Windows::Forms::Label^  toeSerialConnectionLabel;
	private: System::Windows::Forms::Button^  toeConnectSerialButton;
	private: System::Windows::Forms::GroupBox^  toeVoltageSetupGroupBox;
	private: System::Windows::Forms::Label^  toeVoltageSetupLabel;
	private: System::Windows::Forms::Button^  toeActivateButton;
	private: System::Windows::Forms::Label^  toeIterationStepLabel;
	private: System::Windows::Forms::TextBox^  toeIterationStepTextBox;
	private: System::Windows::Forms::Label^  toeVoltageStepLabel;
	private: System::Windows::Forms::TextBox^  toeVoltageStepTextBox;
	private: System::Windows::Forms::Label^  toeVoltageEndLabel;
	private: System::Windows::Forms::TextBox^  toeVoltageEndTextBox;
	private: System::Windows::Forms::Label^  toeVoltageStartLabel;
	private: System::Windows::Forms::TextBox^  toeVoltageStartTextBox;
	private: System::IO::Ports::SerialPort^  toeSerialPort1;
	private: System::Windows::Forms::Label^  toeSerialPortStatusLabel;
	private: System::Windows::Forms::TextBox^  toeSerialPortStatusTextBox;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  toeVoltageStartRestrictionsLabel;
	private: System::Windows::Forms::TextBox^  toeCurrentVoltageTextBox;
	private: System::Windows::Forms::Label^  toeCurrentVoltageLabel;

	private: System::ComponentModel::IContainer^  components;



	public:

		//***********************************************************************************************************
		// void nextIterationNotify(int currentIteration)
		// -----------------------------------------------
		// Aim of this member function is to notify the toellner power supply control modul that the main form 
		// has finished the current classifiction iteration.
		//
		// The toe module increases or decreases the voltage step after the number of iterations, set in text box 
		// "toeIterationStepTextBox", has passed. 
		//
		// The voltage is only updated if the control button is enabled.
		//
		//***********************************************************************************************************
		void nextIterationNotify(int currentIteration)
		{
			//check if a current step interval has passed.
			if (currentIteration % numberIterationsVoltageSwitch == 0 &&
				toeActivateButton->Text == "Enabled")
			{
				//activate next voltage step
				if (voltageStartValue < voltageEndValue)
				{
					//next step -> + step value
					currentOutputVoltage += voltageStepValue; 
				}
				else if (voltageStartValue > voltageEndValue)
				{
					//next step -> - step value
					currentOutputVoltage -= voltageStepValue;
				}

				//check output voltage boundaries
				if (currentOutputVoltage > MAX_OUTPUT_VOLTAGE)
				{
					currentOutputVoltage = MAX_OUTPUT_VOLTAGE;
				}
				else if (currentOutputVoltage < MIN_OUTPUT_VOLTAGE)
				{
					currentOutputVoltage = MIN_OUTPUT_VOLTAGE;
				}

				//update power supply voltage
				setVoltage(currentOutputVoltage);
			}
		}


		//***********************************************************************************************************
		// void setVoltage()
		// -----------------------------------------	
		// Aim of this member function is to set the output voltage of the Toellner 8872 power supply unit.
		//
		// Command to set the voltage is transmitted by serial interface, as defined in the com settings of this module.
		//
		// The output voltage has to be inside the hard boundaries of [7V ... 17V], specified for AAG ECU.
		//
		//***********************************************************************************************************
	private:	
		void setVoltage(int setVoltage)
		{
			int output_V;
			int output_mV;

			String^ commandMsg;
		
			
			//voltage boundary check
			if (setVoltage < 7000)
			{
				setVoltage = 7000;
			}
			else if (setVoltage > 17000)
			{
				setVoltage = 17000;
			}

			
			//divide the voltage into V and mV numbers
			output_V  = setVoltage / 1000;
			output_mV = setVoltage % 1000;

			//send voltage setting command to power supply unit
			commandMsg = "V " + Convert::ToString(output_V) + "." + Convert::ToString(output_mV) + "\n";
			if (toeSerialPort1->IsOpen==true)
			{
				toeSerialPort1->Write(commandMsg);
			}

			//send output = ON command to power supply unit
			commandMsg = "EX ON\n";
			if (toeSerialPort1->IsOpen==true)
			{
				toeSerialPort1->Write(commandMsg);
			}

			toeCurrentVoltageTextBox->Text = Convert::ToString(setVoltage);

		}

	public:
		void initToeModule()
		{
			if (toeActivateButton->Text == "Enabled")
			{
				try
				{
					currentOutputVoltage = Convert::ToInt32(toeVoltageStartTextBox->Text);
					toeCurrentVoltageTextBox->Text = toeVoltageStartTextBox->Text;
					setVoltage(currentOutputVoltage);
				}
				catch (Exception ^)
				{
					currentOutputVoltage = 0;
					toeCurrentVoltageTextBox->Text = "0";
				}
			}
		}

	public:
		int getCurrentOutputVoltage()
		{
			int retValue = 0;
			if (toeActivateButton->Text == "Enabled")
			{
				try
				{
					retValue = Convert::ToInt32(toeCurrentVoltageTextBox->Text);
				}
				catch (Exception ^)
				{
					retValue = 0;
				}
			}
			return retValue;
		}



	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->toeDisconnectSerialButton = (gcnew System::Windows::Forms::Button());
			this->toeSelectPortLabel = (gcnew System::Windows::Forms::Label());
			this->toeSelectPortBox = (gcnew System::Windows::Forms::ComboBox());
			this->toeSpeedGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->toeSpeed38400Button = (gcnew System::Windows::Forms::RadioButton());
			this->towSpeed9600Button = (gcnew System::Windows::Forms::RadioButton());
			this->toeSerialConnectionLabel = (gcnew System::Windows::Forms::Label());
			this->toeConnectSerialButton = (gcnew System::Windows::Forms::Button());
			this->toeVoltageSetupGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->toeCurrentVoltageTextBox = (gcnew System::Windows::Forms::TextBox());
			this->toeCurrentVoltageLabel = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->toeVoltageStartRestrictionsLabel = (gcnew System::Windows::Forms::Label());
			this->toeActivateButton = (gcnew System::Windows::Forms::Button());
			this->toeIterationStepLabel = (gcnew System::Windows::Forms::Label());
			this->toeIterationStepTextBox = (gcnew System::Windows::Forms::TextBox());
			this->toeVoltageStepLabel = (gcnew System::Windows::Forms::Label());
			this->toeVoltageStepTextBox = (gcnew System::Windows::Forms::TextBox());
			this->toeVoltageEndLabel = (gcnew System::Windows::Forms::Label());
			this->toeVoltageEndTextBox = (gcnew System::Windows::Forms::TextBox());
			this->toeVoltageStartLabel = (gcnew System::Windows::Forms::Label());
			this->toeVoltageStartTextBox = (gcnew System::Windows::Forms::TextBox());
			this->toeVoltageSetupLabel = (gcnew System::Windows::Forms::Label());
			this->toeSerialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->toeSerialPortStatusLabel = (gcnew System::Windows::Forms::Label());
			this->toeSerialPortStatusTextBox = (gcnew System::Windows::Forms::TextBox());
			this->toeSpeedGroupBox->SuspendLayout();
			this->toeVoltageSetupGroupBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// toeDisconnectSerialButton
			// 
			this->toeDisconnectSerialButton->Location = System::Drawing::Point(158, 252);
			this->toeDisconnectSerialButton->Name = L"toeDisconnectSerialButton";
			this->toeDisconnectSerialButton->Size = System::Drawing::Size(104, 39);
			this->toeDisconnectSerialButton->TabIndex = 16;
			this->toeDisconnectSerialButton->Text = L"Disconnect";
			this->toeDisconnectSerialButton->UseVisualStyleBackColor = true;
			this->toeDisconnectSerialButton->Click += gcnew System::EventHandler(this, &toeControl::toeDisconnectSerialButton_Click);
			// 
			// toeSelectPortLabel
			// 
			this->toeSelectPortLabel->AutoSize = true;
			this->toeSelectPortLabel->Location = System::Drawing::Point(25, 68);
			this->toeSelectPortLabel->Name = L"toeSelectPortLabel";
			this->toeSelectPortLabel->Size = System::Drawing::Size(89, 13);
			this->toeSelectPortLabel->TabIndex = 15;
			this->toeSelectPortLabel->Text = L"Select COM-Port:";
			// 
			// toeSelectPortBox
			// 
			this->toeSelectPortBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->toeSelectPortBox->FormattingEnabled = true;
			this->toeSelectPortBox->Items->AddRange(gcnew cli::array< System::Object^  >(8) {L"COM1", L"COM2", L"COM3", L"COM4", L"COM5", 
				L"COM6", L"COM7", L"COM8"});
			this->toeSelectPortBox->Location = System::Drawing::Point(28, 84);
			this->toeSelectPortBox->Name = L"toeSelectPortBox";
			this->toeSelectPortBox->Size = System::Drawing::Size(121, 21);
			this->toeSelectPortBox->TabIndex = 14;
			this->toeSelectPortBox->SelectedIndexChanged += gcnew System::EventHandler(this, &toeControl::toeSelectPortBox_SelectedIndexChanged);
			// 
			// toeSpeedGroupBox
			// 
			this->toeSpeedGroupBox->Controls->Add(this->toeSpeed38400Button);
			this->toeSpeedGroupBox->Controls->Add(this->towSpeed9600Button);
			this->toeSpeedGroupBox->Location = System::Drawing::Point(28, 131);
			this->toeSpeedGroupBox->Name = L"toeSpeedGroupBox";
			this->toeSpeedGroupBox->Size = System::Drawing::Size(234, 101);
			this->toeSpeedGroupBox->TabIndex = 13;
			this->toeSpeedGroupBox->TabStop = false;
			this->toeSpeedGroupBox->Text = L"Speed (baud)";
			// 
			// toeSpeed38400Button
			// 
			this->toeSpeed38400Button->AutoSize = true;
			this->toeSpeed38400Button->Checked = true;
			this->toeSpeed38400Button->Enabled = false;
			this->toeSpeed38400Button->Location = System::Drawing::Point(26, 71);
			this->toeSpeed38400Button->Name = L"toeSpeed38400Button";
			this->toeSpeed38400Button->Size = System::Drawing::Size(55, 17);
			this->toeSpeed38400Button->TabIndex = 1;
			this->toeSpeed38400Button->TabStop = true;
			this->toeSpeed38400Button->Text = L"38400";
			this->toeSpeed38400Button->UseVisualStyleBackColor = true;
			// 
			// towSpeed9600Button
			// 
			this->towSpeed9600Button->AutoSize = true;
			this->towSpeed9600Button->Enabled = false;
			this->towSpeed9600Button->Location = System::Drawing::Point(26, 33);
			this->towSpeed9600Button->Name = L"towSpeed9600Button";
			this->towSpeed9600Button->Size = System::Drawing::Size(49, 17);
			this->towSpeed9600Button->TabIndex = 0;
			this->towSpeed9600Button->TabStop = true;
			this->towSpeed9600Button->Text = L"9600";
			this->towSpeed9600Button->UseVisualStyleBackColor = true;
			// 
			// toeSerialConnectionLabel
			// 
			this->toeSerialConnectionLabel->AutoSize = true;
			this->toeSerialConnectionLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->toeSerialConnectionLabel->Location = System::Drawing::Point(24, 37);
			this->toeSerialConnectionLabel->Name = L"toeSerialConnectionLabel";
			this->toeSerialConnectionLabel->Size = System::Drawing::Size(234, 20);
			this->toeSerialConnectionLabel->TabIndex = 12;
			this->toeSerialConnectionLabel->Text = L"Serial connection Power Control";
			// 
			// toeConnectSerialButton
			// 
			this->toeConnectSerialButton->Location = System::Drawing::Point(28, 252);
			this->toeConnectSerialButton->Name = L"toeConnectSerialButton";
			this->toeConnectSerialButton->Size = System::Drawing::Size(104, 38);
			this->toeConnectSerialButton->TabIndex = 11;
			this->toeConnectSerialButton->Text = L"Connect";
			this->toeConnectSerialButton->UseVisualStyleBackColor = true;
			this->toeConnectSerialButton->Click += gcnew System::EventHandler(this, &toeControl::toeConnectSerialButton_Click);
			// 
			// toeVoltageSetupGroupBox
			// 
			this->toeVoltageSetupGroupBox->BackColor = System::Drawing::Color::Gainsboro;
			this->toeVoltageSetupGroupBox->Controls->Add(this->toeCurrentVoltageTextBox);
			this->toeVoltageSetupGroupBox->Controls->Add(this->toeCurrentVoltageLabel);
			this->toeVoltageSetupGroupBox->Controls->Add(this->label1);
			this->toeVoltageSetupGroupBox->Controls->Add(this->toeVoltageStartRestrictionsLabel);
			this->toeVoltageSetupGroupBox->Controls->Add(this->toeActivateButton);
			this->toeVoltageSetupGroupBox->Controls->Add(this->toeIterationStepLabel);
			this->toeVoltageSetupGroupBox->Controls->Add(this->toeIterationStepTextBox);
			this->toeVoltageSetupGroupBox->Controls->Add(this->toeVoltageStepLabel);
			this->toeVoltageSetupGroupBox->Controls->Add(this->toeVoltageStepTextBox);
			this->toeVoltageSetupGroupBox->Controls->Add(this->toeVoltageEndLabel);
			this->toeVoltageSetupGroupBox->Controls->Add(this->toeVoltageEndTextBox);
			this->toeVoltageSetupGroupBox->Controls->Add(this->toeVoltageStartLabel);
			this->toeVoltageSetupGroupBox->Controls->Add(this->toeVoltageStartTextBox);
			this->toeVoltageSetupGroupBox->Controls->Add(this->toeVoltageSetupLabel);
			this->toeVoltageSetupGroupBox->Location = System::Drawing::Point(1, 310);
			this->toeVoltageSetupGroupBox->Name = L"toeVoltageSetupGroupBox";
			this->toeVoltageSetupGroupBox->Size = System::Drawing::Size(291, 361);
			this->toeVoltageSetupGroupBox->TabIndex = 17;
			this->toeVoltageSetupGroupBox->TabStop = false;
			// 
			// toeCurrentVoltageTextBox
			// 
			this->toeCurrentVoltageTextBox->Location = System::Drawing::Point(190, 299);
			this->toeCurrentVoltageTextBox->Name = L"toeCurrentVoltageTextBox";
			this->toeCurrentVoltageTextBox->Size = System::Drawing::Size(75, 20);
			this->toeCurrentVoltageTextBox->TabIndex = 29;
			// 
			// toeCurrentVoltageLabel
			// 
			this->toeCurrentVoltageLabel->AutoSize = true;
			this->toeCurrentVoltageLabel->Location = System::Drawing::Point(165, 277);
			this->toeCurrentVoltageLabel->Name = L"toeCurrentVoltageLabel";
			this->toeCurrentVoltageLabel->Size = System::Drawing::Size(104, 13);
			this->toeCurrentVoltageLabel->TabIndex = 28;
			this->toeCurrentVoltageLabel->Text = L"Output voltage [mV]:";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::SystemColors::ControlDark;
			this->label1->Location = System::Drawing::Point(17, 138);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(92, 13);
			this->label1->TabIndex = 27;
			this->label1->Text = L"Range: 7V -> 17V";
			// 
			// toeVoltageStartRestrictionsLabel
			// 
			this->toeVoltageStartRestrictionsLabel->AutoSize = true;
			this->toeVoltageStartRestrictionsLabel->ForeColor = System::Drawing::SystemColors::ControlDark;
			this->toeVoltageStartRestrictionsLabel->Location = System::Drawing::Point(17, 84);
			this->toeVoltageStartRestrictionsLabel->Name = L"toeVoltageStartRestrictionsLabel";
			this->toeVoltageStartRestrictionsLabel->Size = System::Drawing::Size(92, 13);
			this->toeVoltageStartRestrictionsLabel->TabIndex = 26;
			this->toeVoltageStartRestrictionsLabel->Text = L"Range: 7V -> 17V";
			// 
			// toeActivateButton
			// 
			this->toeActivateButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->toeActivateButton->ForeColor = System::Drawing::Color::Red;
			this->toeActivateButton->Location = System::Drawing::Point(20, 277);
			this->toeActivateButton->Name = L"toeActivateButton";
			this->toeActivateButton->Size = System::Drawing::Size(135, 62);
			this->toeActivateButton->TabIndex = 25;
			this->toeActivateButton->Text = L"Disabled";
			this->toeActivateButton->UseVisualStyleBackColor = true;
			this->toeActivateButton->Click += gcnew System::EventHandler(this, &toeControl::toeActivateButton_Click);
			// 
			// toeIterationStepLabel
			// 
			this->toeIterationStepLabel->AutoSize = true;
			this->toeIterationStepLabel->Location = System::Drawing::Point(17, 229);
			this->toeIterationStepLabel->Name = L"toeIterationStepLabel";
			this->toeIterationStepLabel->Size = System::Drawing::Size(110, 13);
			this->toeIterationStepLabel->TabIndex = 24;
			this->toeIterationStepLabel->Text = L"Step after \? iterations:";
			// 
			// toeIterationStepTextBox
			// 
			this->toeIterationStepTextBox->Location = System::Drawing::Point(157, 226);
			this->toeIterationStepTextBox->Name = L"toeIterationStepTextBox";
			this->toeIterationStepTextBox->Size = System::Drawing::Size(111, 20);
			this->toeIterationStepTextBox->TabIndex = 23;
			this->toeIterationStepTextBox->TextChanged += gcnew System::EventHandler(this, &toeControl::toeIterationStepTextBox_TextChanged);
			// 
			// toeVoltageStepLabel
			// 
			this->toeVoltageStepLabel->AutoSize = true;
			this->toeVoltageStepLabel->Location = System::Drawing::Point(17, 175);
			this->toeVoltageStepLabel->Name = L"toeVoltageStepLabel";
			this->toeVoltageStepLabel->Size = System::Drawing::Size(130, 13);
			this->toeVoltageStepLabel->TabIndex = 22;
			this->toeVoltageStepLabel->Text = L"Voltage STEP value [mV]:";
			// 
			// toeVoltageStepTextBox
			// 
			this->toeVoltageStepTextBox->Location = System::Drawing::Point(157, 172);
			this->toeVoltageStepTextBox->Name = L"toeVoltageStepTextBox";
			this->toeVoltageStepTextBox->Size = System::Drawing::Size(111, 20);
			this->toeVoltageStepTextBox->TabIndex = 21;
			this->toeVoltageStepTextBox->TextChanged += gcnew System::EventHandler(this, &toeControl::toeVoltageStepTextBox_TextChanged);
			// 
			// toeVoltageEndLabel
			// 
			this->toeVoltageEndLabel->AutoSize = true;
			this->toeVoltageEndLabel->Location = System::Drawing::Point(17, 120);
			this->toeVoltageEndLabel->Name = L"toeVoltageEndLabel";
			this->toeVoltageEndLabel->Size = System::Drawing::Size(125, 13);
			this->toeVoltageEndLabel->TabIndex = 20;
			this->toeVoltageEndLabel->Text = L"Voltage END value [mV]:";
			// 
			// toeVoltageEndTextBox
			// 
			this->toeVoltageEndTextBox->Location = System::Drawing::Point(157, 117);
			this->toeVoltageEndTextBox->Name = L"toeVoltageEndTextBox";
			this->toeVoltageEndTextBox->Size = System::Drawing::Size(111, 20);
			this->toeVoltageEndTextBox->TabIndex = 19;
			this->toeVoltageEndTextBox->TextChanged += gcnew System::EventHandler(this, &toeControl::toeVoltageEndTextBox_TextChanged);
			// 
			// toeVoltageStartLabel
			// 
			this->toeVoltageStartLabel->AutoSize = true;
			this->toeVoltageStartLabel->Location = System::Drawing::Point(17, 66);
			this->toeVoltageStartLabel->Name = L"toeVoltageStartLabel";
			this->toeVoltageStartLabel->Size = System::Drawing::Size(138, 13);
			this->toeVoltageStartLabel->TabIndex = 18;
			this->toeVoltageStartLabel->Text = L"Voltage START value [mV]:";
			// 
			// toeVoltageStartTextBox
			// 
			this->toeVoltageStartTextBox->Location = System::Drawing::Point(157, 63);
			this->toeVoltageStartTextBox->Name = L"toeVoltageStartTextBox";
			this->toeVoltageStartTextBox->Size = System::Drawing::Size(111, 20);
			this->toeVoltageStartTextBox->TabIndex = 17;
			this->toeVoltageStartTextBox->TextChanged += gcnew System::EventHandler(this, &toeControl::toeVoltageStartTextBox_TextChanged);
			// 
			// toeVoltageSetupLabel
			// 
			this->toeVoltageSetupLabel->AutoSize = true;
			this->toeVoltageSetupLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->toeVoltageSetupLabel->Location = System::Drawing::Point(16, 15);
			this->toeVoltageSetupLabel->Name = L"toeVoltageSetupLabel";
			this->toeVoltageSetupLabel->Size = System::Drawing::Size(108, 20);
			this->toeVoltageSetupLabel->TabIndex = 16;
			this->toeVoltageSetupLabel->Text = L"Voltage setup";
			// 
			// toeSerialPortStatusLabel
			// 
			this->toeSerialPortStatusLabel->AutoSize = true;
			this->toeSerialPortStatusLabel->Location = System::Drawing::Point(166, 68);
			this->toeSerialPortStatusLabel->Name = L"toeSerialPortStatusLabel";
			this->toeSerialPortStatusLabel->Size = System::Drawing::Size(40, 13);
			this->toeSerialPortStatusLabel->TabIndex = 18;
			this->toeSerialPortStatusLabel->Text = L"Status:";
			// 
			// toeSerialPortStatusTextBox
			// 
			this->toeSerialPortStatusTextBox->Location = System::Drawing::Point(169, 84);
			this->toeSerialPortStatusTextBox->Name = L"toeSerialPortStatusTextBox";
			this->toeSerialPortStatusTextBox->Size = System::Drawing::Size(93, 20);
			this->toeSerialPortStatusTextBox->TabIndex = 19;
			// 
			// toeControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 673);
			this->Controls->Add(this->toeSerialPortStatusTextBox);
			this->Controls->Add(this->toeSerialPortStatusLabel);
			this->Controls->Add(this->toeVoltageSetupGroupBox);
			this->Controls->Add(this->toeDisconnectSerialButton);
			this->Controls->Add(this->toeSelectPortLabel);
			this->Controls->Add(this->toeSelectPortBox);
			this->Controls->Add(this->toeSpeedGroupBox);
			this->Controls->Add(this->toeSerialConnectionLabel);
			this->Controls->Add(this->toeConnectSerialButton);
			this->Name = L"toeControl";
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Text = L"Toellner 8872 control";
			this->toeSpeedGroupBox->ResumeLayout(false);
			this->toeSpeedGroupBox->PerformLayout();
			this->toeVoltageSetupGroupBox->ResumeLayout(false);
			this->toeVoltageSetupGroupBox->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void toeVoltageStartTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {

			//check user input for correctness
			try
			{
				if (toeVoltageStartTextBox->Text != "")
				{
					voltageStartValue = Convert::ToInt32(toeVoltageStartTextBox->Text);	
				}

				//check if value is inside boundaries
				//if not -> lock enable button for power control
				if (voltageStartValue < 7000)
				{
					toeActivateButton->Enabled = false;
					toeVoltageStartTextBox->ForeColor = Color::Red;
				}
				else if (voltageStartValue > 17000)
				{
					toeActivateButton->Enabled = false;
					toeVoltageStartTextBox->ForeColor = Color::Red;
				}
				else
				{
					toeActivateButton->Enabled = true;
					toeVoltageStartTextBox->ForeColor = Color::Black;
				}
			}
			catch (Exception^)
			{
				toeVoltageStartTextBox->Text = "7000";
				voltageStartValue = 7000;
				toeActivateButton->Enabled = true;
				toeVoltageStartTextBox->ForeColor = Color::Black;
			}

			//also check if the end value is inside its boundaries
			if (voltageEndValue < 7000)
			{
				toeActivateButton->Enabled = false;
			}
			else if (voltageEndValue > 17000)
			{
				toeActivateButton->Enabled = false;
			}
		}
private: System::Void toeVoltageEndTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {

			//check user input for correctness
			try
			{
				if (toeVoltageEndTextBox->Text != "")
				{
					voltageEndValue = Convert::ToInt32(toeVoltageEndTextBox->Text);				
				}

				//check if value is inside boundaries
				if (voltageEndValue < 7000)
				{
					toeActivateButton->Enabled = false;
					toeVoltageEndTextBox->ForeColor = Color::Red;
				}
				else if (voltageEndValue > 17000)
				{
					toeActivateButton->Enabled = false;
					toeVoltageEndTextBox->ForeColor = Color::Red;
				}
				else
				{
					toeActivateButton->Enabled = true;
					toeVoltageEndTextBox->ForeColor = Color::Black;
				}
			}
			catch (Exception^)
			{
				toeVoltageEndTextBox->Text = "7000";
				voltageEndValue = 7000;
				toeActivateButton->Enabled = true;
				toeVoltageEndTextBox->ForeColor = Color::Black;
			}

			//also check if the start value is inside its boundaries
			if (voltageStartValue < 7000)
			{
				toeActivateButton->Enabled = false;
			}
			else if (voltageStartValue > 17000)
			{
				toeActivateButton->Enabled = false;
			}

		 }
private: System::Void toeVoltageStepTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {

			//check user input for correctness
			try
			{
				if (toeVoltageStepTextBox->Text != "")
				{
					voltageStepValue = Convert::ToInt32(toeVoltageStepTextBox->Text);				
				}
			}
			catch (Exception^)
			{
				toeVoltageStepTextBox->Text = "1000";
				voltageStepValue = 1000;
			}
		 }
private: System::Void toeIterationStepTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {

			//check user input for correctness
			try
			{
				if (toeIterationStepTextBox->Text != "")
				{
					numberIterationsVoltageSwitch = Convert::ToInt32(toeIterationStepTextBox->Text);				
				}
				if (numberIterationsVoltageSwitch == 0)
				{
					numberIterationsVoltageSwitch = 1;
					toeIterationStepTextBox->Text = "1";
				}
			}
			catch (Exception^)
			{
				toeIterationStepTextBox->Text = "1";
				numberIterationsVoltageSwitch = 1;
			}

		 }
private: System::Void toeActivateButton_Click(System::Object^  sender, System::EventArgs^  e) {

			 if (powerControlActivated == true)
			 {
				//disable the power control
				powerControlActivated = false;
				toeActivateButton->Text = "Disabled";
				toeActivateButton->ForeColor = Color::Red;
				//set power supply to local control
				if (toeSerialPort1->IsOpen==true)
				{
					toeSerialPort1->Write(":SYST:LOC\n");
				}


			 }
			 else if (powerControlActivated == false)
			 {
				//enable the power control
				powerControlActivated = true;
				toeActivateButton->Text = "Enabled";
				toeActivateButton->ForeColor = Color::Green;
				if (toeSerialPort1->IsOpen==true)
				{
					toeSerialPort1->Write(":SYST:REM\n");
				}
				setVoltage(currentOutputVoltage);
			 }
		 }
private: System::Void toeConnectSerialButton_Click(System::Object^  sender, System::EventArgs^  e) {

			try
			{
				toeSerialPort1->Open();
				if (toeSerialPort1->IsOpen==true)
				{
					toeSerialPort1->DiscardInBuffer();    // clear receive buffer
					toeSerialPort1->DiscardOutBuffer();   // clear transmit buffer
					toeSerialPortStatusTextBox->Text = "Open";
				}
			}
			//catch (IOException ^ e)
			//{
			//	toeSerialPortStatusTextBox->Text = "Invalid";
			//}
			catch (UnauthorizedAccessException ^)
			{
				toeSerialPortStatusTextBox->Text = "Access denied";
			}
			catch (InvalidOperationException ^)
			{
				toeSerialPortStatusTextBox->Text = "Already open";
			}
		}
private: System::Void toeSelectPortBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				
			if (toeSelectPortBox->SelectedItem=="COM1")
			{
				toeSerialPort1->PortName = L"COM1";
			}
			if (toeSelectPortBox->SelectedItem=="COM2")
			{
				toeSerialPort1->PortName = L"COM2";
			}
			if (toeSelectPortBox->SelectedItem=="COM3")
			{
				toeSerialPort1->PortName = L"COM3";
			}
			if (toeSelectPortBox->SelectedItem=="COM4")
			{
				toeSerialPort1->PortName = L"COM4";
			}
			if (toeSelectPortBox->SelectedItem=="COM5")
			{
				toeSerialPort1->PortName = L"COM5";
			}
			if (toeSelectPortBox->SelectedItem=="COM6")
			{
				toeSerialPort1->PortName = L"COM6";
			}
			if (toeSelectPortBox->SelectedItem=="COM7")
			{
				toeSerialPort1->PortName = L"COM7";
			}
		 }
private: System::Void toeDisconnectSerialButton_Click(System::Object^  sender, System::EventArgs^  e) {

			toeSerialPort1->Close();
			toeSerialPortStatusTextBox->Text = "Closed";
		 }
};
}
