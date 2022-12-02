x = [1, 2, 5, 10, 15, 20, 25];
y1 = [0.7982,0.5983,0.0226,0.0,0.0,0.0,0.0];
y2 = [0.6866,0.5363,0.0383,0.0,0.0,0.0,0.0];
y3 = [0.685,0.5361,0.0049,0.0,0.0,0.0,0.0];

xx  = linspace(1, 25, 100);
yy1 = interp1(x, y1, xx, 'makima');
yy2 = interp1(x, y2, xx, 'makima');
yy3 = interp1(x, y3, xx, 'makima');

figure;
plot(xx,yy1,xx,yy2,xx,yy3);
ylim([-0.1 0.7])
xlabel('Lambda');
ylabel('Blocking Probability');
title('Lambda vs. Blocking Probability (w/ Constant Mu)');
legend('Theoretical Blocking', 'Random Blocking', 'Min. Blocking');