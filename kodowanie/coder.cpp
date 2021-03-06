//
//  coder.cpp
//  kodowanie
//
//  Created by Adam Stanislawski on 27.03.2018.
//  Copyright © 2018 Adam Stanislawski. All rights reserved.
//

#include "coder.hpp"

Coder::Coder() {
    createGenMatrix();
    createMatrixH();
}

bool Coder::isDataValid(std::vector<int> data) {
    
    if(data.size() != k) {
        errorMessage = "Błędna długość ciągu informacyjnego!";
        return false;
    }
    for(int i = 0; i < data.size() ; i++) {
        if(data.at(i) != 0 && data.at(i) != 1) {
            errorMessage = "Błędny znak w ciągu informacyjnym";
            return false;
        }
    }
    return true;
}

void Coder::formatData() {
    
    for(int i = 0; i < n; i++) {
        code.at(i) = code.at(i)%2;
    }
    formatOutputData(code);
}

std::vector<int> Coder::encodeData(std::vector<int> data) {
    
    if(!isDataValid(data)) {
        result = errorMessage;
        return std::vector<int>(1,-1);
    }
    code = std::vector<int>(n,0);
    
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < k; j++) {
            if(data.at(j) && genMatrix[j][i]) code.at(i)++;
        }
    }
    formatData();
    
    return code;
}
