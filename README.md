# dMulti-c
Dero Multisig smart contract with Ui (C++)

- [About](https://github.com/SixofClubsss/dMulti-c/edit/main/README.md#about)
- [Contract Usage](https://github.com/SixofClubsss/dMulti-c/edit/main/README.md#contract-usage)
- [Ui](https://github.com/SixofClubsss/dMulti-c/edit/main/README.md#ui)

![170848755-d2cb4933-df2b-46f9-80e6-4349621871a3](https://user-images.githubusercontent.com/84689659/186300133-8a3f22cd-0b43-4b1e-9f78-6416ef7cb8b0.png)
### About
dMulti is a [Dero](https://dero.io/) smart contract for mutli-signature purposes. The internal contract fuctions are written to be easily extracted for use in any [DVM](https://docs.dero.io/rtd_pages/dev_dvm.html) multisig application. To demonstrate their usage the current contract is designed to issue tokens, send Dero, replace a signer, add a new signer and update the contract. These fuctions require the consensus to be met before the action is accepted and executed or it can be rejected and will not execute. Only one voting action can take place at a time. The contract can handle any numbers of signers.

The dMulti-c Ui was built with Qt 5.12.8.  

![blank](https://user-images.githubusercontent.com/84689659/186326051-ce81b9a2-e257-4185-ba48-7f9e6346b8ab.png)

### Donations
- **Dero Address**: dero1qyr8yjnu6cl2c5yqkls0hmxe6rry77kn24nmc5fje6hm9jltyvdd5qq4hn5pn

![DeroDonations](https://user-images.githubusercontent.com/84689659/165414903-44164e7e-4277-44f8-b1fe-8d139f559db1.jpg)

## Contract usage  
- Before installing the contract set parameters in lines 30 and 50 to 320 in `InitializePrivate()`
```
Function InitializePrivate() Uint64
    10  IF EXISTS("owner1") == 0 THEN GOTO 30
    20 RETURN 1
    30 STORE("co_signers", 3)               /// How man initial signers will be on the contract
    40 initConst()

    50 STORE("signatures_required", 2)    /// How many signatures for approval or rejection
    70 STORE("owner2", ADDRESS_RAW("deto1qyre7td6x9r88y4cavdgpv6k7lvx6j39lfsx420hpvh3ydpcrtxrxqg8v8e3z"))
    80 STORE("owner3", ADDRESS_RAW("deto1qy2nxgts7wdn28ckc4l2tewphjcppqjfj69ddkxjn0ay8hlsjx73jqgmat5s8"))
    // 90 STORE("owner4", ADDRESS_RAW(""))
    // 100 STORE("owner5", ADDRESS_RAW(""))     /// Set initial signers address, ensure it match co_signer total 
    // 110 STORE("owner6", ADDRESS_RAW(""))
                                              /// If supplying inital token balance, set supply_count to match
    ///300 SEND_ASSET_TO_ADDRESS(SIGNER(), 50000000000, SCID())  
    310 STORE("supply_count", 0)
    320 STORE("max_supply", 100000000000)   /// Set max token supply

    330 IF EXISTS("owner"+ITOA(LOAD("co_signers"))) != 1 THEN GOTO 500
    400 RETURN 0
    500 RETURN 1
End Function
```
- Install the contract.
```
curl  --request POST --data-binary  @dMulti.bas http://127.0.0.1:30000/install_sc;
```
---
- Issue a asset using `SendAsset()` with parameters:
  - `address` the receiver address of asset transaction. Initial signer chooses receiver to be approved.
  - `amount` atomic unit amount of asset.
  - `ynBool` aproval (1) or rejection (0)
```
curl http://127.0.0.1:30000/json_rpc -d'{"jsonrpc":"2.0","id":"0","method":"scinvoke","params":{"scid":"eb0bfd7205a8753282ebf62a103451cdb30f161db301db742b50dc1b9f2a5c88", "ringsize":2 , "sc_rpc":[{"name":"entrypoint","datatype":"S","value":"SendAsset"}, {"name":"address","datatype":"S","value":"deto1qy2nxgts7wdn28ckc4l2tewphjcppqjfj69ddkxjn0ay8hlsjx73jqgmat5s8"}, {"name":"amount","datatype":"U","value": 50000}, {"name":"ynBool","datatype":"U","value":1}] }}' -H 'Content-Type: application/json';
```
---
- Send Dero from contract balance using `SendDero()` with parameters:
  - `address` the receiver address of Dero transaction. Initial signer chooses receiver to be approved.
  - `amount` atomic unit amount of Dero.
  - `ynBool` aproval (1) or rejection (0)
```
curl http://127.0.0.1:30000/json_rpc -d'{"jsonrpc":"2.0","id":"0","method":"scinvoke","params":{"scid":"eb0bfd7205a8753282ebf62a103451cdb30f161db301db742b50dc1b9f2a5c88", "ringsize":2 , "sc_rpc":[{"name":"entrypoint","datatype":"S","value":"SendDero"}, {"name":"address","datatype":"S","value":"deto1qy2nxgts7wdn28ckc4l2tewphjcppqjfj69ddkxjn0ay8hlsjx73jqgmat5s8"}, {"name":"amount","datatype":"U","value": 50000}, {"name":"ynBool","datatype":"U","value":1}] }}' -H 'Content-Type: application/json';
```
---
- Replace a existing owner using `RemoveOwner()` with parameters:
  - `newOwner` the new owner address who will replace exisiting owner.
  - `ownerNum` the existing owner to be removed. Initial signer chooses owner to be removed.
  - `ynBool` aproval (1) or rejection (0)
```
curl http://127.0.0.1:30000/json_rpc -d'{"jsonrpc":"2.0","id":"0","method":"scinvoke","params":{"scid":"eb0bfd7205a8753282ebf62a103451cdb30f161db301db742b50dc1b9f2a5c88", "ringsize":2 , "sc_rpc":[{"name":"entrypoint","datatype":"S","value":"RemoveOwner"}, {"name":"newOwner","datatype":"S","value":"deto1qy2nxgts7wdn28ckc4l2tewphjcppqjfj69ddkxjn0ay8hlsjx73jqgmat5s8"}, {"name":"ownerNum","datatype":"U","value":2}, {"name":"ynBool","datatype":"U","value":1}] }}' -H 'Content-Type: application/json';
```
---
- Add a new owner using `AddOwner()` with parameters:
  - `newOwner` the new owner address who will replace exisiting owner.
  - `consensus` the new required signature consensus. Initial signer chooses new consensus.
  - `ynBool` aproval (1) or rejection (0)

```
curl http://127.0.0.1:30000/json_rpc -d'{"jsonrpc":"2.0","id":"0","method":"scinvoke","params":{"scid":"eb0bfd7205a8753282ebf62a103451cdb30f161db301db742b50dc1b9f2a5c88", "ringsize":2 , "sc_rpc":[{"name":"entrypoint","datatype":"S","value":"AddOwner"}, {"name":"newOwner","datatype":"S","value":"deto1qy2nxgts7wdn28ckc4l2tewphjcppqjfj69ddkxjn0ay8hlsjx73jqgmat5s8"}, {"name":"consensus","datatype":"U","value":2}, {"name":"ynBool","datatype":"U","value":1}] }}' -H 'Content-Type: application/json';
```
---
- Update the contract using `UpdateCode()` with parameters:
  - `code` the new updated contract code. Initial signer chooses code.
  - `ynBool` aproval (1) or rejection (0)
```
curl http://127.0.0.1:30000/json_rpc -d'{"jsonrpc":"2.0","id":"0","method":"scinvoke","params":{"scid":"eb0bfd7205a8753282ebf62a103451cdb30f161db301db742b50dc1b9f2a5c88", "ringsize":2 , "sc_rpc":[{"name":"entrypoint","datatype":"S","value":"UpdateCode"}, {"name":"code","datatype":"S","value":"NEW_CODE_HERE"}, {"name":"ynBool","datatype":"U","value":1}] }}' -H 'Content-Type: application/json'
```
---

## Ui
  ### Linux
- Download repo & build
```
git clone https://github.com/SixofClubsss/dMulti-c.git
cd dMulti-c
qmake
make
```

- Start app
```
./dMulti
```
- Connect to daemon, wallet and contract with the fields provided at the bottom. Once connected the contract tab will show contract details and notify of any vote in progress.

![contract2](https://user-images.githubusercontent.com/84689659/186321118-b6173b49-57a5-4904-8f5a-8c9cf5b480d5.png)

- In Funds tab you can initiate and vote on a Dero or assets transaction and deposit to the contract. The initial signer chooses the receiver address to be store on chain and which the other signers will vote to approve or reject the transaction. Use the confirm check box to set approval or rejection.

![funds](https://user-images.githubusercontent.com/84689659/186322963-4f2b3b0a-2cb1-4efd-b446-3ba14c7e1493.png)

- In Actions tab you can you can initiate and vote on removal of a signer and adding a new signer. The initial signer chooses the new owner address to be store on chain and which the other signers will vote to approve or reject the new owner. When adding a new owner the inital signer can choose to change the consensus of the contract. Other signers will vote to approve.

![actions](https://user-images.githubusercontent.com/84689659/186323715-2321cf03-1542-4644-b1b6-750c01e31f59.png)

- In update tab you can initiate and vote on contract updates. The initial signer chooses the new code to be voted upon.

![update](https://user-images.githubusercontent.com/84689659/186323924-1255e51a-06b1-4e44-b224-6243e81c1de9.png)

---
### Licensing
    
The source code is published under the [GNU GPL V3 License.](https://github.com/SixofClubsss/Dero-Poker-Table/blob/main/Licenses/LICENSE)     
Copyright 2022 SixofClubs  

