% csv format
formatSpec = '%C%C%C%d%f';

% load csv
T = readtable(...
    '../../../data/anymal-stand/sample.csv', ...
    'Delimiter', ',', ...
    'Format',formatSpec ...
    );

% constants
numIter = 50000;

% data from rai
RAI = T(T.SIM == 'RAI', :);
[RAI_G, RAI_numrows] = findgroups(RAI.NUMROW);
RAI_mins = splitapply(@min, RAI.TIME, RAI_G);

% data from bullet
BT = T(T.SIM == 'BULLET', :);
[BT_G, BT_numrows] = findgroups(BT.NUMROW);
BT_mins = splitapply(@min, BT.TIME, BT_G);

% data from dart-dantzig-bullet
DART_DAN_BT = T(...
    T.SIM == 'DART' ...
    & T.SOLVER == 'DANTZIG' ...
    & T.DETECTOR == 'BULLET', :);
[DART_DAN_BT_G, DART_DAN_BT_numrows] = findgroups(DART_DAN_BT.NUMROW);
DART_DAN_BT_mins = splitapply(...
    @min, ...
    DART_DAN_BT.TIME, ...
    DART_DAN_BT_G);

% data from dart-pgs-bullet
DART_PGS_BT = T(...
    T.SIM == 'DART' ...
    & T.SOLVER == 'PGS' ...
    & T.DETECTOR == 'BULLET', :);
[DART_PGS_BT_G, DART_PGS_BT_numrows] = findgroups(DART_PGS_BT.NUMROW);
DART_PGS_BT_mins = splitapply(...
    @min, ...
    DART_PGS_BT.TIME, ...
    DART_PGS_BT_G);

% data from dart-dantzig-ode
DART_DAN_ODE = T(...
    T.SIM == 'DART' ...
    & T.SOLVER == 'DANTZIG' ...
    & T.DETECTOR == 'ODE', :);
[DART_DAN_ODE_G, DART_DAN_ODE_numrows] = findgroups(DART_DAN_ODE.NUMROW);
DART_DAN_ODE_mins = splitapply(...
    @min, ...
    DART_DAN_ODE.TIME, ...
    DART_DAN_ODE_G);

% data from dart-pgs-ode
DART_PGS_ODE = T(...
    T.SIM == 'DART' ...
    & T.SOLVER == 'PGS' ...
    & T.DETECTOR == 'ODE', :);
[DART_PGS_ODE_G, DART_PGS_ODE_numrows] = findgroups(DART_PGS_ODE.NUMROW);
DART_PGS_ODE_mins = splitapply(...
    @min, ...
    DART_PGS_ODE.TIME, ...
    DART_PGS_ODE_G);

% data from mujoco-pgs
MJC_PGS = T(...
    T.SIM == 'MUJOCO' ...
    & T.SOLVER == 'PGS-NOSLIP', :);
[MJC_PGS_G, MJC_PGS_numrows] = findgroups(MJC_PGS.NUMROW);
MJC_PGS_mins = splitapply(...
    @min, ...
    MJC_PGS.TIME, ...
    MJC_PGS_G);

% data from mujoco-cg
MJC_CG = T(...
    T.SIM == 'MUJOCO' ...
    & T.SOLVER == 'CG-NOSLIP', :);
[MJC_CG_G, MJC_CG_numrows] = findgroups(MJC_CG.NUMROW);
MJC_CG_mins = splitapply(...
    @min, ...
    MJC_CG.TIME, ...
    MJC_CG_G);

% data from mujoco-newton
MJC_NEWTON = T(...
    T.SIM == 'MUJOCO' ...
    & T.SOLVER == 'NEWTON-NOSLIP', :);
[MJC_NEWTON_G, MJC_NEWTON_numrows] = findgroups(MJC_NEWTON.NUMROW);
MJC_NEWTON_mins = splitapply(...
    @min, ...
    MJC_NEWTON.TIME, ...
    MJC_NEWTON_G);

% data from ODE
ODE = T(T.SIM == 'ODE', :);
[ODE_G, ODE_numrows] = findgroups(ODE.NUMROW);
ODE_mins = splitapply(...
    @min, ...
    ODE.TIME, ...
    ODE_G);

%% plot
% linear
figure('Name', 'linear', 'Position', [0, 0, 800, 600])
plot(RAI_numrows.^2, RAI_mins, ...
    plotspec.RAIRAI{1}, ...
    'color', plotspec.RAIRAI{3}, ...
    'DisplayName', plotspec.RAIRAI{2})
hold on
plot(BT_numrows.^2, BT_mins, ...
    plotspec.BULLETMULTIBODY{1}, ...
    'color', plotspec.BULLETMULTIBODY{3}, ...
    'DisplayName', plotspec.BULLETMULTIBODY{2})
plot(DART_DAN_BT_numrows.^2, DART_DAN_BT_mins, ...
    plotspec.DARTDANTZIG{1}, ...
    'color', plotspec.DARTDANTZIG{3}, ...
    'DisplayName', plotspec.DARTDANTZIG{2})
plot(DART_PGS_BT_numrows.^2, DART_PGS_BT_mins, ...
    plotspec.DARTPGS{1}, ...
    'color', plotspec.DARTPGS{3}, ...
    'DisplayName', plotspec.DARTPGS{2})
plot(MJC_PGS_numrows.^2, MJC_PGS_mins, ...
    plotspec.MUJOCOPGS{1}, ...
    'color', plotspec.MUJOCOPGS{3}, ...
    'DisplayName', plotspec.MUJOCOPGS{2})
plot(MJC_CG_numrows.^2, MJC_CG_mins, ...
    plotspec.MUJOCOCG{1}, ...
    'color', plotspec.MUJOCOCG{3}, ...
    'DisplayName', plotspec.MUJOCOCG{2})
plot(MJC_NEWTON_numrows.^2, MJC_NEWTON_mins, ...
    plotspec.MUJOCONEWTON{1}, ...
    'color', plotspec.MUJOCONEWTON{3}, ...
    'DisplayName', plotspec.MUJOCONEWTON{2})
plot(ODE_numrows.^2, ODE_mins, ...
    plotspec.ODESTANDARD{1}, ...
    'color', plotspec.ODESTANDARD{3}, ...
    'DisplayName', plotspec.ODESTANDARD{2})
% plot(DART_DAN_ODE_numrows.^2, DART_DAN_ODE_mins, '-m.') % redundant
% plot(DART_PGS_ODE_numrows.^2, DART_PGS_ODE_mins, '-mo') % redundant
xlabel('number of robots (n)')
ylabel(sprintf('50k simulation time (sec) \n FAST ←'))
legend('Location', 'eastoutside')
title('ANYmal PD control test')
hold off
saveas(gcf,'plots/sampleplot.eps','epsc')
saveas(gcf,'plots/sampleplot.fig','fig')
saveas(gcf,'plots/sampleplot.png')

% log scale
figure('Name', 'log', 'Position', [0, 0, 800, 600])
plot(RAI_numrows.^2, RAI_mins, ...
    plotspec.RAIRAI{1}, ...
    'color', plotspec.RAIRAI{3}, ...
    'DisplayName', plotspec.RAIRAI{2})
hold on
plot(BT_numrows.^2, BT_mins, ...
    plotspec.BULLETMULTIBODY{1}, ...
    'color', plotspec.BULLETMULTIBODY{3}, ...
    'DisplayName', plotspec.BULLETMULTIBODY{2})
plot(DART_DAN_BT_numrows.^2, DART_DAN_BT_mins, ...
    plotspec.DARTDANTZIG{1}, ...
    'color', plotspec.DARTDANTZIG{3}, ...
    'DisplayName', plotspec.DARTDANTZIG{2})
plot(DART_PGS_BT_numrows.^2, DART_PGS_BT_mins, ...
    plotspec.DARTPGS{1}, ...
    'color', plotspec.DARTPGS{3}, ...
    'DisplayName', plotspec.DARTPGS{2})
plot(MJC_PGS_numrows.^2, MJC_PGS_mins, ...
    plotspec.MUJOCOPGS{1}, ...
    'color', plotspec.MUJOCOPGS{3}, ...
    'DisplayName', plotspec.MUJOCOPGS{2})
plot(MJC_CG_numrows.^2, MJC_CG_mins, ...
    plotspec.MUJOCOCG{1}, ...
    'color', plotspec.MUJOCOCG{3}, ...
    'DisplayName', plotspec.MUJOCOCG{2})
plot(MJC_NEWTON_numrows.^2, MJC_NEWTON_mins, ...
    plotspec.MUJOCONEWTON{1}, ...
    'color', plotspec.MUJOCONEWTON{3}, ...
    'DisplayName', plotspec.MUJOCONEWTON{2})
plot(ODE_numrows.^2, ODE_mins, ...
    plotspec.ODESTANDARD{1}, ...
    'color', plotspec.ODESTANDARD{3}, ...
    'DisplayName', plotspec.ODESTANDARD{2})
% plot(DART_DAN_ODE_numrows.^2, DART_DAN_ODE_mins, '-m.') % redundant
% plot(DART_PGS_ODE_numrows.^2, DART_PGS_ODE_mins, '-mo') % redundant
xlabel('number of robots (log n)')
ylabel(sprintf('50k simulation time (log sec) \n FAST ←'))
legend('Location', 'eastoutside')
hold off
title('ANYmal PD control test (log scale)')
ylim([0, 10^4.5])
xlim([0, 10^2.5])
set(gca, 'YScale', 'log', 'XScale', 'log')
saveas(gcf,'plots/sampleplot-log.eps','epsc')
saveas(gcf,'plots/sampleplot-log.fig','fig')
saveas(gcf,'plots/sampleplot-log.png')

% speed bar graph (1 anymal)
c = categorical({...
    'Rai';...
    'BtMultibody';...
    'DartDantzig';...
    'DartPGS';...
    'MjcPGS';...
    'MjcCG';...
    'MjcNewton';...
    'OdeStd'
    });
values = [...
    numIter / RAI_mins(1)           / 1000; ...
    numIter / BT_mins(1)            / 1000; ...
    numIter / DART_DAN_BT_mins(1)   / 1000; ...
    numIter / DART_PGS_BT_mins(1)   / 1000; ...
    numIter / MJC_PGS_mins(1)       / 1000; ...
    numIter / MJC_CG_mins(1)        / 1000; ...
    numIter / MJC_NEWTON_mins(1)    / 1000; ...
    numIter / ODE_mins(1)           / 1000 ...
    ];

T2 = table(c, values);
T2.Properties.VariableNames = {'sim','speed'};
T2 = sortrows(T2, 2, 'descend');

T2.sim = reordercats(T2.sim,cellstr(T2.sim));

figure('Name', 'speed', 'Position', [0, 0, 800, 600])
bar(T2.sim(1), T2.speed(1), 'FaceColor', plotspec.RAIRAI{3})            
hold on
bar(T2.sim(2), T2.speed(2), 'FaceColor', plotspec.MUJOCOPGS{3})         
bar(T2.sim(3), T2.speed(3), 'FaceColor', plotspec.MUJOCOCG{3})          
bar(T2.sim(4), T2.speed(4), 'FaceColor', plotspec.MUJOCONEWTON{3})      
bar(T2.sim(5), T2.speed(5), 'FaceColor', plotspec.BULLETMULTIBODY{3})   
bar(T2.sim(6), T2.speed(6), 'FaceColor', plotspec.DARTDANTZIG{3})       
bar(T2.sim(7), T2.speed(7), 'FaceColor', plotspec.DARTPGS{3})      
bar(T2.sim(8), T2.speed(8), 'FaceColor', plotspec.ODESTANDARD{3})  
hold off
title('ANYmal PD control test (1 robot)')
% numbers on bars
text(1:length(T2.speed), ...
    T2.speed, ...
    num2str(T2.speed, '%0.2f'),...
    'vert', 'bottom', ...
    'horiz','center', ...
    'FontWeight','bold');
ylabel(sprintf('timestep per second (kHz) \n FAST →'))
ylim([0, 110])

saveas(gcf,'plots/samplebar.eps','epsc')
saveas(gcf,'plots/samplebar.fig','fig')
saveas(gcf,'plots/samplebar.png')