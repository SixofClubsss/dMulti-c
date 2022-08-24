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


void MainWindow::loginInfo()   /// Used to get wallet login info for wallet functions
{
    QString userInfo = ui->userpassInput->text();
    string userpassStr = userInfo.toStdString();
    rpcLogin = userpassStr;
}

void MainWindow::disableGroupBoxes()
{
    ui->assetGroupBox->setEnabled(false);
    ui->deroGroupBox->setEnabled(false);
    ui->depositGroupBox->setEnabled(false);
    ui->removeGroupBox->setEnabled(false);
    ui->addGroupBox->setEnabled(false);
    ui->updateGroupBox->setEnabled(false);
}


void MainWindow::setGroupBoxes()
{
    if(rpc::connected == true && ui->contractCheckBox->isChecked() && ui->walletConnectedBox->isChecked() && ui->walletDeroDisplay->value() != 0 && ui->heightDisplay->value() > 1){
        ui->assetGroupBox->setEnabled(true);
        ui->deroGroupBox->setEnabled(true);
        ui->depositGroupBox->setEnabled(true);
        ui->removeGroupBox->setEnabled(true);
        ui->addGroupBox->setEnabled(true);
        ui->updateGroupBox->setEnabled(true);
    }

    if(rpc::deroAmt > 0){
        ui->assetGroupBox->setEnabled(false);
        ui->depositGroupBox->setEnabled(false);
        ui->removeGroupBox->setEnabled(false);
        ui->addGroupBox->setEnabled(false);
        ui->updateGroupBox->setEnabled(false);
    }

    if(rpc::assetAmt > 0){
        ui->deroGroupBox->setEnabled(false);
        ui->depositGroupBox->setEnabled(false);
        ui->removeGroupBox->setEnabled(false);
        ui->addGroupBox->setEnabled(false);
        ui->updateGroupBox->setEnabled(false);
    }

    if(rpc::remove > 0){
        ui->assetGroupBox->setEnabled(false);
        ui->deroGroupBox->setEnabled(false);
        ui->depositGroupBox->setEnabled(false);
        ui->addGroupBox->setEnabled(false);
        ui->updateGroupBox->setEnabled(false);
    }

    if(rpc::new_required > 0){
        ui->assetGroupBox->setEnabled(false);
        ui->deroGroupBox->setEnabled(false);
        ui->depositGroupBox->setEnabled(false);
        ui->removeGroupBox->setEnabled(false);
        ui->updateGroupBox->setEnabled(false);
    }

    if(rpc::update > 0){
        ui->assetGroupBox->setEnabled(false);
        ui->deroGroupBox->setEnabled(false);
        ui->depositGroupBox->setEnabled(false);
        ui->removeGroupBox->setEnabled(false);
        ui->addGroupBox->setEnabled(false);
    }
}

void MainWindow::currentEvent()
{
    if(rpc::deroAmt > 0){
        ui->contractTextBrowser->append("Dero Transaction Pending, Amount: "+QString::number(rpc::deroAmt/100000)+"\nReceiver: "+rpc::receiver+"\n");
    }

    if(rpc::assetAmt > 0){
        ui->contractTextBrowser->append("Asset Transaction Pending, Amount: "+QString::number(rpc::assetAmt/100000)+"\nReceiver: "+rpc::receiver+"\n");
    }

    if(rpc::remove > 0){
        ui->contractTextBrowser->append("Remove Transaction Pending, Owner Numer: "+QString::number(rpc::remove)+"\nNew Owner: "+rpc::new_owner+"\n");
    }

    if(rpc::new_required > 0){
        ui->contractTextBrowser->append("Add Transaction Pending,\nNew Owner: "+rpc::new_owner+"\n");
    }

    if(rpc::update > 0){
        ui->contractTextBrowser->append("Update Transaction Pending,\n");
    }
}


QString MainWindow::signedText(int s)
{
    if(s == 1){
        return "Yes";
    }else {
        return "No";
    }
}


void MainWindow::printDetails()
{
    if(ui->contractCheckBox->isChecked() && rpc::owner1 != ""){
        ui->contractTextBrowser->setText("Co Signers: "+QString::number(rpc::co_signers)+"\n");
        currentEvent();
        ui->contractTextBrowser->append("Signatures Required: "+QString::number(rpc::signatures_required)+"\n");
        ui->contractTextBrowser->append("Signature Count: "+QString::number(rpc::signature_count)+"\n");
        ui->contractTextBrowser->append("Rejected Count: "+QString::number(rpc::reject_count)+"\n");
        ui->contractTextBrowser->append("Authorized Transactions: "+QString::number(rpc::tx_count)+"\n");

        ui->contractTextBrowser->append("Owner 1: "+rpc::owner1+" - Signed: "+signedText(rpc::owner1_signed));
        ui->contractTextBrowser->append("Owner 2: "+rpc::owner2+" - Signed: "+signedText(rpc::owner2_signed));
        if(rpc::co_signers >= 3){
            ui->contractTextBrowser->append("Owner 3: "+rpc::owner3+" - Signed: "+signedText(rpc::owner3_signed));
        }
        if(rpc::co_signers >= 4){
            ui->contractTextBrowser->append("Owner 4: "+rpc::owner4+" - Signed: "+signedText(rpc::owner4_signed));
        }

        if(rpc::co_signers >= 5){
            ui->contractTextBrowser->append("Owner 5: "+rpc::owner5+" - Signed: "+signedText(rpc::owner5_signed));
        }
        if(rpc::co_signers >= 6){
            ui->contractTextBrowser->append("Owner 6: "+rpc::owner6+" - Signed: "+signedText(rpc::owner6_signed));
        }

    }
}
