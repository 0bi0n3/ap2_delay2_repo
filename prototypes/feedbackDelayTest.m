%% Feedback Delay
%
% This script was adapted and referenced from Zolzer (2011), Pirkle (2019) and Tarr (2019).
% Oberon Day-West (21501990).
%%

% test signal
inputSignal = [1 ; -1 ; 2 ; -2 ; zeros(6,1)];

% buffer (longer than delay)
buffer = zeros(20,1);

% delay amount (in samples)
delay = 5;

% feedback gain
fbGain = 0.5;

% output vector
N = length(inputSignal);
output = zeros(N,1);

% series delay
for n = 1:N
    % passing buffer into feedback delay
    [output(n,1), buffer] = feedbackDelayFuncTest(inputSignal(n,1), buffer, delay, fbGain);
    % return buffer for next loop
end

% plot input and output results
disp('Feedback delay: 5 samples');
disp('Original input signal: ')
disp(inputSignal);
disp('Output signal result: ')
disp(output);

% stem plots
figure;
subplot(2,1,1);
stem(1:N, inputSignal, 'filled');
title('Original input signal');
xlabel('Sample #');
ylabel('Amplitude');

subplot(2,1,2);
stem(1:N, output, 'filled');
title('Output signal results');
xlabel('Sample #');
ylabel('Amplitude');
