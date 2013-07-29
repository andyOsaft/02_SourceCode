#pragma once

namespace robokid_control {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung für showStatisticsForm
	/// </summary>
	public ref class showStatisticsForm : public System::Windows::Forms::Form
	{
	public:
		showStatisticsForm(void)
		{
			InitializeComponent();
			//
			//TODO: Konstruktorcode hier hinzufügen.
			//
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~showStatisticsForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  closeStatsButton;
	private: System::Windows::Forms::TextBox^  statsTextBox;
	private: System::Windows::Forms::Label^  statsLabel;
	protected: 

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->closeStatsButton = (gcnew System::Windows::Forms::Button());
			this->statsTextBox = (gcnew System::Windows::Forms::TextBox());
			this->statsLabel = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// closeStatsButton
			// 
			this->closeStatsButton->Location = System::Drawing::Point(140, 291);
			this->closeStatsButton->Name = L"closeStatsButton";
			this->closeStatsButton->Size = System::Drawing::Size(75, 23);
			this->closeStatsButton->TabIndex = 0;
			this->closeStatsButton->Text = L"Close";
			this->closeStatsButton->UseVisualStyleBackColor = true;
			// 
			// statsTextBox
			// 
			this->statsTextBox->Location = System::Drawing::Point(21, 50);
			this->statsTextBox->Multiline = true;
			this->statsTextBox->Name = L"statsTextBox";
			this->statsTextBox->Size = System::Drawing::Size(318, 229);
			this->statsTextBox->TabIndex = 1;
			// 
			// statsLabel
			// 
			this->statsLabel->AutoSize = true;
			this->statsLabel->Location = System::Drawing::Point(17, 19);
			this->statsLabel->Name = L"statsLabel";
			this->statsLabel->Size = System::Drawing::Size(93, 13);
			this->statsLabel->TabIndex = 2;
			this->statsLabel->Text = L"Robokid statistics:";
			// 
			// showStatisticsForm
			// 
			this->AcceptButton = this->closeStatsButton;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->closeStatsButton;
			this->ClientSize = System::Drawing::Size(350, 326);
			this->ControlBox = false;
			this->Controls->Add(this->statsLabel);
			this->Controls->Add(this->statsTextBox);
			this->Controls->Add(this->closeStatsButton);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Name = L"showStatisticsForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Robokid Statistics";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
