#include "dialog.h"
#include <QApplication>
#include <QWSServer>
#include <QSplashScreen>
#include <QProgressBar>
#include <configadapter.h>
#include <QProcess>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QThread::currentThread()->setPriority(QThread::HighPriority);

    // kiem tra va an con tro di, (hide mouse pointer)
#ifdef Q_WS_QWS
    QWSServer::setCursorVisible( false );
#endif

    // splash screen
    QPixmap pixmap ("/home/root/CODE/images/SPLASHSCREEN.png");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    QFont font("FreeSans", 48, QFont::Bold);
    splash->setFont(font);
    splash->show();

    sleep(5);

    // khoi tao dialog cho giao dien
    Dialog w;

    w.showFullScreen();
    w.show();

    // tat splash screen
    QTimer::singleShot(2000, splash, SLOT(close()));

    return a.exec();
}
