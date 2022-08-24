/// dMulti v0.1 ♣♣♣♣♣♣

Function InitializePrivate() Uint64
    10  IF EXISTS("owner1") == 0 THEN GOTO 30
    20 RETURN 1
    30 STORE("co_signers", 3)
    40 initConst()

    50 STORE("signatures_required", 2)
    70 STORE("owner2", ADDRESS_RAW("deto1qyre7td6x9r88y4cavdgpv6k7lvx6j39lfsx420hpvh3ydpcrtxrxqg8v8e3z"))
    80 STORE("owner3", ADDRESS_RAW("deto1qy2nxgts7wdn28ckc4l2tewphjcppqjfj69ddkxjn0ay8hlsjx73jqgmat5s8"))
    // 90 STORE("owner4", ADDRESS_RAW(""))
    // 100 STORE("owner5", ADDRESS_RAW(""))
    // 110 STORE("owner6", ADDRESS_RAW(""))

    ///300 SEND_ASSET_TO_ADDRESS(SIGNER(), 50000000000, SCID())  /// If supplying inital token balance, set supply_count to match
    310 STORE("supply_count", 0)
    320 STORE("max_supply", 100000000000)

    330 IF EXISTS("owner"+ITOA(LOAD("co_signers"))) != 1 THEN GOTO 500
    400 RETURN 0
    500 RETURN 1
End Function

/// Internal functions
Function initConst() Uint64
    10 STORE("owner1", SIGNER())
    20 DIM i as Uint64
    30 LET i = 1
    40 STORE("owner"+ITOA(i)+"_signed", 0)
    50 LET i = i+1
    60 IF i <= LOAD("co_signers") THEN GOTO 40
    70 STORE("signature_count", 0)
    80 STORE("reject_count", 0)
    90 STORE("tx_count", 0)
    100 RETURN 0
End Function


Function checkOwners() Uint64
    10 DIM i as Uint64
    20 LET i = 1
    30 IF LOAD("owner"+ITOA(i)) == SIGNER() THEN GOTO 40 ELSE GOTO 60
    40 MAPSTORE("signedBy", "owner"+ITOA(i))
    50 RETURN 1
    60 LET i = i+1
    70 IF i <= LOAD("co_signers") THEN GOTO 30
    100 RETURN 0
End Function


Function firstSigner(t String, who String, address String, amount Uint64) Uint64
    10 STORE(t, amount)
    20 IF EXISTS("update") THEN GOTO 50
    30 STORE(who, ADDRESS_RAW(address))
    40 RETURN 0
    50 STORE(who, address)
    60 RETURN 0
End Function


Function sign(who String) Uint64
    10 STORE(who+"_signed", 1)
    20 STORE("signature_count", LOAD("signature_count")+1)
    30 RETURN 0
End Function


Function isApproved(s String) Uint64
    10 IF LOAD(s) == LOAD("signatures_required") THEN GOTO 30
    20 RETURN 0
    30 RETURN 1
End Function


Function end() Uint64
    10 DIM i as Uint64
    20 LET i = 1
    30 STORE("owner"+ITOA(i)+"_signed", 0)
    40 LET i = i+1
    50 IF i <= LOAD("co_signers") THEN GOTO 30
    70 STORE("signature_count", 0)
    80 STORE("reject_count", 0)
    90 DELETE("receiver")
    100 STORE("tx_count", LOAD("tx_count")+1)
    110 RETURN 0
End Function


Function issueAsset(receiver String, amount Uint64) Uint64
    10 IF amount+LOAD("supply_count") <= LOAD("max_supply") THEN GOTO 30
    20 RETURN 1
    30 IF IS_ADDRESS_VALID(receiver) THEN GOTO 600 ELSE GOTO 20

    600 SEND_ASSET_TO_ADDRESS(receiver, amount, SCID())
    610 STORE("supply_count", LOAD("supply_count")+amount)
    620 RETURN 0
End Function


/// Multisig functions
Function SendDero(address String, amount Uint64,  ynBool Uint64) Uint64
    10 IF EXISTS("asset_amount") || EXISTS("new_owner") || EXISTS("update") THEN GOTO 50
    20 IF IS_ADDRESS_VALID(ADDRESS_RAW(address)) && ynBool < 2 THEN GOTO 30 ELSE GOTO 50
    30 IF checkOwners() THEN GOTO 100
    50 RETURN 1

    100 IF LOAD(MAPGET("signedBy")+"_signed") == 0 THEN GOTO 110 ELSE GOTO 50
    110 IF EXISTS("dero_amount") THEN GOTO 140
    120 IF ynBool == 0 THEN GOTO 50
    130 firstSigner("dero_amount", "receiver", address, amount)
    140 IF ynBool == 0 THEN GOTO 4000
    150 sign(MAPGET("signedBy"))
    160 IF isApproved("signature_count") THEN GOTO 6000
    170 RETURN 0

    4000 STORE("reject_count", LOAD("reject_count")+1)
    4100 IF LOAD("reject_count") == LOAD("signatures_required") THEN GOTO 6100
    4200 RETURN 0

    6000 SEND_DERO_TO_ADDRESS(LOAD("receiver"), LOAD("dero_amount"))
    6100 DELETE("dero_amount")
    6150 STORE("tx#"+ITOA(LOAD("tx_count")+1), TXID())
    6200 end()
    7000 RETURN 0
End Function


Function SendAsset(address String, amount Uint64, ynBool Uint64) Uint64
    5 IF amount+LOAD("supply_count") > LOAD("max_supply") THEN GOTO 50
    10 IF EXISTS("dero_amount") || EXISTS("new_owner") || EXISTS("update") THEN GOTO 50
    20 IF IS_ADDRESS_VALID(ADDRESS_RAW(address)) && ynBool < 2 THEN GOTO 30 ELSE GOTO 50
    30 IF checkOwners() THEN GOTO 100
    50 RETURN 1

    100 IF LOAD(MAPGET("signedBy")+"_signed") == 0 THEN GOTO 110 ELSE GOTO 50
    110 IF EXISTS("asset_amount") THEN GOTO 140
    120 IF ynBool == 0 THEN GOTO 50
    130 firstSigner("asset_amount", "receiver", address, amount)
    140 IF ynBool == 0 THEN GOTO 4000
    150 sign(MAPGET("signedBy"))
    160 IF isApproved("signature_count") THEN GOTO 6000
    170 RETURN 0

    4000 STORE("reject_count", LOAD("reject_count")+1)
    4100 IF LOAD("reject_count") == LOAD("signatures_required") THEN GOTO 6100
    4200 RETURN 0

    6000 issueAsset(LOAD("receiver"), LOAD("asset_amount"))
    6100 DELETE("asset_amount")
    6150 STORE("tx#"+ITOA(LOAD("tx_count")+1), TXID())
    6600 end()
    7000 RETURN 0
End Function


Function OwnerDeposit() Uint64
    10 DIM i as Uint64
    20 LET i = 1
    30 IF LOAD("owner"+ITOA(i)) == SIGNER() THEN GOTO 70
    40 LET i = i+1
    50 IF i <= LOAD("co_signers") THEN GOTO 30
    60 RETURN 1
    70 IF ASSETVALUE(SCID()) > 0 THEN GOTO 80 ELSE GOTO 90
    80 STORE("supply_count", LOAD("supply_count")-ASSETVALUE(SCID()))
    90 RETURN 0
End Function


Function RemoveOwner(newOwner String, ownerNum Uint64, ynBool Uint64) Uint64
    10 IF EXISTS("dero_amount") || EXISTS("asset_amount") || EXISTS("update") || EXISTS("new_required") THEN GOTO 50
    20 IF IS_ADDRESS_VALID(ADDRESS_RAW(newOwner)) && ynBool < 2 THEN GOTO 30 ELSE GOTO 50
    30 IF checkOwners() THEN GOTO 100
    50 RETURN 1

    100 IF LOAD(MAPGET("signedBy")+"_signed") == 0 THEN GOTO 110 ELSE GOTO 50
    110 IF EXISTS("remove") THEN GOTO 140
    120 IF ynBool == 0 THEN GOTO 50
    130 firstSigner("remove", "new_owner", newOwner, ownerNum)
    140 IF ynBool == 0 THEN GOTO 4000
    150 sign(MAPGET("signedBy"))
    160 IF isApproved("signature_count") THEN GOTO 6000
    170 RETURN 0

    4000 STORE("reject_count", LOAD("reject_count")+1)
    4100 IF LOAD("reject_count") == LOAD("signatures_required") THEN GOTO 6100
    4200 RETURN 0

    6000 STORE("owner"+ITOA(LOAD("remove")), LOAD("new_owner"))
    6100 DELETE("remove")
    6200 DELETE("new_owner")
    6250 STORE("tx#"+ITOA(LOAD("tx_count")+1), TXID())
    6300 end() 
    7000 RETURN 0
End Function


Function AddOwner(newOwner String, consensus Uint64, ynBool Uint64) Uint64
    5 IF consensus <= 1 THEN GOTO 50
    10 IF EXISTS("dero_amount") || EXISTS("asset_amount") || EXISTS("update") || EXISTS("remove") THEN GOTO 50
    20 IF IS_ADDRESS_VALID(ADDRESS_RAW(newOwner)) && ynBool < 2 THEN GOTO 30 ELSE GOTO 50
    30 IF checkOwners() THEN GOTO 100
    50 RETURN 1

    100 IF LOAD(MAPGET("signedBy")+"_signed") == 0 THEN GOTO 110 ELSE GOTO 50
    110 IF EXISTS("new_required") THEN GOTO 140
    120 IF ynBool == 0 THEN GOTO 50
    130 firstSigner("new_required", "new_owner", newOwner, consensus)
    140 IF ynBool == 0 THEN GOTO 4000
    150 sign(MAPGET("signedBy"))
    160 IF isApproved("signature_count") THEN GOTO 6000
    170 RETURN 0

    4000 STORE("reject_count", LOAD("reject_count")+1)
    4100 IF LOAD("reject_count") == LOAD("signatures_required") THEN GOTO 6100
    4200 RETURN 0

    6000 STORE("owner"+ITOA(LOAD("co_signers")+1), LOAD("new_owner"))
    6050 STORE("signatures_required", LOAD("new_required"))
    6075 STORE("co_signers", LOAD("co_signers")+1)
    6100 DELETE("new_required")
    6200 DELETE("new_owner")
    6250 STORE("tx#"+ITOA(LOAD("tx_count")+1), TXID())
    6300 end() 
    7000 RETURN 0
End Function


Function UpdateCode(code String, ynBool Uint64) Uint64 
    10 IF EXISTS("dero_amount") || EXISTS("asset_amount") || EXISTS("new_owner") THEN GOTO 50
    20 IF code != "" && ynBool < 2 THEN GOTO 30 ELSE GOTO 50
    30 IF checkOwners() THEN GOTO 100
    50 RETURN 1

    100 IF LOAD(MAPGET("signedBy")+"_signed") == 0 THEN GOTO 110 ELSE GOTO 50
    110 IF EXISTS("update") THEN GOTO 140
    120 IF ynBool == 0 THEN GOTO 50
    130 firstSigner("update", "new_code", code, 1)
    140 IF ynBool == 0 THEN GOTO 4000
    150 sign(MAPGET("signedBy"))
    160 IF isApproved("signature_count") THEN GOTO 6000
    170 RETURN 0

    4000 STORE("reject_count", LOAD("reject_count")+1)
    4100 IF LOAD("reject_count") == LOAD("signatures_required") THEN GOTO 6100
    4200 RETURN 0

    6000 UPDATE_SC_CODE(LOAD("new_code"))
    6100 DELETE("update")
    6150 STORE("tx#"+ITOA(LOAD("tx_count")+1), TXID())
    6200 end() 
    7000 RETURN 0
End Function