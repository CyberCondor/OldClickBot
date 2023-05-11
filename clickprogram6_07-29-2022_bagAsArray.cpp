#include <windows.h>
#include <string>
#include <iostream>
#include <cmath>

#define Y_POS_OFFSET 32

#define BS_X_POSITIONS 4
#define BS_Y_POSITIONS 7

#define PRAY_X_POSITIONS 5
#define PRAY_Y_POSITIONS 7

#define MAGIC_X_POSITIONS 7
#define MAGIC_Y_POSITIONS 11

#define NUMBER_OF_BAG_SPACES          (BS_X_POSITIONS * BS_Y_POSITIONS)       // total cells in space - 4x7  = 28
#define NUMBER_OF_PRAYER_SPACES       (PRAY_X_POSITIONS * PRAY_Y_POSITIONS)   // total cells in space - 5x7  = 35
#define NUMBER_OF_NORMAL_SPELL_SPACES (MAGIC_X_POSITIONS * MAGIC_Y_POSITIONS) // total cells in space - 7x11 = 77


struct XY_POS 
{
    int      x;
    int      y;
    COLORREF cr; //Color Ref
    float    distanceFromCenter;
    bool     found;             // is thing found at this position?
    bool     active;            // is thing active at this position?
    bool     available;         // is thing available at this position?
    XY_POS(); 
};
XY_POS::XY_POS()
{
    x         = 0;
    y         = 0;
    cr        = 0;
    found     = false;
    active    = false;
    available = false;
}

struct Object 
{
    XY_POS topLeftPos;
    XY_POS bottomRightPos;
    float height;
    float width;
    float size;
    XY_POS center;
    bool found;

    Object();
};
Object::Object()
{
    found  = false;
    height = 0;
    width  = 0;
    size   = 0;
}

struct BagSpace
{
    Object bagSize;
    XY_POS bs[NUMBER_OF_BAG_SPACES];
    XY_POS magic[NUMBER_OF_NORMAL_SPELL_SPACES];
    XY_POS prayer[NUMBER_OF_PRAYER_SPACES];
    bool isFull = false;
    int numberOfTimesEmptied = 0;
    int currentSpace = 0;
};

struct MapSpace
{
    Object compass;
    Object health;
    Object prayer;
    Object run;
    Object spAttack;
    Object miniMap;
};

// struct NormalSpellBook
// { 
//     XY_POS magic[NUMBER_OF_NORMAL_SPELLS_SPACE];

// };

// struct MagicBook
// {
//     Object MB;
//     NormalSpellBook nSB;
    
// };

// struct WorldObject
// {
//     struct{}


// };


static void init()
{
    std::cout << "======================================================" << std::endl;
    std::cout << "[ HOTKEYS ]" << std::endl << std::endl;
    std::cout << "LShift = TrackPosition N Stuff" << std::endl;
    std::cout << "RShift = Enter Clicker" << std::endl;
    std::cout << "Escape  = Close" << std::endl << std::endl << std::endl;
    std::cout << " " << std::endl << std::endl;
    std::cout << "======================================================" << std::endl;
    std::cout << " " << std::endl << std::endl;
    std::cout << "[ LOG ] " << std::endl << std::endl;
}

static void MouseInput()
{
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(input));
    ZeroMemory(&input, sizeof(input));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(input));
    //ZeroMemory(&input, sizeof(input));     // adds a bug?
    Sleep(10);
}

static void KeebInput(SHORT virtual_key_code)
{
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = virtual_key_code;
    SendInput(1, &input, sizeof(input));
    ZeroMemory(&input, sizeof(input));
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = virtual_key_code;
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    Sleep(100);
    SendInput(1, &input, sizeof(input));
    ZeroMemory(&input, sizeof(input));
}

static void ShiftDrop()
{
    INPUT shiftKey = { 0 };
    shiftKey.type = INPUT_KEYBOARD;
    shiftKey.ki.wVk = VK_LSHIFT;
    SendInput(2, &shiftKey, sizeof(shiftKey));
    ZeroMemory(&shiftKey, sizeof(shiftKey));
    Sleep(100);

    INPUT mouseClick = { 0 };
    mouseClick.type = INPUT_MOUSE;
    mouseClick.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &mouseClick, sizeof(mouseClick));
    ZeroMemory(&mouseClick, sizeof(mouseClick));
    Sleep(100);
    mouseClick.type = INPUT_MOUSE;
    mouseClick.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &mouseClick, sizeof(mouseClick));
    //ZeroMemory(&mouseClick, sizeof(mouseClick));   adds a bug?
    Sleep(100);
    
    shiftKey.type = INPUT_KEYBOARD;
    shiftKey.ki.wVk = VK_LSHIFT;
    shiftKey.ki.dwFlags = KEYEVENTF_KEYUP;
    Sleep(100);
    SendInput(1, &shiftKey, sizeof(shiftKey));
    ZeroMemory(&shiftKey, sizeof(shiftKey));
}

static COLORREF GetColorAtPosition(HDC dc, HDC memDC, int x, int y){
    BitBlt(memDC, 0, 0, 1, 1, dc, x, y, SRCCOPY);
    COLORREF cr = GetPixel(memDC, 0, 0);
    return cr;
}

void DROPEVERYTHINGFROMINVENTORY(HDC dc, HDC memDC, HWND hWND, BagSpace &bagSpace){
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(hWND, &cursorPos);

    KeebInput(VK_ESCAPE); Sleep(256);
    bagSpace.bagSize.center.cr = GetColorAtPosition(dc, memDC, bagSpace.bagSize.center.x, bagSpace.bagSize.center.y);
    if(bagSpace.bagSize.center.cr != 2700606){KeebInput(VK_ESCAPE); Sleep(777);}
    bagSpace.bagSize.center.cr = GetColorAtPosition(dc, memDC, bagSpace.bagSize.center.x, bagSpace.bagSize.center.y);

    //for(int i = 0; i < NUMBER_OF_BAG_SPACES; i++){if(bagSpace.bs[i].found){SetCursorPos(bagSpace.bs[i].x, bagSpace.bs[i].y + Y_POS_OFFSET);Sleep(64);ShiftDrop();Sleep(8);}}
    for(int i = 0; i < NUMBER_OF_BAG_SPACES; i++){SetCursorPos(bagSpace.bs[i].x, bagSpace.bs[i].y + Y_POS_OFFSET);Sleep(300);}

    bagSpace.numberOfTimesEmptied++;
}

static bool CheckIfCursorSetOnTree(HDC dc, HDC memDC){
    bool cursorSetOnTree = false;

    // BitBlt(memDC, 0, 0, 1, 1, dc, 98, 11, SRCCOPY); // works for Oak trees and normal trees - what else does it work for
    // COLORREF cr1 = GetPixel(memDC, 0, 0);
    // if(GetBValue(cr1) > 190 && GetGValue(cr1) > 190 && GetRValue(cr1) < 50){cursorSetOnTree = true;}
    
    // BitBlt(memDC, 0, 0, 1, 1, dc, 98, 10, SRCCOPY); // works for Oak trees and normal trees - what else does it work for
    // COLORREF cr2 = GetPixel(memDC, 0, 0);
    // if(GetBValue(cr2) > 190 && GetGValue(cr2) > 190 && GetRValue(cr2) < 50){cursorSetOnTree = true;}
    
    BitBlt(memDC, 0, 0, 1, 1, dc, 97, 11, SRCCOPY); // works for Oak trees and normal trees - what else does it work for 
    COLORREF cr3 = GetPixel(memDC, 0, 0);
    if(GetBValue(cr3) > 190 && GetGValue(cr3) > 190 && GetRValue(cr3) < 50){cursorSetOnTree = true;}
    
    BitBlt(memDC, 0, 0, 1, 1, dc, 99, 12, SRCCOPY); // works for Oak trees and normal trees - what else does it work for
    COLORREF cr4 = GetPixel(memDC, 0, 0);
    if(GetBValue(cr4) > 190 && GetGValue(cr4) > 190 && GetRValue(cr4) < 50){cursorSetOnTree = true;}
    
    BitBlt(memDC, 0, 0, 1, 1, dc, 100, 10, SRCCOPY); // works for Oak trees and normal trees - what else does it work for
    COLORREF cr5 = GetPixel(memDC, 0, 0);
    if(GetBValue(cr5) > 190 && GetGValue(cr5) > 190 && GetRValue(cr5) < 50){cursorSetOnTree = true;}
    
    BitBlt(memDC, 0, 0, 1, 1, dc, 97, 13, SRCCOPY); // works for Oak trees and normal trees - what else does it work for
    COLORREF cr6 = GetPixel(memDC, 0, 0);
    if(GetBValue(cr6) > 190 && GetGValue(cr6) > 190 && GetRValue(cr6) < 50){cursorSetOnTree = true;}

    return cursorSetOnTree;
}

static POINT GetCursorPosition(HDC dc, HDC memDC, HWND hWND, XY_POS playerPosition){
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(hWND, &cursorPos);
    BitBlt(memDC, 0, 0, 1, 1, dc, cursorPos.x, cursorPos.y, SRCCOPY);
    COLORREF cr = GetPixel(memDC, 0, 0);    
    std::cout << "x position: " << cursorPos.x << " | y position: " << cursorPos.y
              << " --- Color: " << cr
              << " - Red: "   << (int)GetRValue(cr)
              << " - Green: " << (int)GetGValue(cr)
              << " - Blue: "  << (int)GetBValue(cr) << std::endl;               
    Sleep(10);
    return cursorPos;
}

void GetBagSpaceStatus(HDC dc, HDC memDC, BagSpace &bagSpace){
    //BS for Bag Space

    KeebInput(VK_ESCAPE); Sleep(500);
    bagSpace.bagSize.center.cr = GetColorAtPosition(dc, memDC, bagSpace.bagSize.center.x, bagSpace.bagSize.center.y);
    if(bagSpace.bagSize.center.cr != 2700606){KeebInput(VK_ESCAPE); Sleep(500);}
    bagSpace.bagSize.center.cr = GetColorAtPosition(dc, memDC, bagSpace.bagSize.center.x, bagSpace.bagSize.center.y);
    if(bagSpace.bagSize.center.cr != 2700606){KeebInput(VK_ESCAPE); Sleep(777);}

    for(int i = 0; i < NUMBER_OF_BAG_SPACES; i++){bagSpace.bs[i].cr = GetColorAtPosition(dc, memDC, bagSpace.bs[i].x, bagSpace.bs[i].y); if(bagSpace.bs[i].cr != 2700606){bagSpace.bs[i].found = true;}else{bagSpace.bs[i].found = false;} }

    if(bagSpace.bs[27].cr  == 2700606){
        bagSpace.isFull = false;
    }
    else{std::cout << " Bag Full! | Times Emptied:"  << bagSpace.numberOfTimesEmptied << " | Color: " << bagSpace.bagSize.center.cr << std::endl;
         bagSpace.isFull = true;
    }

    //if(!bagSpace.isFull){

        std::cout << " Times Emptied:"  << bagSpace.numberOfTimesEmptied << std::endl;

        int index = 0;
        for(int y = 0; y < BS_Y_POSITIONS; y++){
            std::cout<<   "\t"<<index<<": "<<bagSpace.bs[index].x<<"|"<<bagSpace.bs[index].y<<" - "<<bagSpace.bs[index].cr;
            if(index < NUMBER_OF_BAG_SPACES){index++;}
            std::cout<<"\t|\t"<<index<<": "<<bagSpace.bs[index].x<<"|"<<bagSpace.bs[index].y<<" - "<<bagSpace.bs[index].cr;
            if(index < NUMBER_OF_BAG_SPACES){index++;}
            std::cout<<"\t|\t"<<index<<": "<<bagSpace.bs[index].x<<"|"<<bagSpace.bs[index].y<<" - "<<bagSpace.bs[index].cr;
            if(index < NUMBER_OF_BAG_SPACES){index++;}
            std::cout<<"\t|\t"<<index<<": "<<bagSpace.bs[index].x<<"|"<<bagSpace.bs[index].y<<" - "<<bagSpace.bs[index].cr<<"\n"<<std::endl;
            if(index < NUMBER_OF_BAG_SPACES){index++;}
        }

        // std::cout<<   "\t01: "<<bagSpace.bs[0].x<<"|"<<bagSpace.bs[0].y<<" - "<<bagSpace.bs[0].cr<<"\t|\t02: "<<bagSpace.bs[1].x<<"|"<<bagSpace.bs[1].y<<" - "<<bagSpace.bs[1].cr
        //          <<"\t|\t03: "<<bagSpace.bs[2].x<<"|"<<bagSpace.bs[2].y<<" - "<<bagSpace.bs[2].cr<<"\t|\t04: "<<bagSpace.bs[3].x<<"|"<<bagSpace.bs[3].y<<" - "<<bagSpace.bs[3].cr<<"\n"<<std::endl;
        // std::cout<<   "\t05: "<<bagSpace.bs[4].x<<"|"<<bagSpace.bs[4].y<<" - "<<bagSpace.bs[4].cr<<"\t|\t06: "<<bagSpace.bs[5].x<<"|"<<bagSpace.bs[5].y<<" - "<<bagSpace.bs[5].cr
        //          <<"\t|\t07: "<<bagSpace.bs[6].x<<"|"<<bagSpace.bs[6].y<<" - "<<bagSpace.bs[6].cr<<"\t|\t08: "<<bagSpace.bs[7].x<<"|"<<bagSpace.bs[7].y<<" - "<<bagSpace.bs[7].cr<<"\n"<<std::endl;
        // std::cout<<   "\t09: "<<bagSpace.bs[8].x<<"|"<<bagSpace.bs[8].y<<" - "<<bagSpace.bs[8].cr<<"\t|\t10: "<<bagSpace.bs[9].x<<"|"<<bagSpace.bs[9].y<<" - "<<bagSpace.bs[9].cr
        //          <<"\t|\t11: "<<bagSpace.bs[10].x<<"|"<<bagSpace.bs[10].y<<" - "<<bagSpace.bs[10].cr<<"\t|\t12: "<<bagSpace.bs[11].x<<"|"<<bagSpace.bs[11].y<<" - "<<bagSpace.bs[11].cr<<"\n"<<std::endl;
        // std::cout<<   "\t13: "<<bagSpace.bs[12].x<<"|"<<bagSpace.bs[12].y<<" - "<<bagSpace.bs[12].cr<<"\t|\t14: "<<bagSpace.bs[13].x<<"|"<<bagSpace.bs[13].y<<" - "<<bagSpace.bs[13].cr
        //          <<"\t|\t15: "<<bagSpace.bs[14].x<<"|"<<bagSpace.bs[14].y<<" - "<<bagSpace.bs[14].cr<<"\t|\t16: "<<bagSpace.bs[15].x<<"|"<<bagSpace.bs[15].y<<" - "<<bagSpace.bs[15].cr<<"\n"<<std::endl;
        // std::cout<<   "\t17: "<<bagSpace.bs[16].x<<"|"<<bagSpace.bs[16].y<<" - "<<bagSpace.bs[16].cr<<"\t|\t18: "<<bagSpace.bs[17].x<<"|"<<bagSpace.bs[17].y<<" - "<<bagSpace.bs[17].cr
        //          <<"\t|\t19: "<<bagSpace.bs[18].x<<"|"<<bagSpace.bs[18].y<<" - "<<bagSpace.bs[18].cr<<"\t|\t20: "<<bagSpace.bs[19].x<<"|"<<bagSpace.bs[19].y<<" - "<<bagSpace.bs[19].cr<<"\n"<<std::endl;
        // std::cout<<   "\t21: "<<bagSpace.bs[20].x<<"|"<<bagSpace.bs[20].y<<" - "<<bagSpace.bs[20].cr<<"\t|\t22: "<<bagSpace.bs[21].x<<"|"<<bagSpace.bs[21].y<<" - "<<bagSpace.bs[21].cr
        //          <<"\t|\t23: "<<bagSpace.bs[22].x<<"|"<<bagSpace.bs[22].y<<" - "<<bagSpace.bs[22].cr<<"\t|\t24: "<<bagSpace.bs[23].x<<"|"<<bagSpace.bs[23].y<<" - "<<bagSpace.bs[23].cr<<"\n"<<std::endl;
        // std::cout<<   "\t25: "<<bagSpace.bs[24].x<<"|"<<bagSpace.bs[24].y<<" - "<<bagSpace.bs[24].cr<<"\t|\t26: "<<bagSpace.bs[25].x<<"|"<<bagSpace.bs[25].y<<" - "<<bagSpace.bs[25].cr
        //          <<"\t|\t27: "<<bagSpace.bs[26].x<<"|"<<bagSpace.bs[26].y<<" - "<<bagSpace.bs[26].cr<<"\t|\t28: "<<bagSpace.bs[27].x<<"|"<<bagSpace.bs[27].y<<" - "<<bagSpace.bs[27].cr<<"\n"<<std::endl;
    //}
}

void GetPrayerStatus(HDC dc, HDC memDC, HWND hWND, BagSpace &bagSpace){
    // What is the prayerbook HotKey?
    // Check if I have Prayer available. What percent is my prayer at?
    // Where is the prayer button?
    // Are any prayers active -  if so -> What prayers are active
    // Do I have a prayer potion with me?
    // How long do I have left until prayer runs out?
    // What do I want to do with the information collected here?
    // PRAY_X_POSITIONS 5;
    // PRAY_Y_POSITIONS 7;
    // NUMBER_OF_PRAYER_SPACES (PRAY_X_POSITIONS * PRAY_X_POSITIONS);         // total cells in space - 5x7  = 35
    SetForegroundWindow(hWND);
    SetActiveWindow(hWND);
    SetFocus(hWND);

    Sleep(500);

    KeebInput(VK_F5); Sleep(500);
    // bagSpace.bagSize.center.cr = GetColorAtPosition(dc, memDC, bagSpace.bagSize.center.x, bagSpace.bagSize.center.y);
    // if(bagSpace.bagSize.center.cr != 2700606){KeebInput(VK_F2); Sleep(777);}
    // bagSpace.bagSize.center.cr = GetColorAtPosition(dc, memDC, bagSpace.bagSize.center.x, bagSpace.bagSize.center.y);

    for(int i = 0; i < NUMBER_OF_PRAYER_SPACES; i++){bagSpace.prayer[i].cr = GetColorAtPosition(dc, memDC, bagSpace.prayer[i].x, bagSpace.prayer[i].y); if(bagSpace.prayer[i].cr == 996669){bagSpace.prayer[i].active = true;}else{bagSpace.prayer[i].active = false;} }

    std::cout << " Prayer:"  << bagSpace.numberOfTimesEmptied << std::endl;

    int index = 0;
    for(int y = 0; y < PRAY_Y_POSITIONS; y++){
        std::cout<<   "\t"<<index<<": "<<bagSpace.prayer[index].x<<"|"<<bagSpace.prayer[index].y<<" - "<<bagSpace.prayer[index].cr;
        if(index < NUMBER_OF_PRAYER_SPACES){index++;}
        std::cout<<"\t|\t"<<index<<": "<<bagSpace.prayer[index].x<<"|"<<bagSpace.prayer[index].y<<" - "<<bagSpace.prayer[index].cr;
        if(index < NUMBER_OF_PRAYER_SPACES){index++;}
        std::cout<<"\t|\t"<<index<<": "<<bagSpace.prayer[index].x<<"|"<<bagSpace.prayer[index].y<<" - "<<bagSpace.prayer[index].cr;
        if(index < NUMBER_OF_PRAYER_SPACES){index++;}
        std::cout<<"\t|\t"<<index<<": "<<bagSpace.prayer[index].x<<"|"<<bagSpace.prayer[index].y<<" - "<<bagSpace.prayer[index].cr;
        if(index < NUMBER_OF_PRAYER_SPACES){index++;}
        std::cout<<"\t|\t"<<index<<": "<<bagSpace.prayer[index].x<<"|"<<bagSpace.prayer[index].y<<" - "<<bagSpace.prayer[index].cr<<"\n"<<std::endl;
        if(index < NUMBER_OF_PRAYER_SPACES){index++;}
    }

    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(hWND, &cursorPos);

    for(int i = 0; i < NUMBER_OF_PRAYER_SPACES; i++){SetCursorPos(bagSpace.prayer[i].x, bagSpace.prayer[i].y + Y_POS_OFFSET);Sleep(700);}
}

void GetMagicStatus(HDC dc, HDC memDC, HWND hWND, BagSpace &bagSpace){
    // What is the prayerbook HotKey?                                                                                                           - VK_F6
    // Check if I have runes available. If so, what runes do I have? How many runes do I have?
    // Are any spells available -  if so -> What spells are available
    // Do I have a magic potion with me?
    // How long do I have left until I run out of runes?
    // What do I want to do with the information collected here?
    // MAGIC_X_POSITIONS 7;
    // MAGIC_Y_POSITIONS 11;
    // NUMBER_OF_NORMAL_SPELL_SPACES (MAGIC_X_POSITIONS * MAGIC_X_POSITIONS); // total cells in space - 7x11 = 77
    SetForegroundWindow(hWND);
    SetActiveWindow(hWND);
    SetFocus(hWND);

    Sleep(500);

    KeebInput(VK_F6); Sleep(500);
    //KeebInput(VK_F6); Sleep(500);
    // bagSpace.bagSize.center.cr = GetColorAtPosition(dc, memDC, bagSpace.bagSize.center.x, bagSpace.bagSize.center.y);
    // if(bagSpace.bagSize.center.cr != 2700606){KeebInput(VK_F6); Sleep(777);}
    // bagSpace.bagSize.center.cr = GetColorAtPosition(dc, memDC, bagSpace.bagSize.center.x, bagSpace.bagSize.center.y);

    for(int i = 0; i < NUMBER_OF_NORMAL_SPELL_SPACES; i++){bagSpace.magic[i].cr = GetColorAtPosition(dc, memDC, bagSpace.magic[i].x, bagSpace.magic[i].y); if(bagSpace.magic[i].cr == 996669){bagSpace.magic[i].available = true;}else{bagSpace.magic[i].available = false;} }

    std::cout << " Magic:"  << std::endl;

    int index = 0;
    for(int y = 0; y < MAGIC_Y_POSITIONS; y++){
        std::cout<<   "\t"<<index<<": "<<bagSpace.magic[index].x<<"|"<<bagSpace.magic[index].y<<" - "<<bagSpace.magic[index].cr;
        if(index < NUMBER_OF_NORMAL_SPELL_SPACES){index++;}
        std::cout<<"\t|\t"<<index<<": "<<bagSpace.magic[index].x<<"|"<<bagSpace.magic[index].y<<" - "<<bagSpace.magic[index].cr;
        if(index < NUMBER_OF_NORMAL_SPELL_SPACES){index++;}
        std::cout<<"\t|\t"<<index<<": "<<bagSpace.magic[index].x<<"|"<<bagSpace.magic[index].y<<" - "<<bagSpace.magic[index].cr;
        if(index < NUMBER_OF_NORMAL_SPELL_SPACES){index++;}
        std::cout<<"\t|\t"<<index<<": "<<bagSpace.magic[index].x<<"|"<<bagSpace.magic[index].y<<" - "<<bagSpace.magic[index].cr;
        if(index < NUMBER_OF_NORMAL_SPELL_SPACES){index++;}
        std::cout<<"\t|\t"<<index<<": "<<bagSpace.magic[index].x<<"|"<<bagSpace.magic[index].y<<" - "<<bagSpace.magic[index].cr;
        if(index < NUMBER_OF_NORMAL_SPELL_SPACES){index++;}
        std::cout<<"\t|\t"<<index<<": "<<bagSpace.magic[index].x<<"|"<<bagSpace.magic[index].y<<" - "<<bagSpace.magic[index].cr;
        if(index < NUMBER_OF_NORMAL_SPELL_SPACES){index++;}
        std::cout<<"\t|\t"<<index<<": "<<bagSpace.magic[index].x<<"|"<<bagSpace.magic[index].y<<" - "<<bagSpace.magic[index].cr<<"\n"<<std::endl;
        if(index < NUMBER_OF_NORMAL_SPELL_SPACES){index++;}
    }


    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(hWND, &cursorPos);

    for(int i = 0; i < NUMBER_OF_NORMAL_SPELL_SPACES; i++){SetCursorPos(bagSpace.magic[i].x, bagSpace.magic[i].y + Y_POS_OFFSET);Sleep(700);}
}

BagSpace MapBagSpace(HDC dc, HDC memDC, XY_POS gameWindowSize, XY_POS playerPosition){
    BagSpace bagSpace;
    int confidenceNumber; //use this to check if pixels arond point are also the same color

    bool found = false;

    // BS_X_POSITIONS 4;
    // BS_Y_POSITIONS 7;
    // PRAY_X_POSITIONS 5;
    // PRAY_Y_POSITIONS 7;
    // MAGIC_X_POSITIONS 7;
    // MAGIC_Y_POSITIONS 11;
    // NUMBER_OF_BAG_SPACES (BS_X_POSITIONS * BS_Y_POSITIONS);                // total cells in space - 4x7  = 28
    // NUMBER_OF_PRAYER_SPACES (PRAY_X_POSITIONS * PRAY_X_POSITIONS);         // total cells in space - 5x7  = 35
    // NUMBER_OF_NORMAL_SPELL_SPACES (MAGIC_X_POSITIONS * MAGIC_X_POSITIONS); // total cells in space - 7x11 = 77

    KeebInput(VK_ESCAPE); Sleep(1000);
    bagSpace.bagSize.center.cr = GetColorAtPosition(dc, memDC, bagSpace.bagSize.center.x, bagSpace.bagSize.center.y);
    if(bagSpace.bagSize.center.cr != 2700606){KeebInput(VK_ESCAPE); Sleep(777);}
    bagSpace.bagSize.center.cr = GetColorAtPosition(dc, memDC, bagSpace.bagSize.center.x, bagSpace.bagSize.center.y);
    if(bagSpace.bagSize.center.cr != 2700606){KeebInput(VK_ESCAPE); Sleep(777);}

    int countSpace = 0;

    //189 | Bag height: 260 - modern layout
    found = false;
    while(!found){
        for(int y = gameWindowSize.y ; y > playerPosition.y + 1; y--){
            for(int x = gameWindowSize.x; x > playerPosition.x + 1; x--){
                COLORREF pixelColor = GetColorAtPosition(dc, memDC, x, y);
                if(pixelColor == 2700606 && found == false){
                    //found first instance of bagspace
                    countSpace = 0;
                    for(int x2 = x; countSpace < 101 && x2 > playerPosition.x + 1; x2--){
                        COLORREF pixelColor = GetColorAtPosition(dc, memDC, x2, y);
                        if(pixelColor == 2700606){
                            countSpace++;
                            found = true;
                        }
                        else{countSpace = 105; found = false;}
                    }
                    if(found){
                        std::cout << "x position: " << x << " | y position: " << y
                                  << " --- Color: " << pixelColor << std::endl;
                        bagSpace.bagSize.bottomRightPos.x = x;
                        bagSpace.bagSize.bottomRightPos.y = y;
                        break;
                    }
                }
            }
            if(found){break;}
        }
    }
    bool done = false;
    found = false;
    while(!found){
        for(int y = bagSpace.bagSize.bottomRightPos.y; y > 0 && found == false; y--){
            COLORREF pixelColor = GetColorAtPosition(dc, memDC, bagSpace.bagSize.bottomRightPos.x, y);
            if(pixelColor != 2700606){
                 done = true;
                //countSpace = 0;
                for(int i = 0; i < 8; i++){
                    COLORREF pixelColor = GetColorAtPosition(dc, memDC, bagSpace.bagSize.bottomRightPos.x, y - i);
                    if(pixelColor == 2700606){done = false;}
                }
                if(done){
                    y++;
                    for(int x = bagSpace.bagSize.bottomRightPos.x; x > playerPosition.x + 1 && found == false; x--){
                        COLORREF pixelColor = GetColorAtPosition(dc, memDC, x, y);
                        if(pixelColor != 2700606){
                            x++;
                            COLORREF pixelColor = GetColorAtPosition(dc, memDC, x, y);
                            if(pixelColor == 2700606 && found == false){
                                std::cout << "x position: " << x << " | y position: " << y
                                          << " --- Color: " << pixelColor << std::endl;
                                bagSpace.bagSize.topLeftPos.x = x;                       //might have to adjust this part to account for bag border I think it's like 10 pixels or something
                                bagSpace.bagSize.topLeftPos.y = y;                       //might have to adjust this part to account for bag border I think it's like 10 pixels or something
                                found = true; break;
                            }
                        }
                    }
                    if(found){break;}
                }
            }
        }
    }

    bagSpace.bagSize.size = sqrt(pow(bagSpace.bagSize.bottomRightPos.x - bagSpace.bagSize.topLeftPos.x, 2) + pow(bagSpace.bagSize.bottomRightPos.y - bagSpace.bagSize.topLeftPos.y, 2)); //distance
    if(bagSpace.bagSize.size != 0){bagSpace.bagSize.found = true;}

    bagSpace.bagSize.width    =   bagSpace.bagSize.bottomRightPos.x - bagSpace.bagSize.topLeftPos.x;
    bagSpace.bagSize.height   =   bagSpace.bagSize.bottomRightPos.y - bagSpace.bagSize.topLeftPos.y;
    bagSpace.bagSize.center.x = ((bagSpace.bagSize.bottomRightPos.x + bagSpace.bagSize.topLeftPos.x)/2); //get center position
    bagSpace.bagSize.center.y = ((bagSpace.bagSize.bottomRightPos.y + bagSpace.bagSize.topLeftPos.y)/2); //get center position

    std::cout << "Bag width: "      << bagSpace.bagSize.width    << " | Bag height: " << bagSpace.bagSize.height 
              <<" | Bag Center x: " << bagSpace.bagSize.center.x <<" | Bag Center y: " << bagSpace.bagSize.center.y
              <<" | Bag Size: "     << bagSpace.bagSize.size     << std::endl;

    /////////////////////////////////////////////////////////////////////////////////////////////////////end bagsize

    int bs_xPos[BS_X_POSITIONS];
    int bs_yPos[BS_Y_POSITIONS];
    bs_xPos[0] = ((((bagSpace.bagSize.topLeftPos.x     + bagSpace.bagSize.center.x)/2) + bagSpace.bagSize.topLeftPos.x)/2) + 7;
    bs_xPos[1] = ((((bagSpace.bagSize.topLeftPos.x     + bagSpace.bagSize.center.x)/2) + bagSpace.bagSize.center.x)/2) + 3;
    bs_xPos[2] = ((((bagSpace.bagSize.bottomRightPos.x + bagSpace.bagSize.center.x)/2) + bagSpace.bagSize.center.x)/2) - 3;
    bs_xPos[3] = ((((bagSpace.bagSize.bottomRightPos.x + bagSpace.bagSize.center.x)/2) + bagSpace.bagSize.bottomRightPos.x)/2) - 7;

    bs_yPos[0] = ((((bagSpace.bagSize.topLeftPos.y     + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.topLeftPos.y)/2) - 12;
    bs_yPos[1] = ((  bagSpace.bagSize.topLeftPos.y     + bagSpace.bagSize.center.y)/2) - 8;
    bs_yPos[2] = ((((bagSpace.bagSize.topLeftPos.y     + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.center.y)/2) - 4;
    bs_yPos[3] =     bagSpace.bagSize.center.y;                                                                                                                                          //center y
    bs_yPos[4] = ((((bagSpace.bagSize.bottomRightPos.y + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.center.y)/2) + 2;
    bs_yPos[5] = ((  bagSpace.bagSize.bottomRightPos.y + bagSpace.bagSize.center.y)/2) + 5;
    bs_yPos[6] = ((((bagSpace.bagSize.bottomRightPos.y + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.bottomRightPos.y)/2) + 9;
    int index = 0;
    for(int y = 0; y < BS_Y_POSITIONS; y++){
        for(int x = 0; x < BS_X_POSITIONS; x++){
            bagSpace.bs[index].x = bs_xPos[x]; bagSpace.bs[index].y = bs_yPos[y];
            if(index < NUMBER_OF_BAG_SPACES){index++;}
        }
    }

    int pray_xPos[PRAY_X_POSITIONS];
    int pray_yPos[PRAY_Y_POSITIONS];
    pray_xPos[0] = ((((bagSpace.bagSize.topLeftPos.x     + bagSpace.bagSize.center.x)/2) + bagSpace.bagSize.topLeftPos.x)/2);
    pray_xPos[1] =   ((bagSpace.bagSize.topLeftPos.x     + bagSpace.bagSize.center.x)/2) + 7;
    pray_xPos[2] =     bagSpace.bagSize.center.x;                                                                                                                                        //center x
    pray_xPos[3] =   ((bagSpace.bagSize.bottomRightPos.x + bagSpace.bagSize.center.x)/2) - 7;
    pray_xPos[4] = ((((bagSpace.bagSize.bottomRightPos.x + bagSpace.bagSize.center.x)/2) + bagSpace.bagSize.bottomRightPos.x)/2);

    pray_yPos[0] = ((((bagSpace.bagSize.topLeftPos.y     + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.topLeftPos.y)/2) - 6;
    pray_yPos[1] = ((  bagSpace.bagSize.topLeftPos.y     + bagSpace.bagSize.center.y)/2) - 4;
    pray_yPos[2] = ((((bagSpace.bagSize.topLeftPos.y     + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.center.y)/2) - 2;
    pray_yPos[3] =     bagSpace.bagSize.center.y;                                                                                                                                        //center y
    pray_yPos[4] = ((((bagSpace.bagSize.bottomRightPos.y + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.center.y)/2) + 4;
    pray_yPos[5] = ((  bagSpace.bagSize.bottomRightPos.y + bagSpace.bagSize.center.y)/2) + 8;
    pray_yPos[6] = ((((bagSpace.bagSize.bottomRightPos.y + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.bottomRightPos.y)/2);
    index = 0;
    for(int y = 0; y < PRAY_Y_POSITIONS; y++){
        for(int x = 0; x < PRAY_X_POSITIONS; x++){
            bagSpace.prayer[index].x = pray_xPos[x]; bagSpace.prayer[index].y = pray_yPos[y];
            if(index < NUMBER_OF_PRAYER_SPACES){index++;}  //if index less than total bagspaces then itterate --check to ensure not out of bounds 
        }
    }

    int magic_xPos[MAGIC_X_POSITIONS];
    int magic_yPos[MAGIC_Y_POSITIONS];
    magic_xPos[0] = ((((bagSpace.bagSize.topLeftPos.x     + bagSpace.bagSize.center.x)/2) + bagSpace.bagSize.topLeftPos.x)/2) - 7;
    magic_xPos[1] =   ((bagSpace.bagSize.topLeftPos.x     + bagSpace.bagSize.center.x)/2) - 4;
    magic_xPos[2] = ((((bagSpace.bagSize.topLeftPos.x     + bagSpace.bagSize.center.x)/2) + bagSpace.bagSize.center.x)/2) - 2;
    magic_xPos[3] =     bagSpace.bagSize.center.x;                                                                                                                                       //center x
    magic_xPos[4] = ((((bagSpace.bagSize.bottomRightPos.x + bagSpace.bagSize.center.x)/2) + bagSpace.bagSize.center.x)/2) + 2;
    magic_xPos[5] =   ((bagSpace.bagSize.bottomRightPos.x + bagSpace.bagSize.center.x)/2) + 4;
    magic_xPos[6] = ((((bagSpace.bagSize.bottomRightPos.x + bagSpace.bagSize.center.x)/2) + bagSpace.bagSize.bottomRightPos.x)/2) + 7;

    magic_yPos[0]  = ((((((bagSpace.bagSize.topLeftPos.y + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.topLeftPos.y)/2) + bagSpace.bagSize.topLeftPos.y)/2) - 10;
    magic_yPos[1]  = ((((((bagSpace.bagSize.topLeftPos.y + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.topLeftPos.y)/2) + ((bagSpace.bagSize.topLeftPos.y + bagSpace.bagSize.center.y)/2))/2) - 16;
    magic_yPos[2]  =     ((bagSpace.bagSize.topLeftPos.y + bagSpace.bagSize.center.y)/2) - 8;
    magic_yPos[3]  = ((((((bagSpace.bagSize.topLeftPos.y + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.center.y)/2) + ((bagSpace.bagSize.topLeftPos.y + bagSpace.bagSize.center.y)/2))/2) - 4;
    magic_yPos[4]  = ((((((bagSpace.bagSize.topLeftPos.y + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.center.y)/2) - 4;
    magic_yPos[5]  =       bagSpace.bagSize.center.y;                                                                                                                                    //center y
    magic_yPos[6]  = ((((((bagSpace.bagSize.bottomRightPos.y + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.center.y)/2) + 4;
    magic_yPos[7]  = ((((((bagSpace.bagSize.bottomRightPos.y + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.center.y)/2) + ((bagSpace.bagSize.bottomRightPos.y + bagSpace.bagSize.center.y)/2))/2) + 2;
    magic_yPos[8]  =     ((bagSpace.bagSize.bottomRightPos.y + bagSpace.bagSize.center.y)/2) + 4;
    magic_yPos[9]  = ((((((bagSpace.bagSize.bottomRightPos.y + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.bottomRightPos.y)/2) + ((bagSpace.bagSize.bottomRightPos.y + bagSpace.bagSize.center.y)/2))/2) + 16;
    magic_yPos[10] = ((((((bagSpace.bagSize.bottomRightPos.y + bagSpace.bagSize.center.y)/2) + bagSpace.bagSize.bottomRightPos.y)/2) + bagSpace.bagSize.bottomRightPos.y)/2) + 3;
    index = 0;
    for(int y = 0; y < MAGIC_Y_POSITIONS; y++){
        for(int x = 0; x < MAGIC_X_POSITIONS; x++){
            bagSpace.magic[index].x = magic_xPos[x]; bagSpace.magic[index].y = magic_yPos[y];
            if(index < NUMBER_OF_NORMAL_SPELL_SPACES){index++;}  //if index less than total bagspaces then itterate --check to ensure not out of bounds 
        }
    }

    return bagSpace;
}

//static 

static XY_POS GetWindowSize(HDC dc, HDC memDC, HWND hWND){

    XY_POS gameWindowSize;

    RECT rectangle;
    if(GetWindowRect(hWND, &rectangle))
    {
        int windowWidth = rectangle.right - rectangle.left;
        int windowHeight = rectangle.bottom - rectangle.top;

         gameWindowSize.x = windowWidth - 12;
         gameWindowSize.y = windowHeight - Y_POS_OFFSET;
         std::cout << "width " << gameWindowSize.x << " | height " << gameWindowSize.y << std::endl;
    } 
    return gameWindowSize;  
}

//Can fix this with XY_POS position from center float
static XY_POS GetPlayerPosition(XY_POS gameWindowSize){ 
    XY_POS playerPosition;

    playerPosition.x = (gameWindowSize.x - gameWindowSize.x/2);
    playerPosition.y = (gameWindowSize.y - gameWindowSize.y/2);        
    std::cout << "Player Pos: --- x position: " << playerPosition.x << " | y position: " << playerPosition.y << std::endl;

    return playerPosition;
}

static void GetDistanceFromPlayer(XY_POS playerPosition, POINT otherPosition){
    float dFromP = sqrt(pow(otherPosition.x - playerPosition.x, 2) + pow(otherPosition.y - playerPosition.y, 2));
    //playerPosition.x - playerPosition.y
    std::cout << " --- Distance from player: " << dFromP << std::endl;
    //return dFromP;
}

static XY_POS Set_XYPosOnCursor(HDC dc, HDC memDC, HWND hWND){
    XY_POS setClickPosOnObject;

    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(hWND, &cursorPos);
    BitBlt(memDC, 0, 0, 1, 1, dc, cursorPos.x, cursorPos.y, SRCCOPY);
    COLORREF cr = GetPixel(memDC, 0, 0);
    std::cout << "CAUTION -  Position set ->" 
              << " x position: "   << cursorPos.x 
              << " | y position: " << cursorPos.y << std::endl;
    setClickPosOnObject.x = cursorPos.x;
    setClickPosOnObject.y = cursorPos.y; //need a coordenance struct or something - - - -- haha done!!

    return setClickPosOnObject;
}


void BankEverything_DraynorOak(HDC dc, HDC memDC, BagSpace &bagSpace){

    bool cursorSetOnTree = false;

    SetCursorPos(93, 562 + Y_POS_OFFSET); // go to bank teller   x : 86 | y : 569 direct bank teller --- x : 93 | y position: 562
    Sleep(64);
    cursorSetOnTree = CheckIfCursorSetOnTree(dc, memDC);   
    if(cursorSetOnTree){
        MouseInput();
        Sleep(100);
    }
    //MouseInput();
    Sleep(6000);

    SetCursorPos(716, 981 + Y_POS_OFFSET); // bank everything button
    Sleep(777);
    MouseInput();
    Sleep(1500);

        //have to click on banker

    SetCursorPos(1235, 91 + Y_POS_OFFSET); // go back to tree
    Sleep(64);
    MouseInput();
    Sleep(1024);

    bagSpace.numberOfTimesEmptied++;

    Sleep(3033); //walking for short time back to tree
}

//bury bones
//bank teller pos = x 644 | y  609
//bones pos in bank = x 361 | y 620
//exit bank x 756 | y 221
void BuryBones(HDC dc, HDC memDC, HWND hWND, BagSpace &bagSpace){
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(hWND, &cursorPos);
    int count = 0;
    while (count < 10){
        if (GetAsyncKeyState(VK_NEXT)){std::cout << "<-Clicker Stopped" << std::endl; break;}
        SetCursorPos(644, 609 + Y_POS_OFFSET);
        Sleep(777);
        MouseInput();
        Sleep(777);
        SetCursorPos(361, 620 + Y_POS_OFFSET);
        Sleep(777);
        MouseInput();
        Sleep(777);
        SetCursorPos(756, 221 + Y_POS_OFFSET);
        Sleep(777);
        MouseInput();
        Sleep(333);

        //KeebInput(VK_ESCAPE); Sleep(256);
        bagSpace.bagSize.center.cr = GetColorAtPosition(dc, memDC, bagSpace.bagSize.center.x, bagSpace.bagSize.center.y);
        if(bagSpace.bagSize.center.cr != 2700606){KeebInput(VK_ESCAPE); Sleep(777);}
        bagSpace.bagSize.center.cr = GetColorAtPosition(dc, memDC, bagSpace.bagSize.center.x, bagSpace.bagSize.center.y);
        for(int i = 0; i < NUMBER_OF_BAG_SPACES; i++){SetCursorPos(bagSpace.bs[i].x, bagSpace.bs[i].y + Y_POS_OFFSET);Sleep(510);MouseInput();Sleep(550);}
        count++;
    }
}


void TeleAlc_Camelot(HDC dc, HDC memDC, HWND hWND, BagSpace &bagSpace){

    //alc then teleport
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(hWND, &cursorPos);
    int count = 0;
    int bsIndex = 0;
    while (bsIndex < 8 && count < 2291){
        if (GetAsyncKeyState(VK_NEXT)){std::cout << "<-Clicker Stopped" << std::endl; break;}
        // bagSpace.bagSize.center.cr = GetColorAtPosition(dc, memDC, bagSpace.bagSize.center.x, bagSpace.bagSize.center.y);
        // if(bagSpace.bagSize.center.cr == 2700606){
        //     bagSpace.bagSize.center.cr = GetColorAtPosition(dc, memDC, bagSpace.bagSize.center.x, bagSpace.bagSize.center.y);
        //     std::cout << "--BAG SPACE COLOR FOUND--" << std::endl; KeebInput(VK_F6); Sleep(300);
        // }

        bool foundSomethingToAlc = false;

        GetBagSpaceStatus(dc, memDC, bagSpace);

        if(bagSpace.bs[bsIndex].cr == 2700606){bsIndex++;}
        else{
            KeebInput(VK_F6);
            SetCursorPos(bagSpace.magic[34].x, bagSpace.magic[34].y + Y_POS_OFFSET); // Alc in Magic Book
            Sleep(550);
            MouseInput();
            SetCursorPos(bagSpace.bs[bsIndex].x, bagSpace.bs[bsIndex].y + Y_POS_OFFSET); // Item to Alc in Bag
            Sleep(1500);
            MouseInput();
            SetCursorPos(bagSpace.magic[26].x, bagSpace.magic[26].y + Y_POS_OFFSET); // Camelot Teleport
            Sleep(1000);
            MouseInput();

            count++;
            std::cout << "Count: " <<  count << "\tColor: " << bagSpace.bs[bsIndex].cr << std::endl;
        }
    }
}

void SpamTele_Faldor(HDC dc, HDC memDC, HWND hWND, BagSpace &bagSpace){

    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(hWND, &cursorPos);
    int count = 0;
    while (count < 700){
        if (GetAsyncKeyState(VK_NEXT)){std::cout << "<-Clicker Stopped" << std::endl; break;}

        SetCursorPos(bagSpace.magic[21].x, bagSpace.magic[21].y + Y_POS_OFFSET); // Camelot Teleport
        Sleep(3123);
        MouseInput();

        count++;
    }
}

bool Chop_A_Tree(HDC dc, HDC memDC, HWND hWND, BagSpace &bagSpace, bool &click, XY_POS clickPosOnTree){

    bool cursorSetOnTree = false;

    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(hWND, &cursorPos);

    GetBagSpaceStatus(dc, memDC, bagSpace);
    if(!bagSpace.isFull){
        //SetCursorPos(1118, 16 + Y_POS_OFFSET); // Orient the Map North
        SetCursorPos(798, 16 + Y_POS_OFFSET); // Orient the Map North
        Sleep(777);

        MouseInput();
        
        SetCursorPos(clickPosOnTree.x, clickPosOnTree.y + Y_POS_OFFSET);
        Sleep(64);
        
        cursorSetOnTree = CheckIfCursorSetOnTree(dc, memDC);
        
        if(cursorSetOnTree){
            MouseInput();
        }
    }
    else{// Bag Full!!!!!!!!!!!!!!!!!!!!!!!!    
        DROPEVERYTHINGFROMINVENTORY(dc, memDC, hWND, bagSpace);
        //BankEverything_DraynorOak(dc, memDC, bagSpace);
    }

    if(bagSpace.numberOfTimesEmptied > 120){std::cout << "<-Clicker Stopped - BAGS*" << std::endl; click = false; return click;}

    if(!bagSpace.isFull){
        
        if (GetAsyncKeyState(VK_NEXT)){std::cout << "<-Clicker Stopped" << std::endl; click = false; return click;}
        Sleep(4333);
        SetCursorPos(clickPosOnTree.x, clickPosOnTree.y + Y_POS_OFFSET);
        cursorSetOnTree = CheckIfCursorSetOnTree(dc, memDC);
        if(!cursorSetOnTree){
            GetBagSpaceStatus(dc, memDC, bagSpace);
            DROPEVERYTHINGFROMINVENTORY(dc, memDC, hWND, bagSpace);
            SetCursorPos(clickPosOnTree.x, clickPosOnTree.y + Y_POS_OFFSET);
            Sleep(64);
            cursorSetOnTree = CheckIfCursorSetOnTree(dc, memDC);
            Sleep(2773);
        }
        if (GetAsyncKeyState(VK_NEXT)){std::cout << "<-Clicker Stopped" << std::endl; click = false; return click;}
        cursorSetOnTree = CheckIfCursorSetOnTree(dc, memDC);
        if(!cursorSetOnTree){
            SetCursorPos(clickPosOnTree.x, clickPosOnTree.y + Y_POS_OFFSET);
            Sleep(3333);
        }
        if (GetAsyncKeyState(VK_NEXT)){std::cout << "<-Clicker Stopped" << std::endl; click = false; return click;}
        cursorSetOnTree = CheckIfCursorSetOnTree(dc, memDC);
        if(!cursorSetOnTree){
            Sleep(3224);
        }
        cursorSetOnTree = CheckIfCursorSetOnTree(dc, memDC);
        if(!cursorSetOnTree){
            Sleep(8224);
        }
        cursorSetOnTree = CheckIfCursorSetOnTree(dc, memDC);
        if(!cursorSetOnTree){
            std::cout << "<-Clicker Stopped" << std::endl; click = false; return click;
        }   
    }
    else{Sleep(2000);}
    return click;
}

void Run_NightmareZone_Magic(HDC dc, HDC memDC, HWND hWND, BagSpace &bagSpace)
{
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(hWND, &cursorPos);

    GetBagSpaceStatus(dc, memDC, bagSpace);

}


int main()
{
    init();
    while (true)
    {
        LPCTSTR window_title = "Old School CyberCondorScapee";
        HWND hWND = FindWindow(NULL, window_title);

        while (hWND == NULL){
            hWND = FindWindow(NULL, window_title);
            std::cout << "start game!" << std::endl;
            Sleep(5000);
        }
        if(hWND){
            HDC dc = GetDC(hWND);
            HDC memDC = CreateCompatibleDC(dc);
            HBITMAP memBM = CreateCompatibleBitmap(dc, 1, 1);
            SelectObject(memDC, memBM);
            
            if(GetAsyncKeyState(VK_LBUTTON)){
                XY_POS gameWindowSize = GetWindowSize(dc, memDC, hWND);
                XY_POS playerPosition = GetPlayerPosition(gameWindowSize);
                POINT cursorPos = GetCursorPosition(dc, memDC, hWND, playerPosition);
                GetDistanceFromPlayer(playerPosition, cursorPos);
                Sleep(1000);
            }
            // if (GetAsyncKeyState(VK_LSHIFT)){
            //     XY_POS gameWindowSize = GetWindowSize(dc, memDC, hWND);
            //     XY_POS playerPosition = GetPlayerPosition(gameWindowSize);
            //     POINT cursorPos = GetCursorPosition(dc, memDC, hWND, playerPosition);
            //     GetDistanceFromPlayer(playerPosition, cursorPos);
            //     Sleep(10);
            // }
            if (GetAsyncKeyState(VK_ESCAPE)){
                std::cout << " - Proof of Concept Bot Ended - " << std::endl;
                DeleteDC(memDC);
                DeleteObject(memBM);
                ReleaseDC(hWND,dc);
                Sleep(500);
                break;
            }
            if (GetAsyncKeyState(VK_RMENU)){  // Enter Clicker    //VK_RMENU          
                bool click = false;
                SetForegroundWindow(hWND);
                SetActiveWindow(hWND);
                SetFocus(hWND);
                
                XY_POS clickPosOnTree = Set_XYPosOnCursor(dc, memDC, hWND);
                KeebInput(VK_ESCAPE); Sleep(300);
                XY_POS gameWindowSize = GetWindowSize(dc, memDC, hWND);
                XY_POS playerPosition = GetPlayerPosition(gameWindowSize);
                
                BagSpace   bagSpace = MapBagSpace(dc, memDC, gameWindowSize, playerPosition);
                GetBagSpaceStatus(dc, memDC, bagSpace);

                std::cout << "--- PgUp to start | PgDn to break ---" << std::endl;

                while (true){
                    if (GetAsyncKeyState(VK_PRIOR))    {std::cout << "Clicker Started->" << std::endl; click = true;}
                    else if (GetAsyncKeyState(VK_NEXT)){std::cout << "<-Clicker Stopped" << std::endl; click = false; break;}
                    if (click) {


                    // int choice = 0;

                    // std::cout << "\nESC: Get BagSpace Status" << std::endl;
                    // std::cout << "1: Get MagicSpace Status" << std::endl;
                    // std::cout << "2: Get Prayerspace Status" << std::endl;
                    // std::cout << "3: Set CursorPos" << std::endl;
                    // std::cout << "4: Get All Info" << std::endl;
                    //std::cin >> choice;

                    // if (GetAsyncKeyState(VK_ESCAPE)){ 
                    //     GetBagSpaceStatus(dc, memDC, bagSpace);
                    // }
                    // if (GetAsyncKeyState(VK_F6)){ 
                    //     GetMagicStatus(dc, memDC, hWND, bagSpace);
                    // }
                    // if (GetAsyncKeyState(VK_F5)){ 
                    //     GetPrayerStatus(dc, memDC, hWND, bagSpace);
                    // }
                    // if (GetAsyncKeyState(VK_RMENU)){ 
                    //     clickPosOnTree = Set_XYPosOnCursor(dc, memDC, hWND);
                    // }
                    // Sleep(1000);


                    //    case 0:
                    //         GetBagSpaceStatus(dc, memDC, bagSpace);
                    //         break;
                    //    case 1:
                    //         GetMagicStatus(dc, memDC, hWND, bagSpace);
                    //         break;
                    //     case 2:
                    //         GetPrayerStatus(dc, memDC, hWND, bagSpace);
                    //         break;
                    //     case 3:
                    //         clickPosOnTree = Set_XYPosOnCursor(dc, memDC, hWND);
                    //         break;
                    //     case 4:
                    //         std::cout << "Not built yet" << std::endl;
                    //         break;
                    //     default :
                    //         std::cout << "Choice not Available" << std::endl;
                    // }

                        // GetBagSpaceStatus(dc, memDC, bagSpace);
                        // //DROPEVERYTHINGFROMINVENTORY(dc, memDC, hWND, bagSpace);
                        // GetMagicStatus(dc, memDC, hWND, bagSpace);

                        // GetPrayerStatus(dc, memDC, hWND, bagSpace);

                        // GetBagSpaceStatus(dc, memDC, bagSpace);

/////////////////////////////////////////////////////////////////////////
                        TeleAlc_Camelot(dc, memDC, hWND, bagSpace);
                        KeebInput(VK_NEXT);
                        KeebInput(VK_ESCAPE);
                        click = false;
                        break;
/////////////////////////////////////////////////////////////////////////                        
                        // SpamTele_Faldor(dc, memDC, hWND, bagSpace);
                        // KeebInput(VK_NEXT);
                        // KeebInput(VK_ESCAPE);
                        // click = false;
                        // break;


                        // click = Chop_A_Tree(dc, memDC, hWND, bagSpace, click, clickPosOnTree);
                        // Sleep(1000);

                        // GetMagicStatus(dc, memDC, hWND, bagSpace);

                        // Sleep(1000);

                        // GetPrayerStatus(dc, memDC, hWND, bagSpace);

                        // Sleep(1000);


                        // std::cout << "<-Clicker Stopped" << std::endl; click = false; break;


                        // bool cursorSetOnTree = false;
                        
                        // GetBagSpaceStatus(dc, memDC, bagSpace);
                        // if(!bagSpace.isFull){
                        //     //SetCursorPos(1118, 16 + Y_POS_OFFSET); // Orient the Map North
                        //     SetCursorPos(798, 16 + Y_POS_OFFSET); // Orient the Map North
                        //     Sleep(777);

                        //     MouseInput();
                            
                        //     SetCursorPos(clickPosOnTree.x, clickPosOnTree.y + Y_POS_OFFSET);
                        //     Sleep(64);
                            
                        //     cursorSetOnTree = CheckIfCursorSetOnTree(dc, memDC);
                            
                        //     if(cursorSetOnTree){
                        //         MouseInput();
                        //     }
                        // }
                        // else{// Bag Full!!!!!!!!!!!!!!!!!!!!!!!!    
                        //     DROPEVERYTHINGFROMINVENTORY(dc, memDC, hWND, bagSpace);
                        //     //BankEverything_DraynorOak(dc, memDC, bagSpace);
                        // }

                        // if(bagSpace.numberOfTimesEmptied > 120){std::cout << "<-Clicker Stopped - BAGS*" << std::endl; click = false; break;}
                        
                        // if(!bagSpace.isFull){
                            
                        //     if (GetAsyncKeyState(VK_NEXT)){std::cout << "<-Clicker Stopped" << std::endl; click = false; break;}
                        //     Sleep(4333);
                        //     SetCursorPos(clickPosOnTree.x, clickPosOnTree.y + Y_POS_OFFSET);
                        //     cursorSetOnTree = CheckIfCursorSetOnTree(dc, memDC);
                        //     if(!cursorSetOnTree){
                        //         GetBagSpaceStatus(dc, memDC, bagSpace);
                        //         DROPEVERYTHINGFROMINVENTORY(dc, memDC, hWND, bagSpace);
                        //         SetCursorPos(clickPosOnTree.x, clickPosOnTree.y + Y_POS_OFFSET);
                        //         Sleep(64);
                        //         cursorSetOnTree = CheckIfCursorSetOnTree(dc, memDC);
                        //         Sleep(2773);
                        //     }
                        //     if (GetAsyncKeyState(VK_NEXT)){std::cout << "<-Clicker Stopped" << std::endl; click = false; break;}
                        //     cursorSetOnTree = CheckIfCursorSetOnTree(dc, memDC);
                        //     if(!cursorSetOnTree){
                        //         SetCursorPos(clickPosOnTree.x, clickPosOnTree.y + Y_POS_OFFSET);
                        //         Sleep(3333);
                        //     }
                        //     if (GetAsyncKeyState(VK_NEXT)){std::cout << "<-Clicker Stopped" << std::endl; click = false; break;}
                        //     cursorSetOnTree = CheckIfCursorSetOnTree(dc, memDC);
                        //     if(!cursorSetOnTree){
                        //         Sleep(3224);
                        //     }
                        //     cursorSetOnTree = CheckIfCursorSetOnTree(dc, memDC);
                        //     if(!cursorSetOnTree){
                        //         Sleep(8224);
                        //     }
                        //     cursorSetOnTree = CheckIfCursorSetOnTree(dc, memDC);
                        //     if(!cursorSetOnTree){
                        //         std::cout << "<-Clicker Stopped" << std::endl; click = false; break;
                        //     }   
                        // }
                        // else{Sleep(2000);}
                    }
                }
                Sleep(500);  
            }
            DeleteDC(memDC);
            DeleteObject(memBM);
            ReleaseDC(hWND,dc);
        }
    }
    return 0;
}