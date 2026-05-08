#include <QApplication>
#include <QIcon>

#include "MainWindow.h"
#include "Style.h"

int main(int argc, char *argv[])
	{
		QApplication app(argc, argv);
		
	app.setApplicationName("Renegade");
	app.setApplicationDisplayName("by Azyreh");
	app.setOrganizationName("Azyreh");
	app.setOrganizationDomain("rng-server.net");
	
	app.setStyleSheet(Style::darkTheme());
	
    MainWindow window;

    window.setWindowTitle("Renegade - Semnătură pentru cod");
    window.setWindowIcon(QIcon(":/logo_renegade.svg"));
    window.setMinimumSize(1100, 640);
    window.resize(1220, 684);
		window.show();
		return app.exec();
	}
