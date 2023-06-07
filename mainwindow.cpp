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
    QByte fileFormat = action->data().toByteArray();
    saveFile(fileFormat);

}

void MainWindow::penColor() {

    QColor new_color = QColorDialog::getColor(scribble_area->penColor());
    if (newColor.isValid()) {
        scribble_area->setPenColor(new_color);
    }

}

void MainWindow::penWidth() {

    bool ok;
    int new_width = QInputDialog::getInteger(this, tr("Scribble"), tr("Select pen width : "), scribble_area->penWidth(), 1, 50, 1, &ok);

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

}

MainWindow::~MainWindow()
{
    delete ui;
}

