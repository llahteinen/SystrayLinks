#include "maindialog.hpp"
#include "ui_maindialog.h"
#include <QMenu>
#include <QCloseEvent>


MainDialog::MainDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::MainDialog)
    , m_tray{} /// No parent for value member QObject
{
    ui->setupUi(this);

    setupTrayIcon();
    m_tray.trayIcon().show();
}

MainDialog::~MainDialog()
{
    delete ui;
}

/// This only happens when the main window is closed, but not when the app is quit from the system tray
void MainDialog::closeEvent(QCloseEvent* event)
{
    qDebug() << "MainDialog::closeEvent";
    /// Keep app running, just close the dialog
    if(m_tray.trayIcon().isVisible())
    {
        hide();
        event->ignore();
    }
}

void MainDialog::setupTrayIcon()
{
    connect(&m_tray, &Tray::restoreTriggered, this, [this]() {
        qDebug() << "restoreTriggered";
        showNormal(); /// Window "restore" action
        activateWindow(); /// Make the dialog foreground if user had clicked something else
    });

    connect(&m_tray, &Tray::quitTriggered, qApp, &QCoreApplication::quit);

    QIcon icon = QIcon::fromTheme("emblem-favorite");
    m_tray.trayIcon().setIcon(icon);
    setWindowIcon(icon);
}
