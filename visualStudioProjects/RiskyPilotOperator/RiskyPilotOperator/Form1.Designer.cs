namespace RiskyPilotOperator
{
    partial class riskyPilotOperatorForm
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        
		//creating the delegate for function updateComLogBoxMethod()
		//-> event serialPort1_DataReceived() is executed in a different thread than the function it should call
		//-> therefore a delegate has to be invoked in order to call the function
		public delegate void updateComLogBox();
		public updateComLogBox updateComLogBoxDelegate;




        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(riskyPilotOperatorForm));
            this.serialPortGroupBox = new System.Windows.Forms.GroupBox();
            this.serialPortBaudRateLabel = new System.Windows.Forms.Label();
            this.serialPortNumberLabel = new System.Windows.Forms.Label();
            this.serialPortDisconnectButton = new System.Windows.Forms.Button();
            this.serialPortConnectButton = new System.Windows.Forms.Button();
            this.baudRateComboBox = new System.Windows.Forms.ComboBox();
            this.portNumberComboBox = new System.Windows.Forms.ComboBox();
            this.comLogBox = new System.Windows.Forms.TextBox();
            this.comLogBoxLabel = new System.Windows.Forms.Label();
            this.serialSubMenuButton = new System.Windows.Forms.Button();
            this.mode1SubMenuButton = new System.Windows.Forms.Button();
            this.mode2SubMenuButton = new System.Windows.Forms.Button();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.comLogBoxClearButton = new System.Windows.Forms.Button();
            this.serialPortGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // serialPortGroupBox
            // 
            this.serialPortGroupBox.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("serialPortGroupBox.BackgroundImage")));
            this.serialPortGroupBox.Controls.Add(this.serialPortBaudRateLabel);
            this.serialPortGroupBox.Controls.Add(this.serialPortNumberLabel);
            this.serialPortGroupBox.Controls.Add(this.serialPortDisconnectButton);
            this.serialPortGroupBox.Controls.Add(this.serialPortConnectButton);
            this.serialPortGroupBox.Controls.Add(this.baudRateComboBox);
            this.serialPortGroupBox.Controls.Add(this.portNumberComboBox);
            this.serialPortGroupBox.ForeColor = System.Drawing.SystemColors.ControlLightLight;
            this.serialPortGroupBox.Location = new System.Drawing.Point(7, 50);
            this.serialPortGroupBox.Name = "serialPortGroupBox";
            this.serialPortGroupBox.Size = new System.Drawing.Size(250, 142);
            this.serialPortGroupBox.TabIndex = 0;
            this.serialPortGroupBox.TabStop = false;
            this.serialPortGroupBox.Text = "Serial Port Settings";
            // 
            // serialPortBaudRateLabel
            // 
            this.serialPortBaudRateLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.serialPortBaudRateLabel.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.serialPortBaudRateLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.serialPortBaudRateLabel.Location = new System.Drawing.Point(17, 67);
            this.serialPortBaudRateLabel.Name = "serialPortBaudRateLabel";
            this.serialPortBaudRateLabel.Size = new System.Drawing.Size(68, 21);
            this.serialPortBaudRateLabel.TabIndex = 2;
            this.serialPortBaudRateLabel.Text = "Baud Rate  ";
            this.serialPortBaudRateLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // serialPortNumberLabel
            // 
            this.serialPortNumberLabel.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.serialPortNumberLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.serialPortNumberLabel.Location = new System.Drawing.Point(17, 28);
            this.serialPortNumberLabel.Name = "serialPortNumberLabel";
            this.serialPortNumberLabel.Size = new System.Drawing.Size(68, 21);
            this.serialPortNumberLabel.TabIndex = 2;
            this.serialPortNumberLabel.Text = "Port Number";
            this.serialPortNumberLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // serialPortDisconnectButton
            // 
            this.serialPortDisconnectButton.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.serialPortDisconnectButton.Location = new System.Drawing.Point(133, 105);
            this.serialPortDisconnectButton.Name = "serialPortDisconnectButton";
            this.serialPortDisconnectButton.Size = new System.Drawing.Size(75, 23);
            this.serialPortDisconnectButton.TabIndex = 1;
            this.serialPortDisconnectButton.Text = "Disconnect";
            this.serialPortDisconnectButton.UseVisualStyleBackColor = false;
            this.serialPortDisconnectButton.Click += new System.EventHandler(this.serialPortDisconnectButton_Click);
            // 
            // serialPortConnectButton
            // 
            this.serialPortConnectButton.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.serialPortConnectButton.Location = new System.Drawing.Point(29, 105);
            this.serialPortConnectButton.Name = "serialPortConnectButton";
            this.serialPortConnectButton.Size = new System.Drawing.Size(75, 23);
            this.serialPortConnectButton.TabIndex = 1;
            this.serialPortConnectButton.Text = "Connect";
            this.serialPortConnectButton.UseVisualStyleBackColor = false;
            this.serialPortConnectButton.Click += new System.EventHandler(this.serialPortConnectButton_Click);
            // 
            // baudRateComboBox
            // 
            this.baudRateComboBox.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.baudRateComboBox.Enabled = false;
            this.baudRateComboBox.FormattingEnabled = true;
            this.baudRateComboBox.Items.AddRange(new object[] {
            "9600",
            "19200",
            "38400",
            "57600",
            "115200"});
            this.baudRateComboBox.Location = new System.Drawing.Point(98, 67);
            this.baudRateComboBox.Name = "baudRateComboBox";
            this.baudRateComboBox.Size = new System.Drawing.Size(140, 21);
            this.baudRateComboBox.TabIndex = 0;
            // 
            // portNumberComboBox
            // 
            this.portNumberComboBox.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.portNumberComboBox.FormattingEnabled = true;
            this.portNumberComboBox.Items.AddRange(new object[] {
            "COM0",
            "COM1",
            "COM2",
            "COM3",
            "COM4",
            "COM5",
            "COM6",
            "COM7",
            "COM8",
            "COM9"});
            this.portNumberComboBox.Location = new System.Drawing.Point(98, 28);
            this.portNumberComboBox.Name = "portNumberComboBox";
            this.portNumberComboBox.Size = new System.Drawing.Size(140, 21);
            this.portNumberComboBox.TabIndex = 0;
            this.portNumberComboBox.SelectedIndexChanged += new System.EventHandler(this.portNumberComboBox_SelectedIndexChanged);
            // 
            // comLogBox
            // 
            this.comLogBox.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.comLogBox.ForeColor = System.Drawing.SystemColors.ControlLightLight;
            this.comLogBox.Location = new System.Drawing.Point(7, 228);
            this.comLogBox.Multiline = true;
            this.comLogBox.Name = "comLogBox";
            this.comLogBox.ReadOnly = true;
            this.comLogBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.comLogBox.Size = new System.Drawing.Size(250, 326);
            this.comLogBox.TabIndex = 1;
            // 
            // comLogBoxLabel
            // 
            this.comLogBoxLabel.AutoSize = true;
            this.comLogBoxLabel.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.comLogBoxLabel.ForeColor = System.Drawing.SystemColors.ControlLightLight;
            this.comLogBoxLabel.Location = new System.Drawing.Point(11, 210);
            this.comLogBoxLabel.Name = "comLogBoxLabel";
            this.comLogBoxLabel.Size = new System.Drawing.Size(100, 13);
            this.comLogBoxLabel.TabIndex = 2;
            this.comLogBoxLabel.Text = "Communication Log";
            // 
            // serialSubMenuButton
            // 
            this.serialSubMenuButton.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("serialSubMenuButton.BackgroundImage")));
            this.serialSubMenuButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.serialSubMenuButton.ForeColor = System.Drawing.SystemColors.ControlLightLight;
            this.serialSubMenuButton.Location = new System.Drawing.Point(8, 9);
            this.serialSubMenuButton.Name = "serialSubMenuButton";
            this.serialSubMenuButton.Size = new System.Drawing.Size(68, 35);
            this.serialSubMenuButton.TabIndex = 3;
            this.serialSubMenuButton.Text = "Serial";
            this.serialSubMenuButton.UseVisualStyleBackColor = true;
            // 
            // mode1SubMenuButton
            // 
            this.mode1SubMenuButton.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("mode1SubMenuButton.BackgroundImage")));
            this.mode1SubMenuButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.mode1SubMenuButton.ForeColor = System.Drawing.SystemColors.ControlLightLight;
            this.mode1SubMenuButton.Location = new System.Drawing.Point(98, 9);
            this.mode1SubMenuButton.Name = "mode1SubMenuButton";
            this.mode1SubMenuButton.Size = new System.Drawing.Size(68, 35);
            this.mode1SubMenuButton.TabIndex = 3;
            this.mode1SubMenuButton.Text = "Mode 1";
            this.mode1SubMenuButton.UseVisualStyleBackColor = true;
            this.mode1SubMenuButton.Click += new System.EventHandler(this.mode1SubMenuButton_Click);
            // 
            // mode2SubMenuButton
            // 
            this.mode2SubMenuButton.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("mode2SubMenuButton.BackgroundImage")));
            this.mode2SubMenuButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.mode2SubMenuButton.ForeColor = System.Drawing.SystemColors.ControlLightLight;
            this.mode2SubMenuButton.Location = new System.Drawing.Point(189, 9);
            this.mode2SubMenuButton.Name = "mode2SubMenuButton";
            this.mode2SubMenuButton.Size = new System.Drawing.Size(68, 35);
            this.mode2SubMenuButton.TabIndex = 3;
            this.mode2SubMenuButton.Text = "Mode 2";
            this.mode2SubMenuButton.UseVisualStyleBackColor = true;
            // 
            // serialPort1
            // 
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // comLogBoxClearButton
            // 
            this.comLogBoxClearButton.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.comLogBoxClearButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.comLogBoxClearButton.ForeColor = System.Drawing.SystemColors.ControlLightLight;
            this.comLogBoxClearButton.Location = new System.Drawing.Point(182, 209);
            this.comLogBoxClearButton.Name = "comLogBoxClearButton";
            this.comLogBoxClearButton.Size = new System.Drawing.Size(75, 16);
            this.comLogBoxClearButton.TabIndex = 3;
            this.comLogBoxClearButton.Text = "C L R";
            this.comLogBoxClearButton.UseVisualStyleBackColor = false;
            this.comLogBoxClearButton.Click += new System.EventHandler(this.comLogBoxClearButton_Click);
            // 
            // riskyPilotOperatorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(1008, 562);
            this.Controls.Add(this.comLogBoxClearButton);
            this.Controls.Add(this.mode2SubMenuButton);
            this.Controls.Add(this.mode1SubMenuButton);
            this.Controls.Add(this.serialSubMenuButton);
            this.Controls.Add(this.comLogBoxLabel);
            this.Controls.Add(this.comLogBox);
            this.Controls.Add(this.serialPortGroupBox);
            this.Name = "riskyPilotOperatorForm";
            this.Text = "Risky Pilot Operator";
            this.serialPortGroupBox.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox serialPortGroupBox;
        private System.Windows.Forms.Label serialPortBaudRateLabel;
        private System.Windows.Forms.Label serialPortNumberLabel;
        private System.Windows.Forms.Button serialPortDisconnectButton;
        private System.Windows.Forms.Button serialPortConnectButton;
        private System.Windows.Forms.ComboBox baudRateComboBox;
        private System.Windows.Forms.ComboBox portNumberComboBox;
        private System.Windows.Forms.TextBox comLogBox;
        private System.Windows.Forms.Label comLogBoxLabel;
        private System.Windows.Forms.Button serialSubMenuButton;
        private System.Windows.Forms.Button mode1SubMenuButton;
        private System.Windows.Forms.Button mode2SubMenuButton;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Button comLogBoxClearButton;
    }
}

