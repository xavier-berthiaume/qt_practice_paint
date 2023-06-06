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

MainWindow::~MainWindow()
{
    delete ui;
}

