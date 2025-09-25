%% ================================================================
%  Análisis de Volumen de Tráfico en el Metro Interestatal
%  Autor: Mario Alberto Ibarra Manzano
%  Descripción:
%  Este script carga una base de datos en formato .txt con el volumen
%  de tráfico del metro interestatal, filtra la información de un día 
%  específico y realiza un ajuste polinomial para modelar el comportamiento.
%  Posteriormente, calcula las raíces de la derivada para identificar
%  puntos críticos (máximos o mínimos locales) y grafica los resultados.
% ================================================================

%% Limpieza del entorno
clc;            % Limpia la ventana de comandos
close all;      % Cierra todas las figuras abiertas
clear;          % Borra todas las variables en memoria

%% Configuración de separador de directorios según SO
if ispc
    d = '\';    % Para Windows
else
    d = '/';    % Para Linux/Mac
end

%% Definir ruta base y archivo de datos
path_bd = pwd;                                     % Directorio actual
id_pt = find(path_bd==d);                          % Índices de separadores
path_bd = [path_bd(1:id_pt(end-2)) 'data' d];      % Ruta hacia la carpeta 'data'
file_bd = [path_bd, 'Metro_Interstate_Traffic_Volume_pdf.txt'];

% Si no se encuentra el archivo, abrir ventana para seleccionarlo
while ~isfile(file_bd)
    [filename, pathname] = uigetfile('*.txt', 'Base de datos (txt)', ...
        'Seleccione la base de datos');
    file_bd = [pathname filename];
end

%% Cargar base de datos
% Se espera que el archivo tenga columnas:
% Columna 1 = día de la semana (1=Domingo, 2=Lunes, ..., 7=Sábado)
% Columna 2 = variable independiente (ej. hora, índice)
% Columna 3 = volumen de tráfico (personas)
data = load(file_bd);

%% Definir etiquetas de los días de la semana
v_dias = ["Domingo", "Lunes", "Martes", "Miercoles", ...
          "Jueves", "Viernes", "Sabado"];

%% Selección de parámetros de análisis
s_dia = unique(data(:,1));       % Día a analizar
n_dia = length(s_dia);
ng = 4;          % Grado del polinomio de ajuste

er = zeros(1, n_dia);
for i1=1:n_dia
    id_ts = (data(:,1) == s_dia(i1));
    id_tr = ~id_ts;
    x = data(id_tr, 2);           % Variable independiente
    y = data(id_tr, 3);           % Volumen de tráfico
    xt = data(id_ts, 2);
    yt = data(id_ts, 3);
    nm = length(x);              % Número de muestras
    % Se construye la matriz de Vandermonde para resolver el sistema
    ca = (x*ones(1, ng+1)).^(ones(nm,1)*(0:ng)) \ y;

    % Evaluación del polinomio ajustado
    ye = ((x*ones(1, ng+1)).^(ones(nm,1)*(0:ng))) * ca;

    %% Cálculo de derivadas y raíces
    dca = ca(2:(ng+1)).*(1:ng)';        % Coeficientes de la 1a derivada
    d2ca = dca(2:ng).*(1:(ng-1))';      % Coeficientes de la 2a derivada
    rdca = roots(flip(dca));            % Raíces de la 1ª derivada (puntos críticos)
    
    % Evaluación del polinomio en los puntos críticos
    rye = ((rdca*ones(1, ng+1)).^(ones(ng-1,1)*(0:ng))) * ca;

    pd = rye(ng/2);
    pd_s = ceil(pd)-pd;
    pd_i = pd-floor(pd);
    md = zeros(2, 3);
    xi = x(floor(rdca(2))>=x);
    hi = y(floor(rdca(2))>=x);
    id_xi = xi==floor(rdca(2));
    xs = x(floor(rdca(2))<=x);
    hs = y(floor(rdca(2))<=x);
    id_xs = xs==floor(rdca(2));
    hi(id_xi)=round(hi(id_xi)*pd_i);
    hs(id_xs) = hs(id_xs)-hi(id_xi);
    nxiu = length(unique(xi));
    nxsu = length(unique(xs));
    
    md(1, 1) = (xi'*hi)/sum(hi);
    md(1, 2) = ((xi.^2)'*hi)/sum(hi)-md(1, 1)^2;
    id_xis1 = ones(n_dia-1,1)*(1:nxiu);
    id_xis2 = (0:n_dia-2)'*ones(1,nxiu)*nxiu;
    md(1, 3) = mean(max(hi(id_xis1+id_xis2),[],2));
    md(2, 1) = (xs'*hs)/sum(hs);
    md(2, 2) = ((xs.^2)'*hs)/sum(hs)-md(2, 1)^2;
    id_xss1 = ones(n_dia-1,1)*(1:nxsu);
    id_xss2 = (0:n_dia-2)'*ones(1,nxsu)*nxsu;
    md(2, 3) = mean(max(hs(id_xss1+id_xss2),[],2));
    
    np = 500;
    vx = min(x):(max(x)-min(x))/np:max(x);
    vy1 = md(1,3)*exp(-((vx-md(1,1)).^2)/(2*md(1,2)));
    vy2 = md(2,3)*exp(-((vx-md(2,1)).^2)/(2*md(2,2)));
    vy = [vy1(vx<=rdca(2)) vy2(vx>rdca(2))];
    
    dm = md(1,1)-md(2,1);
    ds2 = md(1,2)-md(2,2);
    xr1 = (2*md(2,1)*md(1,2)-2*md(1,1)*md(2,2)+sqrt(md(1,2)*md(2,2)*(4*dm*dm-log((md(1,3)/md(2,3))^(8*ds2)))))/(2*ds2);
    xr2 = (2*md(2,1)*md(1,2)-2*md(1,1)*md(2,2)-sqrt(md(1,2)*md(2,2)*(4*dm*dm-log((md(1,3)/md(2,3))^(8*ds2)))))/(2*ds2);
    if(xr1>md(1,1)&&xr1<md(2,1))
        xr = xr1;
    else
        xr = xr2;
    end
    vy2 = [vy1(vx<=xr) vy2(vx>xr)];

    ye1 = md(1,3)*exp(-((xt-md(1,1)).^2)/(2*md(1,2)));
    ye2 = md(2,3)*exp(-((xt-md(2,1)).^2)/(2*md(2,2)));
    ye = [ye1(xt<=xr); ye2(xt>xr)];
    er(i1) = sum((yt-ye).^2)/length(xt);
end
fprintf("%.4f+-%.4f\n", mean(er), std(er));

%% Visualización de resultados
figure(1);
bar(x, y);                          % Histograma del tráfico
xlabel(v_dias(s_dia));              % Etiqueta del eje X (día seleccionado)
ylabel("Personas");                 % Etiqueta del eje Y
grid on;                            % Activar la cuadrícula
hold on;

plot(x, ye, 'r-', 'LineWidth', 2);  % Curva polinomial ajustada
plot(rdca, rye, 'bo', 'MarkerSize', 8, 'MarkerFaceColor', 'b'); % Puntos críticos

title("Análisis de Volumen de Tráfico - " + v_dias(s_dia));
legend("Datos originales", "Ajuste polinomial", "Puntos críticos");
print('-f1', '-djpeg90', '-r300', 'Figura1.jpg');

figure(2);
bar(x, y);                          % Histograma del tráfico
xlabel(v_dias(s_dia));              % Etiqueta del eje X (día seleccionado)
ylabel("Personas");                 % Etiqueta del eje Y
grid on;                            % Activar la cuadrícula
hold on;
plot(vx, vy, 'r-', 'LineWidth', 2);

figure(3);
bar(x, y);                          % Histograma del tráfico
xlabel(v_dias(s_dia));              % Etiqueta del eje X (día seleccionado)
ylabel("Personas");                 % Etiqueta del eje Y
grid on;                            % Activar la cuadrícula
hold on;
plot(vx, vy2, 'r-', 'LineWidth', 2);
