/*
dMulti-c

Copyright (C) 2022  SixofClubs

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

https://github.com/SixofClubsss
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->daemonConnectedBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->daemonConnectedBox->setFocusPolicy(Qt::NoFocus);
    ui->walletConnectedBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->walletConnectedBox->setFocusPolicy(Qt::NoFocus);
    ui->contractCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->contractCheckBox->setFocusPolicy(Qt::NoFocus);
    ui->assetGroupBox->setEnabled(false);
    ui->deroGroupBox->setEnabled(false);
    ui->depositGroupBox->setEnabled(false);
    ui->removeGroupBox->setEnabled(false);
    ui->addGroupBox->setEnabled(false);
    ui->updateGroupBox->setEnabled(false);

    connect(ui->daemonRPCinput, SIGNAL(textChanged(QString)),
          this, SLOT(toggle()));

    connect(ui->walletRPCinput, SIGNAL(textChanged(QString)),
          this, SLOT(toggle()));

    connect(ui->userpassInput, SIGNAL(textChanged(QString)),
          this, SLOT(toggle()));

    connect(ui->contractLineEdit, SIGNAL(textChanged(QString)),
              this, SLOT(toggle()));
}

MainWindow::~MainWindow()
{
    Worker::workThread.quit();
    delete ui;
}


void MainWindow::refresh()      /// Controller refresh rate
{
    setGroupBoxes();
    ui->heightDisplay->setValue(rpc::blockHeight);
    ui->walletDeroDisplay->setValue(rpc::deroBalance/100000);
    ui->walletAssetDisplay->setValue(rpc::assetBalance/100000);
    ui->assetIssuedDisplay->setValue(rpc::supplyIssued/100000);
    ui->maxSupplyDisplay->setValue(rpc::maxSupply/100000);
    ui->deroDisplay->setValue(rpc::contractDeroBalance/100000);
    printDetails();
}


void MainWindow::on_connectButton_clicked()
{
    daemonAddress = ui->daemonRPCinput->text()+"/json_rpc";
    checkDaemon();
    loginInfo();
    walletAddress = ui->walletRPCinput->text()+"/json_rpc";
    checkWallet();
    contractAddress = ui->contractLineEdit->text();
    checkContract();
}


void MainWindow::toggle()
{
    daemonAddress =  ui->daemonRPCinput->text();
    ui->daemonConnectedBox->setChecked(false);

    walletAddress = ui->walletRPCinput->text();
    loginInfo();
    ui->walletConnectedBox->setChecked(false);

    contractAddress = ui->contractLineEdit->text();
    ui->contractCheckBox->setChecked(false);

    rpc::blockHeight = 0;
    ui->heightDisplay->setValue(0);
    disableGroupBoxes();
}



void MainWindow::on_sendAssetButton_clicked()
{
    issueAsset();
}


void MainWindow::on_sendDeroButton_clicked()
{
    sendDero();
}


void MainWindow::on_depositoButton_clicked()
{
    deposit();
}


void MainWindow::on_removeButton_clicked()
{
    removeOwner();
}


void MainWindow::on_addButton_clicked()
{
    addOwner();
}


void MainWindow::on_updateButton_clicked()
{
    updateContract();
}


void MainWindow::on_assetCheckBox_stateChanged(int arg1)
{
    if(ui->assetCheckBox->isChecked()){
        ui->assetCheckBox->setStyleSheet("QCheckBox{background-color: rgba(0, 158, 0, 120);}");
    }else {
        ui->assetCheckBox->setStyleSheet("QCheckBox{color: rgb(255, 255, 255);}");
    }
}

void MainWindow::on_deroCheckBox_stateChanged(int arg1)
{
    if(ui->deroCheckBox->isChecked()){
        ui->deroCheckBox->setStyleSheet("QCheckBox{background-color: rgba(0, 158, 0, 120);}");
    }else {
        ui->deroCheckBox->setStyleSheet("QCheckBox{color: rgb(255, 255, 255);}");
    }
}


void MainWindow::on_depositCheckBox_stateChanged(int arg1)
{
    if(ui->depositCheckBox->isChecked()){
        ui->depositCheckBox->setStyleSheet("QCheckBox{background-color: rgba(0, 158, 0, 120);}");
    }else {
        ui->depositCheckBox->setStyleSheet("QCheckBox{color: rgb(255, 255, 255);}");
    }
}


void MainWindow::on_removeCheckBox_stateChanged(int arg1)
{
    if(ui->removeCheckBox->isChecked()){
        ui->removeCheckBox->setStyleSheet("QCheckBox{background-color: rgba(0, 158, 0, 120);}");
    }else {
        ui->removeCheckBox->setStyleSheet("QCheckBox{color: rgb(255, 255, 255);}");
    }
}


void MainWindow::on_addCheckBox_stateChanged(int arg1)
{
    if(ui->addCheckBox->isChecked()){
        ui->addCheckBox->setStyleSheet("QCheckBox{background-color: rgba(0, 158, 0, 120);}");
    }else {
        ui->addCheckBox->setStyleSheet("QCheckBox{color: rgb(255, 255, 255);}");
    }
}


void MainWindow::on_updateCheckBox_stateChanged(int arg1)
{
    if(ui->updateCheckBox->isChecked()){
        ui->updateCheckBox->setStyleSheet("QCheckBox{background-color: rgba(0, 158, 0, 120);}");
    }else {
        ui->updateCheckBox->setStyleSheet("QCheckBox{color: rgb(255, 255, 255);}");
    }
}
