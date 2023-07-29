//
//  circularBuffer.hpp
//  delay2
//
//  Created by Oberon Day-West on 28/07/2023.
//  This script was adapted and referenced from Reiss and McPherson (2015), Tarr (2019) and Roma (2023).
//  Please refer to accompanying report reference list for full reference details.

#include <vector>
#pragma once

class CircularBuffer {
public:
    
    CircularBuffer(int capacity);
    ~CircularBuffer();

    // Read Operation
    double performRead(int delay);

    // Write Operation
    void performWrite(double value);

    // Interpolation Operation
    double performInterpolation(double delay);
   
private:
    
    // Buffer for storing the samples
    std::vector<double> m_Buffer;

    // Current position
    int currentPos;
};

