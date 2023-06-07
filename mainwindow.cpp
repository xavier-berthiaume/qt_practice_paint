#include <QtWidgets>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "scribble.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scribble_area = new ScribbleArea();
    setCentralWidget(scribble_area);

    createActions();
    createMenus();
    setWindowTitle(tr("Practice Paint"));

    resize(500, 500);
}

void MainWindow::closeEvent(QCloseEvent *event) {

    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::open() {

    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());

        if (!fileName.isEmpty()) {
            scribble_area->openImage(fileName);
        }
    } else {

    }
}

void MainWindow::save() {

    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);

}

void MainWindow::penColor() {

    QColor new_color = QColorDialog::getColor(scribble_area->penColor());
    if (new_color.isValid()) {
        scribble_area->setPenColor(new_color);
    }

}

void MainWindow::penWidth() {

    bool ok;
    int new_width = QInputDialog::getInt(this, tr("Scribble"), tr("Select pen width : "), scribble_area->penWidth(), 1, 50, 1, &ok);

    if (ok) {
        scribble_area->setPenWidth(new_width);
    }

}

void MainWindow::about() {

    QMessageBox::about(this, tr("About"), tr("<p>The <b>Scribble</b> is awesome</p>"));

}

void MainWindow::createActions() {

    openAction = new QAction(tr("&Open"), this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    foreach(QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction *action = new QAction(text,this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveAsAction.append(action);
    }

    printAction = new QAction(tr("&Print..."), this);
    connect(printAction, SIGNAL(triggered()), scribble_area, SLOT(print()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    penColorAction = new QAction(tr("&Pen Color..."), this);
    connect(penColorAction, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAction = new QAction(tr("&Pen Width..."), this);
    connect(penWidthAction, SIGNAL(triggered()), this, SLOT(penWidth()));

    clearScreenAction = new QAction(tr("&Clear Screen..."), this);
    clearScreenAction->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAction, SIGNAL(triggered()), this, SLOT(clearImage()));

    aboutAction = new QAction(tr("&About..."), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"));
    connect(aboutQtAction, SIGNAL(triggered()), this, SLOT(aboutQt()));
}

void MainWindow::createMenus() {

    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach(QAction *action, saveAsAction) {
        saveAsMenu->addAction(action);
    }

    fileMenu = new QMenu(tr("&File"), this);

    fileMenu->addAction(openAction);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAction);
    optionMenu->addAction(penWidthAction);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAction);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);

}

bool MainWindow::maybeSave() {

    if (scribble_area->isModified()) {
        QMessageBox::StandardButton return_value;

        return_value = QMessageBox::warning(this, tr("Scribble"), tr("The image has been modified.\nDo want to save changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (return_value == QMessageBox::Save) {
            return saveFile("png");
        } else if (return_value == QMessageBox::Cancel) {
            return false;
        }
    }

    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat) {
    QString initialPath = QDir::currentPath() + "/untitled" + fileFormat;
    QString filename = QFileDialog::getSaveFileName(this, tr("Save As"), initialPath, tr("%1 Files (*.%2):: All Files(*)").arg(QString::fromLatin1(fileFormat.toUpper())).arg(QString::fromLatin1(fileFormat)));

    if (filename.isEmpty()) {
        return false;
    } else {
        return scribble_area->saveImage(filename, fileFormat.constData());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

