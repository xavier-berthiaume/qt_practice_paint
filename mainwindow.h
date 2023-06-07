#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ScribbleArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void about();

private:
    Ui::MainWindow *ui;

    void createActions();
    void createMenus();
    bool maybeSave();
    bool saveFile(const QByteArray &file_format);

    ScribbleArea *scribble_area;

    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *helpMenu;

    QAction *openAction;
    QList<QAction *> saveAsAction;
    QAction *exitAction;
    QAction *penColorAction;
    QAction *penWidthAction;
    QAction *printAction;
    QAction *clearScreenAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
};
#endif // MAINWINDOW_H
