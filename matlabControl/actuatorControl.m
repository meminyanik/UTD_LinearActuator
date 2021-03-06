%%% First lis the connections
% seriallist

sPort = serial('COM5','BaudRate',115200);
fopen(sPort);

%%% Command
% V for vertical actuator
% H for horizontal actuator
% The number of steps

pulsePerRevolution = 3200;

% Test for One Revolution
fprintf(sPort,'V3200');

% revolutionPerMinute = 60;

distancePerRevolution = 5; % mm

desiredMovementH = 5; % mm
desiredMovementV = 1.1;

nPulseH = round(desiredMovementH / distancePerRevolution * pulsePerRevolution);
nPulseV = round(desiredMovementV / distancePerRevolution * pulsePerRevolution);



fprintf(sPort,['H' num2str(nPulseH)]);
% pause(1/revolutionPerMinute*abs(nPulseH)/pulsePerRevolution*60);
fprintf(sPort,['V' num2str(nPulseV)]);
% pause(1/revolutionPerMinute*abs(nPulseV)/pulsePerRevolution*60);


fclose(sPort);

