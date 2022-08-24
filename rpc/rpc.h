#ifndef RPC_H
#define RPC_H
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

#include "string"
#include "curl/curl.h"
#include "QTime"
#include "QJsonDocument"
#include "QJsonObject"
#include "QJsonValue"
#include "QJsonArray"
#include "QString"
#include "QObject"
#include "QDebug"
#include "QThread"

using std::string;


static size_t
WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


class rpc
{

public:
    static bool connected;
    static double blockHeight;
    static double deroBalance;
    static double assetBalance;

    static QString owner1;
    static QString owner2;
    static QString owner3;
    static QString owner4;
    static QString owner5;
    static QString owner6;
    static int owner1_signed;
    static int owner2_signed;
    static int owner3_signed;
    static int owner4_signed;
    static int owner5_signed;
    static int owner6_signed;

    static double contractDeroBalance;
    static double maxSupply;
    static double supplyIssued;
    static int co_signers;
    static int signature_count;
    static int signatures_required;
    static int reject_count;
    static int tx_count;

    static double deroAmt;
    static double assetAmt;
    static int update;
    static int remove;
    static int new_required;
    static QString new_owner;
    static QString receiver;

    int getDeroBalance();
    int getAssetBalance();
    int getHeight();
    int fetchScData();

};


class Worker : public QObject
{
    Q_OBJECT

public:
    static QThread workThread;

private slots:
    void onTimeout()
    {
          rpc rpcData;
          rpcData.getHeight();
          rpcData.fetchScData();
          rpcData.getDeroBalance();
          rpcData.getAssetBalance();
    }

};

#endif // RPC_H
