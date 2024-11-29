#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlineedit.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void appendTextToEdit();
    void SetActiveLineEdit();
    int GetCurrentCursorPos();
    void OnInsButtonClicked();
    void delete1Text();
    void addChargeButton();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_18_clicked();

private:
    Ui::MainWindow *ui;
    QLineEdit *ActiveLineEdit= nullptr;
    QVector<double> charges;
    QVector<double> xPositions;
    QVector<double> yPositions;
};
#endif // MAINWINDOW_H
