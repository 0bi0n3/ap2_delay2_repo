%% Example conversion of seconds to samples
% This script was adapted and referenced from Tarr, (2019), Hack Audio.
% Oberon Day-West (21501990).

%% Ms to Secs to Samples

% Fs = sampling rate
Fs = 48000; 

timeInMS = 350;

% conversion from MS to Secs
timeInSec = timeInMS/1000; 

% fucntion that rounds to the nearest integer sample (continuous to discrete)
timeSamples = fix( timeInSec * Fs ); 

% print conversion to console
disp(timeSamples)


