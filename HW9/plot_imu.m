figure;
% plot all at once


plot(raw); hold on; plot(maf); hold on; plot(fir); hold on; plot (iir);
xlabel('sample'); ylabel('data'); title('IMU samples');
legend('raw', 'maf', 'fir', 'iir');

figure
subplot(4,1,1);plot(raw);title('raw');
subplot(4,1,2);plot(maf);title('maf'); 
subplot(4,1,3);plot(fir);title('fir');
subplot(4,1,4);plot(iir);title('iir');
