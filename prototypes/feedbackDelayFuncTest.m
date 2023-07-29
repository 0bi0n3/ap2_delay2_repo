%% Feedback delay function test
%
% This script was adapted and referenced from Zolzer (2011), Pirkle (2019) and Tarr (2019).
% Oberon Day-West (21501990).
%%

function [output, buffer] = feedbackDelayFuncTest(inputSignal, buffer, delay, fbGain)

output = inputSignal + fbGain * buffer(delay,1);

buffer = [output ; buffer(1:end-1,1)];