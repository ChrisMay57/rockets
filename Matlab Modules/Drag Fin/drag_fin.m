% Drag Fin Analysis
% Ian Gomez, 08/15/16
% This script runs a simulation of the rocket in flight with the goal of
% optimizing the size, number, and max angle of the drag fins.

% Primary Method:
% 1. Load the thrust data from ThrustCurve.org
% 2. Interpolate the data for more steps
% 3. Calculate change in mass from burnt prop
% 4. Calculate velocity from the combined impulse due to drag, gravity and thrust
% 5. Calculate altitude and acceleration from velocity
% https://spacegrant.carthage.edu/ojs/index.php/wsc/article/viewFile/23/23

% Implementation of drag fins
% Look at the potential energy (PE = m*g*h) on the control flight
% Reduce PE to PE_desired = m*g*h_desired
% PE-PE_desired = D_df * t_open
% where D_df is the drag of the drag fins and t_open is time the drag fins
% are deployed
% Use the drag equation to get an estimate on size of fins
% Cd_df = D_df/(0.5 * rho * u^2 * S)
% Guess a Cd_df so to close the problem to solve for total area, S, required
% of drag fins

clear; close all; clc;

% Plots
textsize = 15; % font size
linesize = 2;  % line width
plot_thrust = 0;
plot_h_u_a = 0;
plot_forces = 1;

% Rocket and motor characteristics
rocket.l   = 3.44; % length, m
rocket.d   = 0.14; % diameter, m
rocket.Cd  = 0.6;  % coeff of drag
rocket.S   = pi.*(rocket.d./2).^2; % front cross sectional area, m^2
motor.name = 'M1939';
motor.wetmass  = 8.9882; % kg
motor.propmass = 5.7191; % kg
motor.drymass  = motor.wetmass-motor.propmass; % kg
motor.burntime = 6.20;   % s
motor.mdotavg  = (motor.wetmass-motor.propmass)./motor.burntime; % kg/s
rocket.nomotormass = 19.428; % kg 
rocket.drymass = rocket.nomotormass + motor.drymass; % kg
rocket.wetmass = rocket.nomotormass + motor.wetmass; % kg

%% Thrust Curve

% Load thrust curve
fileID = fopen('AeroTech_M1939.txt');
thrust_curve = textscan(fileID,'%f %f','headerLines',5);
fclose(fileID);

% Converts the cells to a matrix and removes the last line
thrust_curve = cell2mat(thrust_curve);
t_thrust = thrust_curve(1:length(thrust_curve)-1,1);
thrust_curve = thrust_curve(1:length(thrust_curve)-1,2);

% Interpolate the data, original data samples at 0.05s or 20 Hz
% Change time step to add discrete data points to the simulation
time_step = 0.02;
t_powered = 0:time_step:t_thrust(end);
T = interp1(t_thrust,thrust_curve,t_powered);

% Plot the data
if plot_thrust == 1
    thrust_fig = figure('Position', [100, 100, 2000, 1000]);
    plot(t_thrust,thrust_curve,'-mo','LineWidth',linesize)
    set(gca,'FontSize',textsize)
    xlim([0 t_thrust(end)])
    title('Thrust Curve')
    xlabel('Time (s)')
    ylabel('Thrust (N)')
    grid on
end

%% Simulation

t = 1:time_step:100;
rho = @(x) 1.2250.*(288.15/(288.15-0.0065.*x)).^...
    (1+9.80665.*0.0289644/(8.31432.*0.0065));
altitude_target = 3048; % m

% inital liftoff conditions and vector initialization
m = rocket.drymass.*ones(1,length(t)); g = 9.81.*ones(1,length(t)); % m/s^2
F = zeros(1,length(t)); u = zeros(1,length(t));
a = zeros(1,length(t)); h = zeros(1,length(t));
dragloss = zeros(1,length(t)); gravityloss = zeros(1,length(t));
currentmomentum = zeros(1,length(t)); mach1 = zeros(1,length(t));
h(1) = 0; % rocket is at sea level
u(1) = 0; % rocket is stationary
m(1) = rocket.wetmass; % rocket is full of prop

for i = 1:length(t)
    
    if t(i) <= motor.burntime %if motor is on
        m(i+1) = m(i)-motor.mdotavg.*time_step;
    end
    
    [~, mach1(i), ~, rho] = atmosisa(h(i)); % retrieves ambient conditions
    k = 0.5.*rocket.Cd.*rocket.S.*rho;
    dragloss(i) = k.*u(i).^2;
    currentmomentum(i) = u(i).*m(i);
    gravityloss(i) = m(i).*g(i);
    
    % Solve out forces on rocket
    if t(i) <= motor.burntime % powered ascent
        F(i) = T(i) - gravityloss(i) - dragloss(i);
    elseif u(i) > 0           % coasting to apogee
        F(i) = -gravityloss(i) - dragloss(i);
    else                      % descent without a parachute
        F(i) = -gravityloss(i) + dragloss(i);
    end
    
    % Solve for new velocity and acceleration
    if(i < length(t)) % checks to not exceed index
        u(i+1) = (1./m(i+1)).*(currentmomentum(i) + ((F(i)).*time_step));
        a(i) = F(i)./m(i);
        h(i+1) = trapz([0 time_step],[u(i) u(i+1)])+h(i);
    end
    
    % silly bullshit to find the landing time
    if t(i) > 10 && h(i) < 0
        t_land(i) = t(i);
        t_land = unique(t_land(:));
        t_land = t_land(2);
    end
end

xlimit = [0 t_land];

if plot_h_u_a == 1
    figure
    %subplot(3,1,1)
    hold on
    plot(t,altitude_target.*ones(1,length(t)),'--')
    %y1 = get(gca,'ylim');
    %plot([t_powered(end),t_powered(end)],y1)
    plot(t,h,'LineWidth',linesize)
    title('Altitude')
    xlabel('Time (s)')
    ylabel('Height (m)')
    xlim(xlimit)
    grid on
    legend('3048m','Burnout','Location','Southwest')
    % label apogee
    dim = [.4 .4 .6 .2];
    str = strcat({'Apogee = '},num2str(max(h)));
    annotation('textbox',dim,'String',str,'FitBoxToText','on');
    
    figure
    %subplot(3,1,2)
    plot(t,mach1,'--',t,u,'LineWidth',linesize)
    title('Velocity')
    xlabel('Time (s)')
    ylabel('Velocity (m/s)')
    xlim(xlimit)
    grid on
    legend('Mach 1','Location','Southwest')
    
    figure
    %subplot(3,1,3)
    plot(t(1:length(a)),a./g,'LineWidth',linesize)
    title('Acceleration')
    xlabel('Time (s)')
    ylabel('g (m/s^2)')
    xlim(xlimit)
    grid on
end

if plot_forces == 1
    figure
    plot(t,gravityloss,t_powered,T,t,dragloss,'LineWidth',linesize)
    title('In-flight Forces')
    xlabel('Time (s)')
    ylabel('Force (N)')
    xlim(xlimit)
    grid on
    legend('Gravity','Thrust','Drag')
end

%% Drag fin characteristics

rocket.burnouth = h(length(t_powered));
rocket.apogee = max(h);

% Energy calculations
e_net = rocket.drymass.*g(1).*rocket.apogee;
e_want = rocket.drymass.*g(end).*altitude_target;
e_loss = e_net - e_want;
e_loss_perc = (e_net - e_want)/e_want; % J
disp('Percentage of energy need to lose to drag')
disp(strcat(num2str(e_loss_perc.*100),'%'))

% distance to altitude target from altitude at 10s
for i = 1:length(t)
    if t(i) == 10
        i10 = i;
    end
end
d2at =  altitude_target - h(i10); % m
D_df = e_loss./d2at;
disp('Amount of drag needed if fins open after 10s')
disp(strcat(num2str(D_df),'N'))

% small angle 
theta_max_deg = 17.5; % degrees
theta_max_rad = 0.3054326; % rad

% factor of safety for control authority
fs = 1.5;
