%% Multitap delay
%
% This script was adapted and referenced from Zolzer (2011) and Tarr (2019).
% Oberon Day-West (21501990).

%%

% sampling rate
Fs = 48000;

% sampling period
Ts = 1/Fs;

% total number of samples to be created for 2 seconds
N = Fs * 2;

% impulse test signal of 2 seconds
impulseSignal = [1; zeros(N-1, 1)];

% delay line 1
delayLine1 = round(5 * Fs);

% delay gain 1
dGain1 = 0.5;

% delay line 2
delayLine2 = round(0.75 * Fs);

% delay gain 2
dGain2 = 0.25;

% feedback gains
fbGain1 = -0.7;
fbGain2 = 0.5;

% Initialize output signal
out = zeros(N + max(delayLine1, delayLine2), 1);

% parse signal through
for n = max(delayLine1,delayLine2) + 1:length(impulseSignal)

    % direct signal flow
    out(n,1) = impulseSignal(n,1);

    % delayed signal flow
    if n - delayLine1 > 0
        % delay and gain, then feedback
        out(n,1) = out(n,1) + dGain1 * out(n - delayLine1) + fbGain1 * out(n-1);
    end

    if n - delayLine2 > 0
        % delay and gain, then feedback
        out(n,1) = out(n,1) + dGain2 * out(n - delayLine2) + fbGain2 * out(n-1);
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
