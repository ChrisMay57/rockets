% Stagnation Temperature Calculation
% Ian Gomez
% Input: Ma, Tatm
% Output: Plot of T0 v Ma
close all; clc;

Ma = 0:.01:1.5;
Tatm = 293; %K

k = 1.4;    %ratio of specific heats for air
Tatm_array = Tatm*ones(length(Ma)); %for plotting
T0overT = (1+((k-1)/2).*Ma.^2).^(1/(k-1));
T0 = T0overT*Tatm; %K

figure(1)
hold on
set(gcf,'color','w');
plot(Ma,T0,'b')
plot(Ma,Tatm_array,'r','LineStyle','--')
title('T_0 vs changing Mach numbers')
xlabel('Mach Number'); ylabel('T_0 (K)')
legend('T_0',strcat('T_{atm} =  ',num2str(Tatm),'K'),'Location',...
    'Northwest')
plotfixer