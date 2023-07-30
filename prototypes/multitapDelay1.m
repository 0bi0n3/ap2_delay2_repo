%% Multitap delay
%
% This script was adapted and referenced from Zolzer (2011) and Tarr (2019).
% Oberon Day-West (21501990).

%% Multitap Delay Effect Test

clear; clc; close all;

Fs = 48000;
Ts = 1/Fs;
N = Fs * 2; % Number of samples

% Synthesize the impulse signal
imp = zeros(N, 1); % 2-second long signal of 0's
imp(1, 1) = 1; % Change the first sample to 1

% Define the delays and gains for the four delay lines
d1 = round(0.5 * Fs); % 1/2 second delay
d2 = round(0.25 * Fs); % 1/4 second delay
d3 = round(0.125 * Fs); % 1/8 second delay
d4 = round(0.0625 * Fs); % 1/16 second delay

a1 = -0.7; % Gain of feedback delay line 1
a2 = 0.5;  % Gain of feedback delay line 2
a3 = 0.3;  % Gain of feedback delay line 3
a4 = 0.2;  % Gain of feedback delay line 4

% Initialize the output signal
out = zeros(N, 1);

% Apply the delays and feedback
for n = 1:d1
    out(n, 1) = imp(n, 1); % Initially there is no delay
end

for n = d1 + 1:N
    % Delayed signal flow
    out(n, 1) = imp(n, 1) + a1 * out(n - d1) + a2 * out(n - d2) + a3 * out(n - d3) + a4 * out(n - d4);
end

% Plot the impulse response
t_impulseSignal = [0:length(imp) - 1] * Ts;
subplot(1, 2, 1);
stem(t_impulseSignal, imp);
axis([-0.1 2 -0.1 1.1]);
xlabel('Time (sec.)');
title('Input Impulse');

t_out = [0:length(out) - 1] * Ts;
subplot(1, 2, 2);
stem(t_out, out);
axis([-0.1 2 -1.1 1.1]);
xlabel('Time (sec.)');
title('Output Impulse Response');
