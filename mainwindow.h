#ifndef MAINWINDOW_H
#define MAINWINDOW_H
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

#include "rpc/rpc.h"
#include "QMainWindow"
#include "QDebug"
#include "QThread"
#include "QTimer"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loginInfo();
    void currentEvent();
    QString signedText(int);
    void printDetails();
    int checkDaemon();
    int checkContract();
    int checkWallet();
    int issueAsset();
    int sendDero();
    int deposit();
    int removeOwner();
    int addOwner();
    int updateContract();
    static std::string rpcLogin;
    static QString daemonAddress;
    static QString walletAddress;
    static QString contractAddress;


private slots:
    void refresh();

    void on_connectButton_clicked();

    void toggle();

    void on_sendAssetButton_clicked();

    void on_sendDeroButton_clicked();

    void on_depositoButton_clicked();

    void on_removeButton_clicked();

    void on_addButton_clicked();

    void on_deroCheckBox_stateChanged(int arg1);

    void on_assetCheckBox_stateChanged(int arg1);

    void on_depositCheckBox_stateChanged(int arg1);

    void on_removeCheckBox_stateChanged(int arg1);

    void on_addCheckBox_stateChanged(int arg1);

    void on_updateCheckBox_stateChanged(int arg1);

    void on_updateButton_clicked();

private:
    Ui::MainWindow *ui;

    void disableGroupBoxes();

    void setGroupBoxes();
};

#endif // MAINWINDOW_H
