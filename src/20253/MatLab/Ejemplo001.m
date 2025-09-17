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
s_dia = 1;       % Día a analizar (1=Domingo)
ng = 4;          % Grado del polinomio de ajuste

%% Filtrar datos por día seleccionado
id_d = data(:,1) == s_dia;   % Índices de filas del día seleccionado
x = data(id_d, 2);           % Variable independiente
y = data(id_d, 3);           % Volumen de tráfico
nm = length(x);              % Número de muestras

%% Ajuste polinomial de grado 'ng'
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
