//
//  circularBuffer.cpp
//  delay2
//
//  Created by Oberon Day-West on 28/07/2023.
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
    // Read two samples and interpolate between them
    int samples = static_cast<int>(input);
    double v1 = performRead(samples);
    double v2 = performRead(samples + 1);
    double fraction = input - samples;
    return v1 + fraction * (v2 - v1);
}

void CircularBuffer::performWrite(double input)
{
    // Write the input at the current position
    m_Buffer[currentPos] = input;
    currentPos = (currentPos + 1) % m_Buffer.size();
}
