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

#include "iostream"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "rpc/rpc.h"


bool rpc::connected;
QString rpc::owner1;
QString rpc::owner2;
QString rpc::owner3;
QString rpc::owner4;
QString rpc::owner5;
QString rpc::owner6;
int rpc::owner1_signed;
int rpc::owner2_signed;
int rpc::owner3_signed;
int rpc::owner4_signed;
int rpc::owner5_signed;
int rpc::owner6_signed;

double rpc::deroBalance;
double rpc::assetBalance;
double rpc::blockHeight;
double rpc::contractDeroBalance;
double rpc::maxSupply;
double rpc::supplyIssued;
int rpc::co_signers;
int rpc::signature_count;
int rpc::signatures_required;
int rpc::reject_count;
int rpc::tx_count;

double rpc::deroAmt;
double rpc::assetAmt;
int rpc::update;
int rpc::remove;
int rpc::new_required;
QString rpc::new_owner;
QString rpc::receiver;



int MainWindow::checkDaemon()       /// Check connection to daemon
{
    CURL *curlDaemonCheck;
    CURLcode res;
    string daemonReadBuffer;
    char error[CURL_ERROR_SIZE];

    QString parts = "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"getsc\",\"params\":{ \"scid\":\""+contractAddress+"\" , \"ringsize\":2 , \"code\":false , \"variables\":false}}";
    string addThis = parts.toStdString();
    const char *postthis = addThis.c_str();

    string dStr = daemonAddress.toStdString();   /// Get dameon address from menu
    const char *dCh = dStr.c_str();

    curlDaemonCheck = curl_easy_init();

    if(curlDaemonCheck) {
      struct curl_slist *headers = NULL;
      /// Add request headers
      headers = curl_slist_append(headers, "Accept: application/json");
      headers = curl_slist_append(headers, "Content-Type: application/json");
      headers = curl_slist_append(headers, "charset: utf-8");
      /// cUrl options
      curl_easy_setopt(curlDaemonCheck, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curlDaemonCheck, CURLOPT_URL, dCh);
      curl_easy_setopt(curlDaemonCheck, CURLOPT_VERBOSE, 0L);
      curl_easy_setopt(curlDaemonCheck, CURLOPT_CONNECTTIMEOUT, 4L);
      curl_easy_setopt(curlDaemonCheck, CURLOPT_ERRORBUFFER, error);
      /// curl_easy_setopt(curlDaemonCheck, CURLOPT_SSL_VERIFYPEER, 0);   *Remove comment for windows SSL disable*
      curl_easy_setopt(curlDaemonCheck, CURLOPT_POSTFIELDS, postthis);
      curl_easy_setopt(curlDaemonCheck, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));
      curl_easy_setopt(curlDaemonCheck, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curlDaemonCheck, CURLOPT_WRITEDATA, &daemonReadBuffer);

      res = curl_easy_perform(curlDaemonCheck);
      curl_easy_cleanup(curlDaemonCheck);

      QByteArray br = daemonReadBuffer.c_str();             /// Work with result data
      QJsonDocument cbDoc = QJsonDocument::fromJson(br);
      QJsonObject cbObj = cbDoc.object();
      QJsonObject cbResults = cbObj["result"].toObject();
      QJsonValue okCheck = cbResults["status"];

      if(okCheck == "OK"){
          ui->daemonConnectedBox->setChecked(true);
      }else {
          ui->daemonConnectedBox->setChecked(false);
      }

    }
    return 0;

}


int MainWindow::checkContract()       /// Check if contract is valid
{
    CURL *curlContract;
    CURLcode res;
    string contractReadBuffer;
    char error[CURL_ERROR_SIZE];

    QString parts = "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"getsc\",\"params\":{ \"scid\":\""+contractAddress+"\" , \"ringsize\":2 , \"code\":false , \"variables\":true}}";
    string addThis = parts.toStdString();
    const char *postthis = addThis.c_str();

    string ccStr = daemonAddress.toStdString();
    const char *dCh = ccStr.c_str();

    curlContract = curl_easy_init();

    if(curlContract) {
      struct curl_slist *headers = NULL;

      headers = curl_slist_append(headers, "Accept: application/json");
      headers = curl_slist_append(headers, "Content-Type: application/json");
      headers = curl_slist_append(headers, "charset: utf-8");
      /// cUrl options
      curl_easy_setopt(curlContract, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curlContract, CURLOPT_URL, dCh);
      curl_easy_setopt(curlContract, CURLOPT_VERBOSE, 0L);
      curl_easy_setopt(curlContract, CURLOPT_CONNECTTIMEOUT, 4L);
      curl_easy_setopt(curlContract, CURLOPT_ERRORBUFFER, error);
      curl_easy_setopt(curlContract, CURLOPT_POSTFIELDS, postthis);
      /// curl_easy_setopt(curlContract, CURLOPT_SSL_VERIFYPEER, 0);   *Remove comment for windows SSL disable*
      curl_easy_setopt(curlContract, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));
      curl_easy_setopt(curlContract, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curlContract, CURLOPT_WRITEDATA, &contractReadBuffer);

      res = curl_easy_perform(curlContract);
      curl_easy_cleanup(curlContract);

      QByteArray br = contractReadBuffer.c_str();
      QJsonDocument cbDoc = QJsonDocument::fromJson(br);
      QJsonObject cbObj = cbDoc.object();
      QJsonObject cbResults = cbObj["result"].toObject();
      QJsonObject cbStringKeys = cbResults["stringkeys"].toObject();
      QJsonValue consensus = cbStringKeys.value("signatures_required");
      QJsonValue owner1 = cbStringKeys.value("owner1");

      if(owner1.isString() && consensus.toInt() >= 1){
          ui->contractCheckBox->setChecked(true);
      }else {
          ui->contractCheckBox->setChecked(false);
      }

    }
    return 0;

}


int rpc::getHeight()  /// Gets current block height
{
    CURL *curlHeightCheck;
    CURLcode res;
    string heightReadBuffer;
    char error[CURL_ERROR_SIZE];

    static const char *postthis = "{\"jsonrpc\":\"2.0\",\"id\": \"1\",\"method\": \"DERO.GetHeight\"}";

    string fhStr = MainWindow::daemonAddress.toStdString();
    const char *fhCh = fhStr.c_str();

    curlHeightCheck = curl_easy_init();

    if(curlHeightCheck) {
      struct curl_slist *headers = NULL;

      headers = curl_slist_append(headers, "Accept: application/json");
      headers = curl_slist_append(headers, "Content-Type: application/json");
      headers = curl_slist_append(headers, "charset: utf-8");

      curl_easy_setopt(curlHeightCheck, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curlHeightCheck, CURLOPT_URL, fhCh);
      curl_easy_setopt(curlHeightCheck, CURLOPT_VERBOSE, 0L);
      curl_easy_setopt(curlHeightCheck, CURLOPT_CONNECTTIMEOUT, 4L);
      curl_easy_setopt(curlHeightCheck, CURLOPT_ERRORBUFFER, error);
      /// curl_easy_setopt(curlHeightCheck, CURLOPT_SSL_VERIFYPEER, 0);   *Remove comment for windows SSL disable*
      curl_easy_setopt(curlHeightCheck, CURLOPT_POSTFIELDS, postthis);
      curl_easy_setopt(curlHeightCheck, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));
      curl_easy_setopt(curlHeightCheck, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curlHeightCheck, CURLOPT_WRITEDATA, &heightReadBuffer);
      res = curl_easy_perform(curlHeightCheck);
      curl_easy_cleanup(curlHeightCheck);

      QByteArray br = heightReadBuffer.c_str();
      QJsonDocument cbDoc = QJsonDocument::fromJson(br);
      QJsonObject cbObj = cbDoc.object();
      QJsonObject cbResults = cbObj["result"].toObject();
      QJsonValue Height_jv = cbResults.value("height");

      rpc::blockHeight = Height_jv.toDouble();

    }
    return 0;
}


int rpc::fetchScData()       /// Get SC variables
{
    CURL *curlFetch;
    CURLcode res;
    string fetchReadBuffer;
    char error[CURL_ERROR_SIZE];

    QString parts = "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"getsc\",\"params\":{ \"scid\":\""+MainWindow::contractAddress+"\" , \"ringsize\":2 , \"code\":false , \"variables\":true}}";
    string addThis = parts.toStdString();
    const char *postthis = addThis.c_str();

    string fdStr = MainWindow::daemonAddress.toStdString();
    const char *fdCh = fdStr.c_str();

    curlFetch = curl_easy_init();

    if(curlFetch) {
      struct curl_slist *headers = NULL;

      headers = curl_slist_append(headers, "Accept: application/json");
      headers = curl_slist_append(headers, "Content-Type: application/json");
      headers = curl_slist_append(headers, "charset: utf-8");
      /// cUrl options
      curl_easy_setopt(curlFetch, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curlFetch, CURLOPT_URL, fdCh);
      curl_easy_setopt(curlFetch, CURLOPT_VERBOSE, 0L);
      curl_easy_setopt(curlFetch, CURLOPT_CONNECTTIMEOUT, 4L);
      curl_easy_setopt(curlFetch, CURLOPT_ERRORBUFFER, error);
      /// curl_easy_setopt(curlFetch, CURLOPT_SSL_VERIFYPEER, 0);   *Remove comment for windows SSL disable*
      curl_easy_setopt(curlFetch, CURLOPT_POSTFIELDS, postthis);
      curl_easy_setopt(curlFetch, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));
      curl_easy_setopt(curlFetch, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curlFetch, CURLOPT_WRITEDATA, &fetchReadBuffer);

      res = curl_easy_perform(curlFetch);
      curl_easy_cleanup(curlFetch);

      QByteArray brw = fetchReadBuffer.c_str();
      QJsonDocument cbDoc = QJsonDocument::fromJson(brw);
      QJsonObject cbObj = cbDoc.object();
      QJsonObject cbResults = cbObj["result"].toObject();

      QJsonObject cbBalances = cbResults["balances"].toObject();
      QJsonValue ContractDeroBalance_jv = cbBalances.value("0000000000000000000000000000000000000000000000000000000000000000");

      QJsonObject cbStringKeys = cbResults["stringkeys"].toObject();
      QJsonValue Cosigners_jv = cbStringKeys.value("co_signers");
      QJsonValue SignatureCount_jv = cbStringKeys.value("signature_count");
      QJsonValue SignaturesRequired_jv = cbStringKeys.value("signatures_required");
      QJsonValue RejectCount_jv = cbStringKeys.value("reject_count");
      QJsonValue MaxSupply_jv = cbStringKeys.value("max_supply");
      QJsonValue SupplyIssued_jv = cbStringKeys.value("supply_count");
      QJsonValue TxCount_jv = cbStringKeys.value("tx_count");

      QJsonValue DeroAmt_jv = cbStringKeys.value("dero_amount");
      QJsonValue AssetAmt_jv = cbStringKeys.value("asset_amount");

      QJsonValue Update_jv = cbStringKeys.value("update");
      QJsonValue Remove_jv = cbStringKeys.value("remove");

      QJsonValue NewOwner_jv = cbStringKeys.value("new_owner");
      QJsonValue Receiver_jv = cbStringKeys.value("receiver");
      QJsonValue NewRequired_jv = cbStringKeys.value("new_required");

      /// Will currently read up to 6 owners
      QJsonValue Owner1_jv = cbStringKeys.value("owner1");
      QJsonValue Owner2_jv = cbStringKeys.value("owner2");
      QJsonValue Owner3_jv = cbStringKeys.value("owner3");
      QJsonValue Owner4_jv = cbStringKeys.value("owner4");
      QJsonValue Owner5_jv = cbStringKeys.value("owner5");
      QJsonValue Owner6_jv = cbStringKeys.value("owner6");
      QJsonValue Owner1Signed_jv = cbStringKeys.value("owner1_signed");
      QJsonValue Owner2Signed_jv = cbStringKeys.value("owner2_signed");
      QJsonValue Owner3Signed_jv = cbStringKeys.value("owner3_signed");
      QJsonValue Owner4Signed_jv = cbStringKeys.value("owner4_signed");
      QJsonValue Owner5Signed_jv = cbStringKeys.value("owner5_signed");
      QJsonValue Owner6Signed_jv = cbStringKeys.value("owner6_signed");

      rpc::owner1 = Owner1_jv.toString();
      rpc::owner2 = Owner2_jv.toString();
      rpc::owner3 = Owner3_jv.toString();
      rpc::owner4 = Owner4_jv.toString();
      rpc::owner5 = Owner5_jv.toString();
      rpc::owner6 = Owner6_jv.toString();
      rpc::owner1_signed = Owner1Signed_jv.toInt();
      rpc::owner2_signed = Owner2Signed_jv.toInt();
      rpc::owner3_signed = Owner3Signed_jv.toInt();
      rpc::owner4_signed = Owner4Signed_jv.toInt();
      rpc::owner5_signed = Owner5Signed_jv.toInt();
      rpc::owner6_signed = Owner6Signed_jv.toInt();


      rpc::contractDeroBalance = ContractDeroBalance_jv.toDouble();
      rpc::maxSupply = MaxSupply_jv.toDouble();
      rpc::supplyIssued = SupplyIssued_jv.toDouble();
      rpc::co_signers = Cosigners_jv.toInt();
      rpc::signature_count = SignatureCount_jv.toInt();
      rpc::signatures_required = SignaturesRequired_jv.toInt();
      rpc::reject_count = RejectCount_jv.toInt();
      rpc::tx_count = TxCount_jv.toInt();

      rpc::deroAmt = DeroAmt_jv.toDouble();
      rpc::assetAmt = AssetAmt_jv.toDouble();
      rpc::update = Update_jv.toInt();
      rpc::remove = Remove_jv.toInt();
      rpc::new_owner = NewOwner_jv.toString();
      rpc::new_required = NewRequired_jv.toInt();
      rpc::receiver = Receiver_jv.toString();

      rpc:connected = true;

    }
    return 0;
}
