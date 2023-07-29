//
//  circularBuffer.cpp
//  delay2
//
//  Created by Oberon Day-West on 28/07/2023.
//  This script was adapted and referenced from Reiss and McPherson (2015), Tarr (2019) and Roma (2023).
//  Please refer to accompanying report reference list for full reference details.
//

#include "circularBuffer.hpp"
#include <algorithm>

CircularBuffer::CircularBuffer(int size)
{
    // Create buffer with the specified size, initially filled with 0s
    // Set current position to 0
    m_Buffer = std::vector<double>(size, 0);
    currentPos = 0;
}

CircularBuffer::~CircularBuffer()
{
    // Destructor - no need to do anything because std::vector handles its own memory
}

double CircularBuffer::performRead(int input)
{
    input = std::min(input, static_cast<int>(m_Buffer.size() - 1));
    int index = currentPos - input;
    if (index < 0) index += m_Buffer.size();
    return m_Buffer[index];
}

double CircularBuffer::performInterpolation(double input)
{
    // Read four samples and perform cubic interpolation between them
    int sampleIndex = static_cast<int>(input);
    double v0 = performRead(sampleIndex - 1);
    double v1 = performRead(sampleIndex);
    double v2 = performRead(sampleIndex + 1);
    double v3 = performRead(sampleIndex + 2);

    // Fractional part of the input
    double fraction = input - sampleIndex;

    // Cubic interpolation formula
    double a = v3 - v2 - v0 + v1;
    double b = v0 - v1 - a;
    double c = v2 - v0;
    double d = v1;

    return ((a * fraction + b) * fraction + c) * fraction + d;
}


void CircularBuffer::performWrite(double input)
{
    // Write the input at the current position
    m_Buffer[currentPos] = input;
    currentPos = (currentPos + 1) % m_Buffer.size();
}
