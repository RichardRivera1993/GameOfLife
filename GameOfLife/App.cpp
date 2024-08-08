#include "App.h"
#include "MainWindow.h"
wxIMPLEMENT_APP(App);

App::App() {

}

App::~App() {
	delete mainWindow;
}

bool App::OnInit() {
	mainWindow = new MainWindow("Game of Life");
	mainWindow->Show(true);
	return true;
}