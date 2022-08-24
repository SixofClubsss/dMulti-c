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
#include "./ui_mainwindow.h"
#include "rpc/rpc.h"

std::string MainWindow::rpcLogin;
QString MainWindow::daemonAddress;
QString MainWindow::walletAddress;
QString MainWindow::contractAddress;


int rpc::getDeroBalance()  /// Gets players dero balance
{
    CURL *curlBalanceCheck;
    CURLcode res;
    string balanceReadBuffer;
    char error[CURL_ERROR_SIZE];

    static const char *postthis = "{\"jsonrpc\":\"2.0\",\"id\": \"1\",\"method\": \"GetBalance\"}";

    string dbStr = MainWindow::walletAddress.toStdString();  /// Get wallet rpc address
    const char *dbCh = dbStr.c_str();

    const char *loginCh = MainWindow::rpcLogin.c_str();  /// Get wallet user:pass

    curlBalanceCheck = curl_easy_init();

    if(curlBalanceCheck) {
      struct curl_slist *headers = NULL;
      /// Add request headers
      headers = curl_slist_append(headers, "Accept: application/json");
      headers = curl_slist_append(headers, "Content-Type: application/json");
      headers = curl_slist_append(headers, "charset: utf-8");
      /// cUrl options
      curl_easy_setopt(curlBalanceCheck, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curlBalanceCheck, CURLOPT_URL, dbCh);
      curl_easy_setopt(curlBalanceCheck, CURLOPT_VERBOSE, 0L);
      curl_easy_setopt(curlBalanceCheck, CURLOPT_CONNECTTIMEOUT, 4L);
      curl_easy_setopt(curlBalanceCheck, CURLOPT_ERRORBUFFER, error);
      curl_easy_setopt(curlBalanceCheck, CURLOPT_USERPWD, loginCh);
      curl_easy_setopt(curlBalanceCheck, CURLOPT_POSTFIELDS, postthis);
      curl_easy_setopt(curlBalanceCheck, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));
      curl_easy_setopt(curlBalanceCheck, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curlBalanceCheck, CURLOPT_WRITEDATA, &balanceReadBuffer);

      res = curl_easy_perform(curlBalanceCheck);
      curl_easy_cleanup(curlBalanceCheck);

      QByteArray br = balanceReadBuffer.c_str();        /// Work with result data
      QJsonDocument cbDoc = QJsonDocument::fromJson(br);
      QJsonObject cbObj = cbDoc.object();
      QJsonObject cbResults = cbObj["result"].toObject();
      QJsonValue Balance_jv = cbResults.value("balance");

      rpc::deroBalance = Balance_jv.toDouble();

    }
    return 0;
}


int rpc::getAssetBalance()  /// Gets players dero balance
{
    CURL *curlBalanceCheck;
    CURLcode res;
    string balanceReadBuffer;
    char error[CURL_ERROR_SIZE];

    QString parts = "{\"jsonrpc\":\"2.0\",\"id\": \"1\",\"method\": \"GetBalance\", \"params\": {\"scid\": \""+MainWindow::contractAddress+"\"}}";
    string addThis = parts.toStdString();
    const char *postthis = addThis.c_str();

    string abStr = MainWindow::walletAddress.toStdString();
    const char *abCh = abStr.c_str();

    const char *loginCh = MainWindow::rpcLogin.c_str();

    curlBalanceCheck = curl_easy_init();

    if(curlBalanceCheck) {
      struct curl_slist *headers = NULL;

      headers = curl_slist_append(headers, "Accept: application/json");
      headers = curl_slist_append(headers, "Content-Type: application/json");
      headers = curl_slist_append(headers, "charset: utf-8");

      curl_easy_setopt(curlBalanceCheck, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curlBalanceCheck, CURLOPT_URL, abCh);
      curl_easy_setopt(curlBalanceCheck, CURLOPT_VERBOSE, 0L);
      curl_easy_setopt(curlBalanceCheck, CURLOPT_CONNECTTIMEOUT, 4L);
      curl_easy_setopt(curlBalanceCheck, CURLOPT_ERRORBUFFER, error);
      curl_easy_setopt(curlBalanceCheck, CURLOPT_USERPWD, loginCh);
      curl_easy_setopt(curlBalanceCheck, CURLOPT_POSTFIELDS, postthis);
      curl_easy_setopt(curlBalanceCheck, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));
      curl_easy_setopt(curlBalanceCheck, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curlBalanceCheck, CURLOPT_WRITEDATA, &balanceReadBuffer);

      res = curl_easy_perform(curlBalanceCheck);
      curl_easy_cleanup(curlBalanceCheck);

      QByteArray br = balanceReadBuffer.c_str();
      QJsonDocument cbDoc = QJsonDocument::fromJson(br);
      QJsonObject cbObj = cbDoc.object();
      QJsonObject cbResults = cbObj["result"].toObject();
      QJsonValue AssetBalance_jv = cbResults.value("balance");

      rpc::assetBalance = AssetBalance_jv.toDouble();

    }
    return 0;
}


int MainWindow::checkWallet()  /// Confirm wallet is connected
{
    CURL *curlWalletCheck;
    CURLcode res;
    string readBuffer;
    char error[CURL_ERROR_SIZE];

    static const char *postthis = "{\"jsonrpc\":\"2.0\",\"id\":\"1\",\"method\":\"Echo\",\"params\":[\"Hello\", \"World\", \"!\"]}";

    string cwStr = walletAddress.toStdString();
    const char *cwCh = cwStr.c_str();

    const char *loginCh = MainWindow::rpcLogin.c_str();

    curlWalletCheck = curl_easy_init();

    if(curlWalletCheck) {
      struct curl_slist *headers = NULL;

      headers = curl_slist_append(headers, "Accept: application/json");
      headers = curl_slist_append(headers, "Content-Type: application/json");
      headers = curl_slist_append(headers, "charset: utf-8");

      curl_easy_setopt(curlWalletCheck, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curlWalletCheck, CURLOPT_URL, cwCh);
      curl_easy_setopt(curlWalletCheck, CURLOPT_VERBOSE, 0L);
      curl_easy_setopt(curlWalletCheck, CURLOPT_CONNECTTIMEOUT, 4L);
      curl_easy_setopt(curlWalletCheck, CURLOPT_ERRORBUFFER, error);
      curl_easy_setopt(curlWalletCheck, CURLOPT_USERPWD, loginCh);
      curl_easy_setopt(curlWalletCheck, CURLOPT_POSTFIELDS, postthis);
      curl_easy_setopt(curlWalletCheck, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));
      curl_easy_setopt(curlWalletCheck, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curlWalletCheck, CURLOPT_WRITEDATA, &readBuffer);
      res = curl_easy_perform(curlWalletCheck);
      curl_easy_cleanup(curlWalletCheck);

      QByteArray br = readBuffer.c_str();
      QJsonDocument cbDoc = QJsonDocument::fromJson(br);
      QJsonObject cbObj = cbDoc.object();
      QJsonValue okCheck = cbObj["result"];

      if(okCheck == "WALLET Hello World !"){
          ui->walletConnectedBox->setChecked(true);
      }else {
          ui->walletConnectedBox->setChecked(false);
      }

    }
    return 0;
}


int MainWindow::issueAsset()      /// Issues asset from contract
{
    CURL *curlIssueAsset;
    CURLcode res;
    string playerEntryReadBuffer;
    char error[CURL_ERROR_SIZE];

    QString yn;
    QString address = ui->assetReceiverLineEdit->text();
    QString amount = QString::number(ui->assetAmountSpinBox->value()*100000, 'g', 100);

    if(ui->assetCheckBox->isChecked()){
        yn = QString::number(1);
    }else {
        yn = QString::number(0);
    }

    QString parts = "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"scinvoke\",\"params\":{\"scid\":\""+contractAddress+"\", \"ringsize\":2 , \"sc_rpc\":[{\"name\":\"entrypoint\",\"datatype\":\"S\",\"value\":\"SendAsset\"}, {\"name\":\"address\",\"datatype\":\"S\",\"value\":\""+address+"\"}, {\"name\":\"amount\",\"datatype\":\"U\",\"value\":"+amount+"}, {\"name\":\"ynBool\",\"datatype\":\"U\",\"value\":"+yn+"}] }}";
    string addThis = parts.toStdString();
    const char *postthis = addThis.c_str();

    string issueStr = walletAddress.toStdString();
    const char *issueCh = issueStr.c_str();

    const char *loginCh = MainWindow::rpcLogin.c_str();

    curlIssueAsset = curl_easy_init();

    if(curlIssueAsset) {
        struct curl_slist *headers = NULL;

        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charset: utf-8");

        curl_easy_setopt(curlIssueAsset, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curlIssueAsset, CURLOPT_URL, issueCh);
        curl_easy_setopt(curlIssueAsset, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curlIssueAsset, CURLOPT_CONNECTTIMEOUT, 4L);
        curl_easy_setopt(curlIssueAsset, CURLOPT_ERRORBUFFER, error);
        curl_easy_setopt(curlIssueAsset, CURLOPT_USERPWD, loginCh);
        curl_easy_setopt(curlIssueAsset, CURLOPT_POSTFIELDS, postthis);
        curl_easy_setopt(curlIssueAsset, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));
        curl_easy_setopt(curlIssueAsset, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curlIssueAsset, CURLOPT_WRITEDATA, &playerEntryReadBuffer);

        res = curl_easy_perform(curlIssueAsset);
        curl_easy_cleanup(curlIssueAsset);

        QByteArray br = playerEntryReadBuffer.c_str();
        QJsonDocument cbDoc = QJsonDocument::fromJson(br);
        QJsonObject cbObj = cbDoc.object();
        QJsonObject cbResults = cbObj["result"].toObject();
        QJsonValue issueTxid = cbResults.value("txid");

        qInfo() << "Issue TXID:" << issueTxid.toString();

        if(issueTxid.isString()){
            ui->fundsTXID->append("Issue TXID: "+issueTxid.toString()+"\n");
        }

    }
    return 0;
}


int MainWindow::sendDero()      /// Sends Dero from contract balance
{
    CURL *curlSendDero;
    CURLcode res;
    string playerEntryReadBuffer;
    char error[CURL_ERROR_SIZE];

    QString yn;
    QString address = ui->deroReceiverLineEdit->text();
    QString amount = QString::number(ui->deroAmountSpinBox->value()*100000, 'g', 100);

    if(ui->deroCheckBox->isChecked()){
        yn = QString::number(1);
    }else {
        yn = QString::number(0);
    }

    QString parts = "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"scinvoke\",\"params\":{\"scid\":\""+contractAddress+"\", \"ringsize\":2 , \"sc_rpc\":[{\"name\":\"entrypoint\",\"datatype\":\"S\",\"value\":\"SendDero\"}, {\"name\":\"address\",\"datatype\":\"S\",\"value\":\""+address+"\"}, {\"name\":\"amount\",\"datatype\":\"U\",\"value\":"+amount+"}, {\"name\":\"ynBool\",\"datatype\":\"U\",\"value\":"+yn+"}] }}";
    string addThis = parts.toStdString();
    const char *postthis = addThis.c_str();

    string sendStr = walletAddress.toStdString();
    const char *sendCh = sendStr.c_str();

    const char *loginCh = MainWindow::rpcLogin.c_str();

    curlSendDero = curl_easy_init();

    if(curlSendDero) {
        struct curl_slist *headers = NULL;

        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charset: utf-8");

        curl_easy_setopt(curlSendDero, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curlSendDero, CURLOPT_URL, sendCh);
        curl_easy_setopt(curlSendDero, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curlSendDero, CURLOPT_CONNECTTIMEOUT, 4L);
        curl_easy_setopt(curlSendDero, CURLOPT_ERRORBUFFER, error);
        curl_easy_setopt(curlSendDero, CURLOPT_USERPWD, loginCh);
        curl_easy_setopt(curlSendDero, CURLOPT_POSTFIELDS, postthis);
        curl_easy_setopt(curlSendDero, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));
        curl_easy_setopt(curlSendDero, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curlSendDero, CURLOPT_WRITEDATA, &playerEntryReadBuffer);

        res = curl_easy_perform(curlSendDero);
        curl_easy_cleanup(curlSendDero);

        QByteArray br = playerEntryReadBuffer.c_str();
        QJsonDocument cbDoc = QJsonDocument::fromJson(br);
        QJsonObject cbObj = cbDoc.object();
        QJsonObject cbResults = cbObj["result"].toObject();
        QJsonValue sendTxid = cbResults.value("txid");

        qInfo() << "Send TXID:" << sendTxid.toString();

        if(sendTxid.isString()){
            ui->fundsTXID->append("Send TXID: "+sendTxid.toString()+"\n");
        }

    }
    return 0;
}


int MainWindow::deposit()      /// Deposit Dero or asset to contract
{
    if(ui->depositCheckBox->isChecked()){

        CURL *curlDeposit;
        CURLcode res;
        string playerEntryReadBuffer;
        char error[CURL_ERROR_SIZE];

        QString parts;
        QString address = ui->depositLineEdit->text();
        QString amount = QString::number(ui->depositAmountSpinBox->value()*100000, 'g', 100);

        if(address == "" || ui->depositLineEdit->text().length() != 64){
            parts = "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"scinvoke\",\"params\":{\"scid\":\""+contractAddress+"\", \"sc_dero_deposit\":"+amount+", \"ringsize\":2, \"sc_rpc\":[{\"name\":\"entrypoint\",\"datatype\":\"S\",\"value\":\"OwnerDeposit\"}] }}";
        }else {
            parts = "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"scinvoke\",\"params\":{\"scid\":\""+contractAddress+"\", \"sc_token_deposit\":"+amount+", \"ringsize\":2, \"sc_rpc\":[{\"name\":\"entrypoint\",\"datatype\":\"S\",\"value\":\"OwnerDeposit\"}] }}";
        }

        string addThis = parts.toStdString();
        const char *postthis = addThis.c_str();

        string depositStr = walletAddress.toStdString();
        const char *depositCh = depositStr.c_str();

        const char *loginCh = MainWindow::rpcLogin.c_str();

        curlDeposit = curl_easy_init();

        if(curlDeposit) {
            struct curl_slist *headers = NULL;

            headers = curl_slist_append(headers, "Accept: application/json");
            headers = curl_slist_append(headers, "Content-Type: application/json");
            headers = curl_slist_append(headers, "charset: utf-8");

            curl_easy_setopt(curlDeposit, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curlDeposit, CURLOPT_URL, depositCh);
            curl_easy_setopt(curlDeposit, CURLOPT_VERBOSE, 0L);
            curl_easy_setopt(curlDeposit, CURLOPT_CONNECTTIMEOUT, 4L);
            curl_easy_setopt(curlDeposit, CURLOPT_ERRORBUFFER, error);
            curl_easy_setopt(curlDeposit, CURLOPT_USERPWD, loginCh);
            curl_easy_setopt(curlDeposit, CURLOPT_POSTFIELDS, postthis);
            curl_easy_setopt(curlDeposit, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));
            curl_easy_setopt(curlDeposit, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curlDeposit, CURLOPT_WRITEDATA, &playerEntryReadBuffer);

            res = curl_easy_perform(curlDeposit);
            curl_easy_cleanup(curlDeposit);

            QByteArray br = playerEntryReadBuffer.c_str();
            QJsonDocument cbDoc = QJsonDocument::fromJson(br);
            QJsonObject cbObj = cbDoc.object();
            QJsonObject cbResults = cbObj["result"].toObject();
            QJsonValue depositTxid = cbResults.value("txid");

            qInfo() << "Deposit TXID:" << depositTxid.toString();

            if(depositTxid.isString()){
                ui->fundsTXID->append("Deposit TXID: "+depositTxid.toString()+"\n");

            }

        }
    }

    return 0;
}


int MainWindow::removeOwner()      /// Replace a owner
{
    CURL *curlRemove;
    CURLcode res;
    string playerEntryReadBuffer;
    char error[CURL_ERROR_SIZE];

    QString yn;
    QString address = ui->removeLineEdit->text();
    QString num = QString::number(ui->removeSpinBox->value());

    if(ui->removeCheckBox->isChecked()){
        yn = QString::number(1);
    }else {
        yn = QString::number(0);
    }

    QString parts = "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"scinvoke\",\"params\":{\"scid\":\""+contractAddress+"\", \"ringsize\":2 , \"sc_rpc\":[{\"name\":\"entrypoint\",\"datatype\":\"S\",\"value\":\"RemoveOwner\"}, {\"name\":\"newOwner\",\"datatype\":\"S\",\"value\":\""+address+"\"}, {\"name\":\"ownerNum\",\"datatype\":\"U\",\"value\":"+num+"}, {\"name\":\"ynBool\",\"datatype\":\"U\",\"value\":"+yn+"}] }}";
    string addThis = parts.toStdString();
    const char *postthis = addThis.c_str();

    string removeStr = walletAddress.toStdString();
    const char *removeCh = removeStr.c_str();

    const char *loginCh = MainWindow::rpcLogin.c_str();

    curlRemove = curl_easy_init();

    if(curlRemove) {
        struct curl_slist *headers = NULL;

        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charset: utf-8");

        curl_easy_setopt(curlRemove, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curlRemove, CURLOPT_URL, removeCh);
        curl_easy_setopt(curlRemove, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curlRemove, CURLOPT_CONNECTTIMEOUT, 4L);
        curl_easy_setopt(curlRemove, CURLOPT_ERRORBUFFER, error);
        curl_easy_setopt(curlRemove, CURLOPT_USERPWD, loginCh);
        curl_easy_setopt(curlRemove, CURLOPT_POSTFIELDS, postthis);
        curl_easy_setopt(curlRemove, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));
        curl_easy_setopt(curlRemove, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curlRemove, CURLOPT_WRITEDATA, &playerEntryReadBuffer);

        res = curl_easy_perform(curlRemove);
        curl_easy_cleanup(curlRemove);

        QByteArray br = playerEntryReadBuffer.c_str();
        QJsonDocument cbDoc = QJsonDocument::fromJson(br);
        QJsonObject cbObj = cbDoc.object();
        QJsonObject cbResults = cbObj["result"].toObject();
        QJsonValue removeTxid = cbResults.value("txid");

        qInfo() << "Remove TXID:" << removeTxid.toString();

        if(removeTxid.isString()){
            ui->actionsTXID->append("Remove TXID: "+removeTxid.toString()+"\n");
        }

    }
    return 0;
}


int MainWindow::addOwner()      /// Adds new owner and vote on consensus
{
    CURL *curlAdd;
    CURLcode res;
    string playerEntryReadBuffer;
    char error[CURL_ERROR_SIZE];

    QString yn;
    QString address = ui->addLineEdit->text();
    QString num = QString::number(ui->addSpinBox->value());

    if(ui->addCheckBox->isChecked()){
        yn = QString::number(1);
    }else {
        yn = QString::number(0);
    }

    QString parts = "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"scinvoke\",\"params\":{\"scid\":\""+contractAddress+"\", \"ringsize\":2 , \"sc_rpc\":[{\"name\":\"entrypoint\",\"datatype\":\"S\",\"value\":\"AddOwner\"}, {\"name\":\"newOwner\",\"datatype\":\"S\",\"value\":\""+address+"\"}, {\"name\":\"consensus\",\"datatype\":\"U\",\"value\":"+num+"}, {\"name\":\"ynBool\",\"datatype\":\"U\",\"value\":"+yn+"}] }}";
    string addThis = parts.toStdString();
    const char *postthis = addThis.c_str();

    string addStr = walletAddress.toStdString();
    const char *addCh = addStr.c_str();

    const char *loginCh = MainWindow::rpcLogin.c_str();

    curlAdd = curl_easy_init();

    if(curlAdd) {
        struct curl_slist *headers = NULL;

        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charset: utf-8");

        curl_easy_setopt(curlAdd, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curlAdd, CURLOPT_URL, addCh);
        curl_easy_setopt(curlAdd, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curlAdd, CURLOPT_CONNECTTIMEOUT, 4L);
        curl_easy_setopt(curlAdd, CURLOPT_ERRORBUFFER, error);
        curl_easy_setopt(curlAdd, CURLOPT_USERPWD, loginCh);
        curl_easy_setopt(curlAdd, CURLOPT_POSTFIELDS, postthis);
        curl_easy_setopt(curlAdd, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));
        curl_easy_setopt(curlAdd, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curlAdd, CURLOPT_WRITEDATA, &playerEntryReadBuffer);

        res = curl_easy_perform(curlAdd);
        curl_easy_cleanup(curlAdd);

        QByteArray br = playerEntryReadBuffer.c_str();
        QJsonDocument cbDoc = QJsonDocument::fromJson(br);
        QJsonObject cbObj = cbDoc.object();
        QJsonObject cbResults = cbObj["result"].toObject();
        QJsonValue addTxid = cbResults.value("txid");

        qInfo() << "Add TXID:" << addTxid.toString();

        if(addTxid.isString()){
            ui->actionsTXID->append("Add TXID: "+addTxid.toString()+"\n");
        }

    }
    return 0;
}


int MainWindow::updateContract()      /// Adds new owner and vote on consensus
{
    CURL *curlUpdate;
    CURLcode res;
    string playerEntryReadBuffer;
    char error[CURL_ERROR_SIZE];

    QString yn;
    QString parts;
    QString data = ui->updateTextEdit->toPlainText();

    if(ui->updateCheckBox->isChecked()){
        yn = QString::number(1);
    }else {
        yn = QString::number(0);
    }

    parts = "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"transfer\",\"params\":{\"transfers\":[{\"destination\":\"deto1qyre7td6x9r88y4cavdgpv6k7lvx6j39lfsx420hpvh3ydpcrtxrxqg8v8e3z\", \"burn\":0}] , \"fees\":20000 , \"scid\":\""+contractAddress+"\", \"ringsize\":2 , \"sc_rpc\":[{\"name\":\"entrypoint\",\"datatype\":\"S\",\"value\":\"UpdateCode\"}, {\"name\":\"code\",\"datatype\":\"S\",\"value\":\""+data+"\"}, {\"name\":\"ynBool\",\"datatype\":\"U\",\"value\":"+yn+"}] }}";
    string addThis = parts.toStdString();
    const char *postthis = addThis.c_str();

    string updateStr = walletAddress.toStdString();
    const char *updateCh = updateStr.c_str();

    const char *loginCh = rpcLogin.c_str();

    curlUpdate = curl_easy_init();

    if(curlUpdate) {
        struct curl_slist *headers = NULL;

        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charset: utf-8");

        curl_easy_setopt(curlUpdate, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curlUpdate, CURLOPT_URL, updateCh);
        curl_easy_setopt(curlUpdate, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curlUpdate, CURLOPT_CONNECTTIMEOUT, 4L);
        curl_easy_setopt(curlUpdate, CURLOPT_ERRORBUFFER, error);
        curl_easy_setopt(curlUpdate, CURLOPT_USERPWD, loginCh);
        curl_easy_setopt(curlUpdate, CURLOPT_POSTFIELDS, postthis);
        curl_easy_setopt(curlUpdate, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));
        curl_easy_setopt(curlUpdate, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curlUpdate, CURLOPT_WRITEDATA, &playerEntryReadBuffer);

        res = curl_easy_perform(curlUpdate);
        curl_easy_cleanup(curlUpdate);

        QByteArray br = playerEntryReadBuffer.c_str();
        QJsonDocument cbDoc = QJsonDocument::fromJson(br);
        QJsonObject cbObj = cbDoc.object();
        QJsonObject cbResults = cbObj["result"].toObject();
        QJsonValue updateTxid = cbResults.value("txid");

        qInfo() << "Update TXID:" << updateTxid.toString();

        if(updateTxid.isString()){
            ui->actionsTXID->append("Update TXID: "+updateTxid.toString()+"\n");
        }

    }
    return 0;
}



