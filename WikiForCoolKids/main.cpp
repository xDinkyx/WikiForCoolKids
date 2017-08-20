#include <QApplication>
#include <QWidget>
#include <QPlainTextEdit>
#include <QMetaObject>
#include <QThread>
#include <QBoxLayout>
#include <QDateTime>
#include <QDebug>

#include <cstdio>
#include <cassert>

#include "MainWindow.h"

QWidget* debug_console = nullptr;
QPlainTextEdit* debug_message_display_textedit = nullptr;
void setupDebugDisplay();
void debugMessageDisplayFunc(QtMsgType type, const QMessageLogContext& context, const QString& msg);

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);

    setupDebugDisplay();

    MainWindow window;
    window.show();

    int ret = a.exec();
    delete debug_console;
    return ret;
}

void setupDebugDisplay()
{
    debug_console = new QWidget();
    debug_console->setWindowTitle("Debug Log");
    debug_console->setAttribute(Qt::WA_QuitOnClose, false); //quit only when main window is closed
    debug_console->setMinimumSize(800, 600);

    debug_message_display_textedit = new QPlainTextEdit(debug_console);
    QFont font = QFont("Consolas");
    font.setStyleHint(QFont::TypeWriter);
    debug_message_display_textedit->setFont(font);
    debug_message_display_textedit->setReadOnly(true);

    QBoxLayout* console_layout = new QVBoxLayout();
    console_layout->addWidget(debug_message_display_textedit);

    debug_console->setLayout(console_layout);
    debug_console->show();

    qInstallMessageHandler(debugMessageDisplayFunc);
}

void debugMessageDisplayFunc(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    bool do_abort = false;
    const char* msgTypeStr = NULL;
    switch (type) 
    {
        case QtDebugMsg:
            msgTypeStr = "Debug";
            break;
        case QtWarningMsg:
            msgTypeStr = "Warning";
            break;
        case QtCriticalMsg:
            msgTypeStr = "Critical";
            break;
        case QtFatalMsg:
            msgTypeStr = "Fatal";
            do_abort = true;
        default:
            assert(0);
            return;
    }

    QTime now = QTime::currentTime();
    QString formattedMessage = QString::fromLatin1("%1 %2 %3")
        .arg(now.toString("hh:mm:ss"))
        .arg(msgTypeStr).arg(msg);

    // print on console:
    fprintf(stderr, "%s\n", formattedMessage.toLocal8Bit().constData());
    // print in debug log window
    {
        bool isMainThread = QThread::currentThread() == QApplication::instance()->thread();
        if (debug_message_display_textedit)
        {
            if (isMainThread)
                debug_message_display_textedit->appendPlainText(formattedMessage);
            else // additional code, so that qDebug calls in threads will work as well
                QMetaObject::invokeMethod(debug_message_display_textedit, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, formattedMessage));
        }
    }
}