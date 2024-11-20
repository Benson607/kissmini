# 程式介紹

此程式可將kiss檔案最簡化，並輸出簡化後的kiss、dot檔案。

目前可處理之kiss指令:

    .start_kiss
    .i
    .o
    .p
    .s
    .r
    .end_kiss

# 所需函式庫

    iostream
    fstream
    sstream
    string
    vector

# 使用方式

進入B11215033.cpp所在之資料夾，並以g++進行編譯

    g++ B11215033.cpp -o main

若無法正常編譯，請確認您是否安裝g++，且能正常連結到所需之函式庫，或是手動連結所需函式庫

或是使用資料夾內提供的setup.sh

    sudo bash setup.sh

這將自動檢查您的系統是否有g++，若沒有安裝g++，則會幫您安裝，裝好後會自動幫您編譯B11215033.cpp

若無法正常安裝g++，可能代表您的ubuntu版本過低，請使用更高版本的ubuntu

使用下面指令將kiss最簡化

    ./main [kiss來源] [目標kiss檔案] [目標dot檔案]

若目標kiss檔案不存在，則會創造該檔案

示範

    g++ B11215033.cpp -o main
    ./main src.kiss trg.kiss result.dot