#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include "parser.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString open = QFileDialog::getOpenFileName(this,"Open a file", QDir::cleanPath("C:/"), "*.cpp");
           QFile file(open);
           if(file.size() == 0){
              QMessageBox::warning(this,"Error","Your File is clear!");
                return;
               }
           if(file.open(QFile::ReadOnly | QFile::Text)) {
               QTextStream in(&file);
               QString text = in.readAll();
               ui->plainTextEdit->setPlainText(text);
           }
           file.close();
}


void MainWindow::on_search_clicked()
{
    ui->Valuables->clear();
    ui->Prototypes->clear();
    ui->Classes->clear();
    ui->Coordinates->clear();
    ui->Depth->clear();
    auto str = ui->plainTextEdit->toPlainText().toStdString();
        parser pars(std::move(str));
        for(auto v : pars.GetFuncs())
        {
            ui->Prototypes->addItem("Prototypes "+QString::fromStdString(v.GetPrototype() + "\nOverrides " + v.GetOverloadedFunctions()));
        }
        for(auto v : pars.GetVals())
        {
            ui->Valuables->addItem("Valuables "+QString::fromStdString(v.GetType() + " " + v.GetName() + " = " +
                                                          v.GetDefaultValue() + " " + v.GetVisibilityArea() + "\n" + v.GetPositions()));
        }
        for(auto v : pars.GetBranchesPositions())
        {
            ui->Depth->addItem("Raw:"+QString::number(v.first) + " " + ",deep of branches: "+ QString::number(v.second) + "\n");
        }
        for(auto v : pars.GetChangePositions()){
            ui->Coordinates->addItem("Changing valuables\nColumn :" + QString::number(v.first) + " ,Raw :" + QString::number(v.second) + "\n");
        }
        ui->Classes->addItem
                (
                "Count of classes : " +QString::number(pars.GetComplexStructs()[0]) + "\n" +
                "Count of Str : " +QString::number(pars.GetComplexStructs()[1]) + "\n" +
                "Dynamic arrays : " +QString::number(pars.GetComplexStructs()[2]) + "\n" +
                "Static arrays : " +QString::number(pars.GetComplexStructs()[3]) + "\n"
                );
        std::pair<int, int> errors = pars.GetErrors();
        if (errors.first == -1 && errors.second == -1) {
            ui->Errors->addItem("No infinite loops found.");
        } else {
            ui->Errors->addItem(QString::fromStdString("Infinite loop on raw: " + std::to_string(errors.first) + ", symbol " + std::to_string(errors.second) + "."));
            }
}


void MainWindow::on_pushButton_2_clicked()
{
    close();
}

