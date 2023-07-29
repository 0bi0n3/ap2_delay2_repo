%% Basic feedback delay
% This script was adapted and referenced from Reiss and McPherson (2015) and Tarr (2019).
% Oberon Day-West (21501990).

%%
% import example audio file
[x, Fs] = audioread( 'snareHit.wav' );

% or create impulse

%x = zeros(100,1);
%x(1) = 1;
%Fs = 48000;

% set delay time (seconds)
delayTime = 500;

timeSec = delayTime/1000;

% cause the delay
delay = fix( timeSec * Fs);

% implement amplitude of delay signal
ampDelay = 0.5;

% Index each sample element of the signal to create the ouput
N = length(x); % length of input signal
y = zeros(N,1); % buffer

% loop through samples
for n = 1:N
    % check to ensure the values aren't negative
    if n < delay+1
        % if negative, pass the audio uneffected
        y( n,1 ) = x( n,1 );

    else
        % make the output signal (y) equal the input + the amplitdue
        % multiplying the delayed signal
        y( n,1 ) = x( n,1 ) + ( ampDelay ) * y( n-delay,1 );

    end

end


% output the audio
sound( y,Fs );

% Plotting x
figure;  % create new figure
t = (0:length(x)-1)/Fs;
plot(t,x);
xlabel('Time (s)');
ylabel('Amplitude');
title('Time Domain Plot for x');

% Plotting Frequency Spectrum of x
figure;  % create new figure
X = fft(x);
f = (0:length(X)-1)*Fs/length(X);
plot(f,abs(X));
xlabel('Frequency (Hz)');
ylabel('Magnitude');
title('Frequency Domain Plot for x');

% Repeat the same for y
% Plotting y
figure;  % create new figure
t = (0:length(y)-1)/Fs;
plot(t,y);
xlabel('Time (s)');
ylabel('Amplitude');
title('Time Domain Plot for y');

% Plotting Frequency Spectrum of y
figure;  % create new figure
Y = fft(y);
f = (0:length(Y)-1)*Fs/length(Y);
plot(f,abs(Y));
xlabel('Frequency (Hz)');
ylabel('Magnitude');
title('Frequency Domain Plot for y');
