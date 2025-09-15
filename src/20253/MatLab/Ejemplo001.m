clc;
close all;
clear;

if ispc
    d = '\';
else
    d = '/';
end

data = load('Metro_Interstate_Traffic_Volume_pdf.txt');
v_dias = ["Domingo", "Lunes", "Martes", "Miercoles", ...
            "Jueves", "Viernes", "Sabado"];

s_dia = 1;
ng = 4;
id_d = data(:,1)==s_dia;
x = data(id_d, 2);
y = data(id_d, 3);
nm = length(x);

ca = (x*ones(1, ng+1)).^(ones(nm,1)*(0:ng))\y;
ye = ((x*ones(1, ng+1)).^(ones(nm,1)*(0:ng)))*ca;
dca = ca(2:(ng+1)).*(1:ng)';
d2ca = dca(2:ng).*(1:(ng-1))';
rdca = roots(flip(dca));
rye = ((rdca*ones(1, ng+1)).^(ones(ng-1,1)*(0:ng)))*ca;


figure(1);
bar(x, y);
xlabel(v_dias(s_dia));
ylabel("Personas");
grid on;
hold on;
plot(x, ye, 'r-');
plot(rdca, rye, 'bo');

