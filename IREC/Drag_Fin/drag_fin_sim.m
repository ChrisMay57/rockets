% Drag Fin Sim
% Ian Gomez,  08/15/16
% Joan Creus, 08/16/16
% This script runs a simulation of the rocket in flight with the goal of
% simulating the flight profile + drag fins

% Primary Method:
% 1. Load the thrust data from ThrustCurve.org
% 2. Interpolate the data for more steps
% 3. Calculate change in mass from burnt prop
% 4. Calculate velocity from the combined impulse due to drag, gravity and thrust
% 5. Calculate altitude and acceleration from velocity
% https://spacegrant.carthage.edu/ojs/index.php/wsc/article/viewFile/23/23

function [h,u,a,time,t,t_powered,mach1,forces,rocket,gravityloss,T,dragloss,...
    parachutedrag,droguedrag] = drag_fin_sim(rocket,motor,parachute,...
    drogue,altitude,dragfin,time)

% -------------------------------------------------------------------------
% Simulation
% -------------------------------------------------------------------------

lam = cos(rocket.launch_angle); % launch angle multiplier for drag terms
drogue.deployed    = rocket.deploydrogue;
parachute.deployed = rocket.deployparachute;
parachute.S        = pi.*(parachute.d./2)^2; % m^2
parachute.deploy_h = altitude.launch_site + parachute.deploy_h;
drogue.S   = pi.*(drogue.d./2)^2;            % m^2
rocket.S   = pi.*(rocket.d./2).^2;           % m^2

[motor,rocket,T,t_powered] = getMotorData(motor,rocket,time);

t = 0:time.step:time.end;                    % s

% Initialize gravity and mass for flight
m = rocket.drymass.*ones(1,length(t));       % kg
g = 9.81.*ones(1,length(t));                 % m/s^2
% Vector initialization for speed
currentmomentum = zeros(1,length(t));
F = zeros(1,length(t)); u = zeros(1,length(t)); dragloss = zeros(1,length(t));
a = zeros(1,length(t)); h = zeros(1,length(t)); gravityloss = zeros(1,length(t));
parachutedrag = zeros(1,length(t)); droguedrag = zeros(1,length(t));
mach1 = zeros(1,length(t));
% Initial conditions
h(1) = altitude.launch_site; % m,   rocket is not at sea level
u(1) = 0;                    % m/s, rocket is stationary
m(1) = rocket.wetmass;       % kg,  rocket is full of prop

% Flight simulation
for i = 1:length(t)
    
    % if motor is on, lose mass
    if t(i) <= motor.burntime
        m(i+1) = m(i)-motor.mdotavg.*time.step;
    end
    
    % Drag calculation
    % The launch angle is taken into account here as well as the drag
    % multiplier for the drag fins
    [~,~,rho,mach1(i)] = getAtmoConditions(h(i));
    k = 0.5.*rocket.Cd.*rocket.S.*rho;
    if (t(i)>dragfin.deploy_t && dragfin.deploy_t > 0 && u(i) > 0)
        k = dragfin.extra_drag_percent*k;
    end
    dragloss(i)        = lam.*k.*u(i).^2;
    
    % Gravity loss and current momentum calculation
    gravityloss(i)     = m(i).*g(i);
    currentmomentum(i) = u(i).*m(i);
    
    % Recovery drag calculation
    % First if statement checks if the rocket is past the drogue deployment
    % speed and above parachute height
    % Second if statemnet checks if the rocket is below the parachute
    % deployment height
    if u(i) < drogue.deploy_u && h(i) > parachute.deploy_h && rocket.deploydrogue == 1
        droguedrag(i)    = lam.*0.5.*drogue.Cd.*drogue.S.*rho.*u(i).^2;
    elseif h(i) < parachute.deploy_h && u(i) < 0 && t(i) > 1 && rocket.deployparachute == 1
        parachutedrag(i) = lam.*0.5.*parachute.Cd.*parachute.S.*rho.*u(i).^2;
    end
    
    % Solve out forces on rocket
    if t(i) <= motor.burntime          % powered ascent
        F(i) = - gravityloss(i) - dragloss(i) + lam.*T(i);
    elseif u(i) > 0                    % coasting to apogee
        F(i) = - gravityloss(i) - dragloss(i);
    elseif 0 >= u(i) > drogue.deploy_u % descent
        F(i) = - gravityloss(i) + dragloss(i) + droguedrag(i)...
            + parachutedrag(i);
    end
    
    % Solve for new velocity and acceleration
    if(i < length(t)) % checks to not exceed index
        u(i+1)  = (1./m(i+1)).*(currentmomentum(i) + ((F(i)).*time.step));
        a(i)    = F(i)./m(i);
        h(i+1)  = trapz([0 time.step],[u(i) u(i+1)])+h(i);
    end
    
    % Store time at apogee and time at landing
    % u(i) < 0 is apogee; h(i) < 0 is landing
    if t(i) > 5 && h(i) >= (0 + altitude.launch_site)
        time.land = t(i);
    end
    if t(i) > 5 && u(i) > 0
        time.apogee = t(i);
    end
end

% Reset altitude for plotting. Air density already taken into account
h = h-altitude.launch_site;

% Store useful information
rocket.flight_time = time.land;
rocket.burnout_h = h(length(t_powered));
rocket.apogee = max(h);

forces = [gravityloss,T,dragloss,parachutedrag,droguedrag];
end