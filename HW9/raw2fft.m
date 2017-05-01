function [P1, F] = raw2fft(f, x)

L = length(x);
Y = fft(x);
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
F = f*(0:L/2)/L;
plot(F(2:end),P1(2:end));

end