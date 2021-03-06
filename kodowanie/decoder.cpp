//
//  decoder.cpp
//  kodowanie
//
//  Created by Adam Stanislawski on 27.03.2018.
//  Copyright © 2018 Adam Stanislawski. All rights reserved.
//

#include "decoder.hpp"

Decoder::Decoder() {
    createGenMatrix();
    createMatrixH();
    t = calcT();
}

std::vector<int> Decoder::checkValidity() {
    
//    noOfErrors = 0;
//    syndrome = std::vector<int>(n-k,0);
//
//    //liczę syndrom
//    for(int i = 0; i < n-k; i++) {
//        for(int j = 0; j < n; j++) {
//            syndrome.at(i) += code.at(j)*matrixH[j][i];
//        }
//        syndrome.at(i) = syndrome.at(i)%2;
//    }
//
//    //liczę błędy w syndromie
//    for(int i = 0; i < n-k; i++) {
//        if(syndrome.at(i) == 1) {
//            noOfErrors++;
//        }
//    }
    
    //liczę syndrom dla przesunięć cyklicznych
    while(swap < n) {
        
        noOfErrors = 0;
        syndrome = std::vector<int>(n-k,0);
        
        //liczę syndrom
        for(int i = 0; i < n-k; i++) {
            for(int j = 0; j < n; j++) {
                syndrome.at(i) += code.at(j)*matrixH[j][i];
            }
            syndrome.at(i) = syndrome.at(i)%2;
        }
        
        //liczę błędy w syndromie
        for(int i = 0; i < n-k; i++) {
            if(syndrome.at(i) == 1) {
                noOfErrors++;
            }
        }
        
        //jeśli nie ma błędów przerwij
        if(noOfErrors == 0) {
            break;
        }
        
        //jeśli błąd można skorygować, skoryguj i przerwij
        if(noOfErrors <= t) {
            doCorrection();
            while(swap--) {
                cyclicSwap(0);
            }
            break;
        }

        //wykonaj przesunięcie
        cyclicSwap(1);
        swap++;
    }
    //jeśli przesunięto cały kod, to błąd jest niekorygowalny
    if(swap == n) noOfErrors = -1;
    
    return syndrome;
}

void Decoder::doCorrection() {
    //dodanie syndromu do kodu
    for(int i = k; i < n; i++) {
        code.at(i) = ( code.at(i) + syndrome.at(i-k) ) % 2;
    }
}

void Decoder::cyclicSwap(bool dir) {
    
    //przesunięcie w lewo
    if(dir == 1) {
        int temp = code.at(0);
        
        for(int i = 0; i < n-1; i++) {
            code.at(i) = code.at(i+1);
        }
        code.at(n-1) = temp;
    }
    
    //przesunięcie w prawo
    if(dir == 0) {
        int temp = code.at(n-1);
        
        for(int i = n-1; i > 0; i--) {
            code.at(i) = code.at(i-1);
        }
        code.at(0) = temp;
    }
}

bool Decoder::isDataValid(std::vector<int> data) {

    if(data.size() != n) {
        errorMessage = "Błędna długość słowa kodowego!";
        return false;
    }
    
    for(int i = 0; i < data.size() ; i++) {
        if(data.at(i) != 0 && data.at(i) != 1) {
            errorMessage = "Błędny znak w ciągu kodowym!";
            return false;
        }
    }
    
    return true;
}

void Decoder::formatData() {
    
    for(int i = 0; i < k; i++) {
        message.at(i) = code.at(i);
    }
    formatOutputData(message);
}

std::vector<int> Decoder::decodeData(std::vector<int> data) {
    
    reset();
    if(!isDataValid(data)) {
        result = errorMessage;
        noOfErrors = -2;
        return std::vector<int>(1,-1);
    }
    code = data;
    
    checkValidity();
    formatData();
    return message;
}

void Decoder::reset() {
    code = std::vector<int>(n,0);
    message = std::vector<int>(k,0);
    syndrome = std::vector<int>(n-k,0);
    noOfErrors = 0;
    swap = 0;
}

std::string Decoder::convertToString(std::vector<int> data) {
    std::string str = "";
    for(int i = 0; i < data.size(); i++) {
        str.append(std::to_string(data.at(i)));
    }
    return str;
}

std::string Decoder::getCorrectCode() {
    std::string str = convertToString(code);
    return str;
}

int Decoder::getCode() {
    return noOfErrors;
}

