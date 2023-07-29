%% Circular Buffer
%
% This script was adapted and referenced from Zolzer (2011), Pirkle (2019) and Tarr (2019).
% Oberon Day-West (21501990).
%%

% number of samples to write
numSamplesWrite = 50;

% Initialize an impulse signal
x = zeros(numSamplesWrite, 1);
x(1:10) = 1; % Set the first sample to 1

% initialise buffer
buffer = zeros(numSamplesWrite, 1);

% initialise write pointer
writePointer = 1;

% loop over audio and intake samples
for n = 1:numSamplesWrite
    % write one sample into buffer
    buffer(writePointer) = x(n);
    
    % increment buffer index
    writePointer = writePointer + 1;

    % error check for going outside of buffer allocation
    if( writePointer == bufferLength + 1 )
        writePointer = 1; % wrap buffer writer position back to the beginning
    end
end


% delay time in samples, to demonstrate correct buffer writing/reading
numSampleDelay = 15;

% read pointer
readPointer = writePointer - numSampleDelay;

% check read pointer is not negative
if( readPointer < 1 )
    readPointer = readPointer + bufferLength;
end

% Generate the output buffer with delay
outputBuffer = circshift(buffer, -numSampleDelay);

% Plot the impulse signal, the circular buffer, and the output from the buffer
figure;
subplot(3, 1, 1);
stem(x);
title('Impulse Signal');

subplot(3, 1, 2);
stem(buffer);
title('Circular Buffer');

subplot(3, 1, 3);
stem(outputBuffer);
title('Output with Delay');