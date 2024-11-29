#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QList>
int n=0;
float k=9e+9;
QList<float> Fx, Fy;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for(int i=1;i<21;i++){
        if (i!=17&&i!=18&&i!=14&&i!=2){
    QString buttonName= QString("pushButton_%1").arg(i);
    QPushButton *button=findChild<QPushButton*>(buttonName);
    if (button) {
        QObject::connect(button, &QPushButton::clicked, this, &MainWindow::appendTextToEdit);
        qDebug() << "Connected button:" << buttonName;
    } else {
        qDebug() << "Button not found:" << buttonName;
    }
        }
    }
    QObject::connect(ui->pushButton_17,&QPushButton::clicked,this,&MainWindow::OnInsButtonClicked);
    QObject::connect(ui->pushButton_14,&QPushButton::clicked,this,&MainWindow::addChargeButton);
    QObject::connect(ui->lineEdit_4,&QLineEdit::selectionChanged,this,&MainWindow::SetActiveLineEdit);
    QObject::connect(ui->lineEdit_5,&QLineEdit::selectionChanged,this,&MainWindow::SetActiveLineEdit);
    QObject::connect(ui->lineEdit_8,&QLineEdit::selectionChanged,this,&MainWindow::SetActiveLineEdit);
    if (ui->lineEdit_4) {
        ui->lineEdit_4->installEventFilter(this);
    } else {
        qDebug() << "lineEdit_4 is null!";
    }
    ui->lineEdit_5->installEventFilter(this);
    ui->lineEdit_8->installEventFilter(this);
    ui->lineEdit_8->setFocus();

}

MainWindow::~MainWindow()
{
    delete ui;
}
int MainWindow::GetCurrentCursorPos()
{
    int cursor= ActiveLineEdit->cursorPosition();
    return cursor;
}

void MainWindow::appendTextToEdit(){
    if (!ActiveLineEdit) {
        qDebug() << "appendTextToEdit: No active line edit! Ignoring button press.";
        return;
    }
    int cursor = GetCurrentCursorPos();
    QPushButton *buttonclicked=qobject_cast<QPushButton*>(sender());
    if (!buttonclicked) {
        qDebug() << "appendTextToEdit: Sender is not a QPushButton! Ignoring.";
        return;
    }
    QString buttonText= buttonclicked->text();
    QString currentText= ActiveLineEdit->text();
    currentText.insert(cursor,buttonText);
    ActiveLineEdit->setText(currentText);
    ActiveLineEdit->setCursorPosition(cursor+buttonText.length());
    ActiveLineEdit->setFocus();
    if (cursor == currentText.length()) {
        ActiveLineEdit->setCursorPosition(currentText.length());
    }




}

void MainWindow::SetActiveLineEdit(){
    ActiveLineEdit= qobject_cast<QLineEdit*>(sender());
    if (!ActiveLineEdit) {
        qDebug() << "SetActiveLineEdit: Sender is not a QLineEdit! Ignoring.";
    } else {
        qDebug() << "SetActiveLineEdit: Active line edit set to:" << ActiveLineEdit->objectName();
    }
}

void MainWindow::OnInsButtonClicked(){
    delete1Text();
}
void MainWindow::delete1Text(){
    int cursor = GetCurrentCursorPos();
    if (cursor > 0) {
        QString currentText = ActiveLineEdit->text();
        currentText.remove(cursor - 1, 1);
        ActiveLineEdit->setText(currentText);
        ActiveLineEdit->setCursorPosition(cursor - 1);

}
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event){
    if(event->type()==QEvent::FocusIn){
        QLineEdit *lineEdit = qobject_cast<QLineEdit*>(watched);

        if(lineEdit){
            ActiveLineEdit = lineEdit;
            qDebug() << "eventFilter: Active line edit set to:" << ActiveLineEdit->objectName();
            return false;
        }
    }
    return QObject::eventFilter(watched,event);
}

void MainWindow::addChargeButton(){
    if (ui->lineEdit_8 && ui->lineEdit_4 && ui->lineEdit_5) {

        float charge = ui->lineEdit_8->text().toFloat();
        float xPos = ui->lineEdit_4->text().toFloat();
        float yPos = ui->lineEdit_5->text().toFloat();


        charges.push_back(charge);
        xPositions.push_back(xPos);
        yPositions.push_back(yPos);

        ui->lineEdit_8->clear();
        ui->lineEdit_4->clear();
        ui->lineEdit_5->clear();
        n++;

        // Optionally set the focus back to the first input field for convenience
        ui->lineEdit_8->setFocus();
    }
}
void MainWindow::on_pushButton_2_clicked()
{
    ui->textEdit->clear();
    const double k = 9e9;
    int numParticles = charges.size();


    QVector<double> Fx(numParticles, 0.0);
    QVector<double> Fy(numParticles, 0.0);


    for (int i = 0; i < numParticles; ++i) {
        for (int j = 0; j < numParticles; ++j) {
            if (i != j) {
                double dx = xPositions[j] - xPositions[i];
                double dy = yPositions[j] - yPositions[i];
                double rSquared = dx * dx + dy * dy;
                const double minRSquared = 1e-12;

                if (rSquared <minRSquared) continue;

                double r = std::sqrt(rSquared);
                double force = k * charges[i] * charges[j] / rSquared;

                Fx[i] += force * dx / r;
                Fy[i] += force * dy / r;
            }
        }
    }
    for (int i = 0; i < numParticles; ++i) {
        if (numParticles < 2) {
           ui->textEdit->append(QString("Not enough charges to calculate forces. Add more charges."));
            return;
        }
        ui->textEdit->append(
            QString("Particle %1: Fx = %2 N, Fy = %3 N")
                .arg(i + 1)
                .arg(QString::number(Fx[i],'e',2),QString::number(Fy[i],'e',2))
            );
    }

}

void MainWindow::on_pushButton_18_clicked()
{
    charges.clear();
    xPositions.clear();
    yPositions.clear();
}

