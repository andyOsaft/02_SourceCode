#pragma once

namespace robokid_control {
	
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;

	

	/// <summary>
	/// Zusammenfassung für mapForm
	/// </summary>
	public ref class mapForm : public System::Windows::Forms::Form
	{
	public:
		mapForm(void)
		{
			InitializeComponent();
			//
			//TODO: Konstruktorcode hier hinzufügen.
			//
			this->Location = Point(653,100);
			//initialize sensorSamples array and global map array
			localSensorSamples = gcnew array< Double > (30);
			//70x70 cell array for map (rank=dimension=2)
			globalMap = gcnew array<Int32,2>(70,70);
			//temp global map with same size than global map
			tempGlobalMap = gcnew array<Int32,2>(70,70);
			//initialize global sensor samples array -> for temp storage of sample x/y coordinates			
			globalSensorSamples = gcnew array<Double,2>(2,30);
			//2x100 pathVector -> for storage of path from robot to target (x/y coordinates)
			pathVector = gcnew array<Int32,2>(2,100);
			
			//Save size of global map
			mapXSize=globalMap->GetLength(0);
			mapYSize=globalMap->GetLength(1);

			//set startFlag
			startFlag=1;
			obstacleUpdateFlag=0;
			obstacleSetFlag=1;
			//for debugging
			//first run is without setRobotPose data (xRobot & yRobot)
			xRobot=99;
			yRobot=99;
			angleRobot=0;
			xTarget=99;
			yTarget=99;
			/*
			for (int k=0;k<30;k++)
			{	
				localSensorSamples[k]=30;
			}
			*/
			//localSensorSamples[0]=30;
			this->Cursor = Cursors::Hand;
		}		

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~mapForm()
		{
			if (components)
			{
				delete components;
			}
		}

	//***********************************************************************************************************
	// GUI parts:
	//***********************************************************************************************************
	private: System::Windows::Forms::Label^  label1;
	

	//***********************************************************************************************************
	// map variables:
	//***********************************************************************************************************
	public:
		//delegate void delegateInvoke(int x , int y);
	public:
		//delegateInvoke^ invokeSendObstacle;
	private:
		int indexLeft, indexRight; 
	public:	
		int startFlag;
	private:	
		array< Double >^ localSensorSamples;
	//array with global coordinates of possible obstacles
	private:
		array<Double,2>^ globalSensorSamples;
	public:
		array<Int32,2>^ globalMap;
	public:
		array<Int32,2>^ tempGlobalMap;
	public:
		array<Int32,2>^ pathVector; 
	private:
		double xRobot, yRobot, angleRobot; //angle in rad

	private:
		double xTarget, yTarget;

	private: 
		double xRobotOld, yRobotOld, angleRobotOld;

	private:
		double xTargetOld, yTargetOld;

	private: 
		double xAngle, yAngle;
	private:
		double xAngleOld, yAngleOld;
	//the following variables control what is done in the paint event
	public:
		//obstacleUpdateFlag is set to 1 after a whole set of sensor data is received in Form1->displayFrontData()
		//it controls in the paint event (mapForm) if the incoming data is converted to global coordinates and drawn to the map
		int obstacleUpdateFlag;
	public:
		//This flag controls whether the user is allowed to define obstacle positions on the map form
		//-> if =0 cursor over mapForm changes to normal one and click has no effect
		int obstacleSetFlag;
	public:
		int mapXSize;
		int mapYSize;

	private:
		GraphicsState^ mapStatus;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

	//***********************************************************************************************************
	// methods & delegates:
	//***********************************************************************************************************
	public:
		void setFrontSensorData(int indexL, int sampleL, int indexR, int sampleR)
		{
			indexLeft=indexL;
			indexRight=indexR;
			
			
			//store samples in front sensor array
			try
			{
				localSensorSamples[indexLeft]=sampleL;
				localSensorSamples[indexRight]=sampleR;
			}
			catch (Exception^)
			{
				int dummy=1;
				dummy = 2;
			}
		}
	public:
		void setRobotPose(int xBot, int yBot, int angleBot)
		{	
			//Delete old robot position
			if (xRobot>0 && xRobot<70 && yRobot>0 && yRobot<70)
			{
				globalMap[(int)xRobot,(int)yRobot]=255;
			}
			//Set new robot position
			xRobot=xBot;
			yRobot=yBot;
			angleRobot=(angleBot*((Math::PI)/180));
			globalMap[(int)xRobot,(int)yRobot]=254;
		}
	public: 
		void setTargetPosition(int xTar, int yTar)
		{
			//Delete old target position
			if (xTarget>0 && xTarget<70 && yTarget>0 && yTarget<70)
			{
				globalMap[(int)xTarget,(int)yTarget]=255;
			}

			//Set new target position
			xTarget=xTar;
			yTarget=yTar;
			if (xTarget>0 && xTarget<70 && yTarget>0 && yTarget<70)
			{
			globalMap[(int)xTarget,(int)yTarget]=1;
			}	
		}
	//***********************************************************************************************************
	// convertIncomingData()
	// -----------------------	
	//	process all sensor values:
	//		- compute obstacle´s local coordinates
	//			-> these coordinates are out of the local view of the robot (robot=origin of coordinate system)
	//			-> obstacle visible if value != 99
	//			-> values are calculated by sine-laws of the right angle triangle
	//		- consider robot´s pose by rotating the coordinates with the robot´s angle value 
	//	      (robot is looking upwards = 0°)					     ------------>
	//			-> this brings us the vector coordinates for vector  RobotObstacle
	//		- with vector addition we can now compute the obstacles global coordinates on the map
	//          ----------->   -------->   ------------>
	//			ZeroObstacle = ZeroRobot + RobotObstacle
	//
	//		- the line that indicates the robot´s angle goes from the center of the robot´s current position cell
	//		  to a coordinate straight in front of the robot
	//		  -> this coordinate in front of the robot is treated and calculated the same way as an obstacle in  
	//			 front of the robot
	//***********************************************************************************************************
	public: 
		void convertIncomingData()
		{
			int k=0, l=0;
			//angle for samples angle (polar coordinate)
			double sampleAngle=0;
			double tempX=0, sampleX=0;
			double tempY=0, sampleY=0;
			double tempXAngle=0, tempYAngle=0;
			
			//pre-process sensor values to eliminate readings caused by noise
			//-> if a sample has a "out of sensor range" value on both sides -> assume noise -> delete it 
			//same filter runs also on robot
			for (l=1; l<29; l++)
			{
				if (l==1)
				{
					if(localSensorSamples[1]==99)
					{
						localSensorSamples[0]=99;
					}
				}
				else if (l==28)
				{
					if(localSensorSamples[28]==99)
					{
						localSensorSamples[29]=99;
					}
				}
		
				if ((localSensorSamples[l-1]==99) && (localSensorSamples[l+1]==99))
				{
					try
					{
						localSensorSamples[l]=99;
					}
					catch (Exception^)
					{
						int dummy=1;
						dummy = 2;
					}
				}
				
			}

			for (l=0; l<30;l++)
			{
				//test if obstacle is in sensor range (value !=99)
				if ((localSensorSamples[l]>0) && (localSensorSamples[l]<40))
				{

					//apply scale factor to sample values
					//-> 1cm map = 1 cell = 5cm reality					
					localSensorSamples[l]=(localSensorSamples[l])/5;

					//local obstacle coordinates	
					//sample angle depends on sample´s position in sampleArray (angle in rad)
					//0°->90° ; 30 samples
					if (l<15)
					{
						sampleAngle=((l*6.43)*( (Math::PI)/180));
						tempX=(localSensorSamples[l])*(Math::Cos(sampleAngle));
						tempY=(localSensorSamples[l])*(Math::Sin(sampleAngle));
					}
					else
					{
						sampleAngle=((180-(l*6.21))*( (Math::PI)/180));
						tempX=(localSensorSamples[l])*(Math::Cos(sampleAngle));
						tempY=(localSensorSamples[l])*(Math::Sin(sampleAngle));
					}


					//apply rotary matrix to the local coordinates
					//matrix multiplication:
					if (l<15)//->points in negative x area -> *-1
					{
						sampleX=(-1*tempX*(Math::Cos(angleRobot)))+(tempY*(Math::Sin(angleRobot)));
						sampleY=(tempX*(Math::Sin(angleRobot)))+(tempY*(Math::Cos(angleRobot)));					
					}			
					else
					{
						sampleX=(tempX*(Math::Cos(angleRobot)))+(tempY*(Math::Sin(angleRobot)));
						sampleY=(-1*tempX*(Math::Sin(angleRobot)))+(tempY*(Math::Cos(angleRobot)));
					}

					try
					{
						//vector addition origin->obstacle = origin->robot + robot->obstacle 
						globalSensorSamples[0,l] = (xRobot-0)+(sampleX);
						globalSensorSamples[1,l] = (yRobot-0)+(sampleY);
					}
					catch (Exception^)
					{
						int dummy=1;
						dummy = 2;
					}
				}
				else
				{	
					globalSensorSamples[0,l] = 0;
					globalSensorSamples[1,l] = 0;
				}
			}
			//calculate coordinates for angle indicator line
			//local coordinates out of the robot´s view
			xAngle=0;
			yAngle=2;
		
			//apply rotary matrix, depending on robot´s angle
			tempXAngle=(xAngle*(Math::Cos(angleRobot)))+(yAngle*(Math::Sin(angleRobot)));
			tempYAngle=(-1*xAngle*(Math::Sin(angleRobot)))+(yAngle*(Math::Cos(angleRobot)));	
			//vector addition origin->obstacle = origin->robot + robot->obstacle 
			xAngle = (xRobot-0)+(tempXAngle);
			yAngle = (yRobot-0)+(tempYAngle);

		}
	//***********************************************************************************************************
	// registerObstaclesGlobal()
	// -----------------------	
	//		- store calculated obstacles in global grid map
	//***********************************************************************************************************
	public: 
		void registerObstaclesGlobal()
		{
			int i=0;
			int obstacleX=0, obstacleY=0;

			for (i=0; i<30;i++)
			{	
				//at the moment only int coordinates in map
				obstacleX = (int)(globalSensorSamples[0,i]+0.5);
				obstacleY = (int)(globalSensorSamples[1,i]+0.5);
				if ((obstacleX!=0) && (obstacleY!=0))
				{
					try
					{
						//make sure, that obstacles are inside the map boundaries
						if (obstacleX>69)
						{
							obstacleX=69;
						}
						else if (obstacleX<0)
						{
							obstacleX=0;
						}
						if (obstacleY>69)
						{
							obstacleY=69;
						}
						else if (obstacleY<0)
						{
							obstacleY=0;
						}
						globalMap[obstacleX,obstacleY]=2; //2=code for obstacle recognized during run-time
					}
					catch (Exception^)
					{
						int dummy=1;
						dummy = 2;
					}
				}
			}
		}
	//***********************************************************************************************************
	// useIncomingMap()
	// -----------------------	
	//		- store content of received tempMap into productive global map
    //
	// Question:  
	//           Why am I not saving the received bytes instantly into the global map array?
	//				
	//			 -> because if I decide not to save any content into the productive map (like robot pos...)
	//				I can filter this undesired content out. And here is a good place for that.
    //           -> secondly if a map transfer is interrupted or not complete I don´t destroy my productive map.
	//***********************************************************************************************************
	public:
		void useIncomingMap()
		{
			int i=0,j=0;

			for (i=0; i<mapXSize; i++)
			{
				for(j=0; j<mapYSize; j++)
				{
					//globalMap[i,j] = tempGlobalMap[69-j,i]; 
					globalMap[i,j] = tempGlobalMap[i,j];
				}
			}
			this->loadMapFile();
		}
	//***********************************************************************************************************
	// load mapFile()
	// -----------------------	
	//
	//-> inside this function all the hard-coded map data is saved into the map data structure
	//-> in future this could include loading file based map data (at the moment data is created using loops) 
	// 
	//
	//	Why are the map elements represented by number 999 -> Because the software sends all wall data equal 253
	//	to the robot. This is not necessary because the same map data is also hard-coded in the robot.
	//***********************************************************************************************************
	public:
		void loadMapFile(void)
		{
			int x,y;


			//create block0
			//first part
			for (y=49; y<64;y++)
			{
				for (x=33; x<36; x++)
				{
					this->globalMap[x,y]=999;
				}
			}
			
			//second part
			for (y=49; y<52;y++)
			{
				for (x=13; x<33; x++)
				{
					this->globalMap[x,y]=999;
				}
			}
			//third part
			for (y=18; y<52;y++)
			{
				for (x=10; x<13; x++)
				{
					this->globalMap[x,y]=999;
				}
			}

			
			//create block1
			for (y=38; y<64;y++)
			{
				for (x=46; x<49; x++)
				{
					this->globalMap[x,y]=999;
				}
			}

			//create block2
			//first part
			for (y=28; y<31;y++)
			{
				for (x=25; x<62; x++)
				{
					this->globalMap[x,y]=999;
				}
			}
			
			//second part
			for (y=10; y<28;y++)
			{
				for (x=40; x<43; x++)
				{
					this->globalMap[x,y]=999;
				}
			}
			//third part
			for (y=10; y<28;y++)
			{
				for (x=59; x<62; x++)
				{
					this->globalMap[x,y]=999;
				}
			}
			

			//create block3
			for (y=11; y<20;y++)
			{
				for (x=23; x<28; x++)
				{
					this->globalMap[x,y]=999;
				}
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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(1643, 1016);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(52, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"robotMap";
			// 
			// mapForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(726, 748);
			this->ControlBox = false;
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->Name = L"mapForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Text = L"Map";
			this->Load += gcnew System::EventHandler(this, &mapForm::mapForm_Load);
			this->Click += gcnew System::EventHandler(this, &mapForm::mapForm_Click);
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &mapForm::mapForm_Paint);
			this->ResumeLayout(false);
			this->PerformLayout();

		}

	//***********************************************************************************************************
	// event handlers:
	//***********************************************************************************************************
#pragma endregion
	private: System::Void mapForm_Load(System::Object^  sender, System::EventArgs^  e) {


			 }
	//***********************************************************************************************************
	// Event->Paint:
	// -----------------------	
	// - creates a pen tool for drawings
	// - on first call of paint -> load default grid image
	// - following calls of paint -> load last map image
	//
	//***********************************************************************************************************
	private: System::Void mapForm_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
						
			int i=0, j=0;
			int lengthAngle=2;
		
			//1.
			//create pens for drawings
			System::Drawing::Pen^ myPen;
			myPen = gcnew System::Drawing::Pen(System::Drawing::Color::Black);
			System::Drawing::Pen^ myRobotPen;
			myRobotPen = gcnew System::Drawing::Pen(System::Drawing::Color::	Brown);	
			System::Drawing::Pen^ myDeletePen;
			myDeletePen = gcnew System::Drawing::Pen(System::Drawing::Color::White);	
			System::Drawing::Pen^ myTargetPen;
			myTargetPen = gcnew System::Drawing::Pen(System::Drawing::Color::Green);	
			myTargetPen->Width = 2.0F;
			System::Drawing::Pen^ myPathPen;
			myPathPen = gcnew System::Drawing::Pen(System::Drawing::Color::	Magenta);
			System::Drawing::Pen^ myDynObstaclePen;
			myDynObstaclePen = gcnew System::Drawing::Pen(System::Drawing::Color::	Brown);
			//System::Drawing::SolidBrush^ myPathBrush;
			//myPathBrush = gcnew System::Drawing::SolidBrush(System::Drawing::Color::Gold);

			//myPen->Color = System::Drawing::Color::Black;
			myPen->Width = 1.0F;	
			myRobotPen->Width = 2.0F;
			myPathPen->Width = 2.0F;
			String^ savePath = "E:\\Buero_02_05_10\\MScEmbeddedSystems\\Dissertation\\visualStudio\\robokid_control_110702\\robokid_control\\robotMap.jpg";
			Image^ gridImage;

			//2. 
			//  - load & draw default grid image
			//	- draw calibration rectangle on screen
			//	- try to save image to jpg file
			//	- set startFlag to zero
			if (startFlag)
			{
				//Load and display grid image for map background
				gridImage = Image::FromFile("grid_for_map.png");	
				xRobotOld = xRobot;
				yRobotOld = yRobot;
				angleRobotOld = angleRobot;
				xTargetOld = xTarget;
				yTargetOld = yTarget;
			}
			else
			{
				//Load and display last map image 
				gridImage = Image::FromFile("robotMap.jpg");	
			}
			
			
			
			
			// Create graphics object for alteration.
			Graphics^ myGraphics = Graphics::FromImage( gridImage );
			
			if (startFlag)
			{
				//clear map
				for (i=0; i<mapXSize;i++)
				{
					for(j=0; j<mapYSize;j++)
					{

						myGraphics->DrawRectangle(myDeletePen, ((i*10)+10), (((mapYSize-j)*10)), 10, 10);
						globalMap[i,j]=255;

					}
				}
				this->loadMapFile();
				startFlag=0;
			}
			
			//if (obstacleUpdateFlag==1) -> deactivated, otherwise the robot´s heading is not displayed correctly on first run
			//{
				//3. call function to convert robot´s local polar coordinates to global map coordinates
				convertIncomingData();
			
				//4. register obstacles coordinates in global map array
				registerObstaclesGlobal();

				obstacleUpdateFlag=0;
			//}

			//scan map & draw content on screen
			for (i=0; i<mapXSize;i++)
			{
				for(j=0; j<mapYSize;j++)
				{	
					//draw path
					if (globalMap[i,j]>2 && globalMap[i,j]<253)
					{
						myGraphics->DrawRectangle(myPathPen, ((i*10)+10), (((mapYSize-j)*10)), 10, 10);
					}
					//check map content and draw the appropriate on screen
					//draw obstacle
					//value = 999 represents a priori defined walls of the environment
					//-> these walls are hard-coded into the robot and don´t need to be sended.
					//value = 253 represents walls defined by the user during program run-time
					else if (globalMap[i,j]==253 || globalMap[i,j]==999)
					{	
						// x,y,width,height +10 pixel offset (frame of grid)
						myGraphics->DrawRectangle(myPen, ((i*10)+10), (((mapYSize-j)*10)), 10, 10);
					}
					//draw dynamic obstacle (recognised during run-time)
					else if (globalMap[i,j]==2)
					{
						myGraphics->DrawRectangle(myDynObstaclePen, ((i*10)+10), (((mapYSize-j)*10)), 10, 10);	
					}

					
				}
			}

			//Grid image calibration -> 10 pixels are 1 grid cell
			// -> 70x70 cells map
			//Draw frame rectangle
			Rectangle testRectangle = Rectangle(10,10,700,700);
			myGraphics->DrawRectangle(myPen, testRectangle);
			// Draw image to screen.
			e->Graphics->DrawImage(gridImage,0,0,1204,1204);




			//Delete old target position
			myGraphics->DrawRectangle(myDeletePen, ((int)(xTargetOld*10)+10), ((int)((mapYSize-yTargetOld)*10)), 10, 10);

			//Draw target position
			myGraphics->DrawRectangle(myTargetPen, ((int)(xTarget*10)+10), ((int)((mapYSize-yTarget)*10)), 9, 9);
			
			//Delete old Robot Position
			myGraphics->DrawRectangle(myDeletePen, ((int)(xRobotOld*10)+10), ((int)((mapYSize-yRobotOld)*10)), 10, 10);

			//Draw Robot Position
			myGraphics->DrawRectangle(myRobotPen, ((int)(xRobot*10)+10), ((int)((mapYSize-yRobot)*10)), 9, 9);
			
			//Draw Robot Angle 
			//x1,y1,x2,y2
			Point point1 = Point( (int) ((xRobot*10)+15), (int) (((mapYSize-yRobot)*10)+5));
			Point point2 = Point( (int) ((xAngle*10)+15), (int) (((mapYSize-yAngle)*10)+5));

			myGraphics->DrawLine(myRobotPen,point1,point2); 
			//myGraphics->DrawLine(myRobotPen,340,340,360, (mapYSize-36)*10);


			e->Graphics->DrawImage(gridImage,0,0,1204,1204);


			try
			{
				// Save the image as a JPG.
				gridImage->Save(savePath, System::Drawing::Imaging::ImageFormat::Jpeg);
			}
			catch (Exception^)
			{
				// Carry on regardless
			}	
			startFlag=0;
			    
		}

	private: System::Void mapForm_Click(System::Object^  sender, System::EventArgs^  e) {
								
			int obstacleX=99;
			int obstacleY=99;
		    //get current mouse position
			Point^ mousePos = this->PointToClient(Control::MousePosition);
			
			if (mousePos->X>20 && mousePos->X<690 && mousePos->Y>20 && mousePos->Y<690)
			{
				if (obstacleSetFlag==1)
				{
					//store obstacle at this map cell
					//-> convert pixel values into cell values x/y (origin top left corner)
					obstacleX = (int)((mousePos->X)/10);
					obstacleY = (int)((mousePos->Y)/10);
					//-> convert coordinates into robot coordinate system (origin down left corner)
					obstacleY = mapYSize - obstacleY;

					//make obstacle bigger than one cell and store all obstacle cells
					if (globalMap[obstacleX-1,obstacleY]==255 || globalMap[obstacleX-1,obstacleY]==0)
					{	
						globalMap[obstacleX-1,obstacleY]=253;
					}
					if (globalMap[obstacleX,obstacleY]==255 || globalMap[obstacleX,obstacleY]==0)
					{
						globalMap[obstacleX,obstacleY]=253;
					}
					if (globalMap[obstacleX-1,obstacleY+1]==255 || globalMap[obstacleX-1,obstacleY+1]==0)
					{
						globalMap[obstacleX-1,obstacleY+1]=253;
					}
					if (globalMap[obstacleX,obstacleY+1]==255 || globalMap[obstacleX,obstacleY+1]==0)
					{
						globalMap[obstacleX,obstacleY+1]=253;
					}


					this->Refresh();
				}
			}
		}
	};
}