%% Example IIR impulse response system
% This script was adapted and referenced from Tarr, (2019), Hack Audio.
% Oberon Day-West (21501990).

%%

% sampling rate
Fs = 48000;

% sampling period
Ts = 1/Fs;

% total number of samples to be created for 2 seconds
N = Fs * 2;

% impulse test signal of 2 seconds
impulseSignal = zeros(N,1);

% change first sample to value of 1;
impulseSignal(1,1) = 1;

% delay line 1
delayLine1 = 5 * Fs;

% feedback gain 1
fbGain1 = -0.7;

% delay line 2
delayLine2 = 0.75 * -0.5 * Fs;

% feedback gain 2
fbGain2 = 0.5;

% parse signal through
for n = 1:length(impulseSignal)
    
    % direct signal flow
    out( n,1 ) = impulseSignal( n,1 );

    % delayed signal flow
    if n - delayLine1 > 0
        out( n,1 ) = out( n,1 ) + fbGain1 * out( n - delayLine1);
    end

    if n - delayLine2 > 0
        out( n,1 ) = out( n,1 ) + fbGain2 * out( n - delayLine2 );
    end

end


t_impulseSignal = [0:length(impulseSignal)-1]*Ts;
t_out = [0:length(out)-1]*Ts;

subplot(1,2,1);
stem(t_impulseSignal, impulseSignal); % Plot the impulse response
axis([-0.1 2 -0.1 1.1]);
xlabel('Time (sec.)');
title('Input Impulse');

subplot(1,2,2);
stem(t_out, out); % Plot the impulse response
axis([-0.1 10 -1.1 1.1]);
xlabel('Time (sec.)');
title('Output Impulse Response');




