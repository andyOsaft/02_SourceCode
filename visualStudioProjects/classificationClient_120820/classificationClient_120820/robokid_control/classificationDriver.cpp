// robokid_control.cpp: Hauptprojektdatei.

#include "stdafx.h"
#include "Form1.h"


using namespace robokid_control;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Aktivieren visueller Effekte von Windows XP, bevor Steuerelemente erstellt werden
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Hauptfenster erstellen und ausführen
	//Application::Run startet die Message loop für die Anwendung
	Application::Run(gcnew Form1());
	return 0;
}
